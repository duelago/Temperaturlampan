#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ElegantOTA.h>

#define OLED_RESET 0

Adafruit_SSD1306 display(OLED_RESET);

#define PIN            D4  // Define the pin for WS2811 LEDs
#define NUM_LEDS       1   // Define the number of LEDs
#define LED_BRIGHTNESS 255  // Define LED brightness (0-255)
#define EEPROM_SIZE    5   // Size of EEPROM for storing METAR station code (max 4 characters + null terminator)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

char METARStation[EEPROM_SIZE];
String METAR; // Declare METAR as a global variable
String songTitle; // Declare songTitle as a global variable

const unsigned long REBOOT_INTERVAL = 24 * 60 * 60 * 1000; // 24 hours in milliseconds
unsigned long previousMillis = 0;

const char* apiUrl = "https://listenapi.planetradio.co.uk/api9.2/nowplaying/mme";

bool blinkLED = false; // Flag to control LED blinking
bool isBlinking = false; // Flag to track if LED is currently blinking

unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500; // Blink interval in milliseconds

bool songPlayed = false; // Flag to track if song has been played
unsigned long lastSongPlayTime = 0; // Timer for last song play time
const unsigned long songPlayInterval = 60 * 60 * 1000; // 1 hour in milliseconds

// Arduino note for complete Last Christmas by Wham

const int tempo = 120; // beats per minute

// Notes for Last Christmas
const int notes[] = {
  262, 392, 440, 349, // Frequencies for C4, G4, A4, F4
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698, // Frequencies for C5, G5, A5, F5

  262, 392, 440, 349,
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698,

  262, 392, 440, 349,
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698,

  262, 392, 440, 349,
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698,
};

// Durations for Last Christmas
const int durations[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,

  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,

  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,

  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
};

const int buzzerPin = D5; // Pin D5

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.begin(115200);
  MDNS.begin("whamageddonlampan");
  ElegantOTA.begin(&server);

  // Initialize NeoPixel strip
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'

  // Configure the tone pin
  pinMode(buzzerPin, OUTPUT);

  // Use WiFiManager to set WiFi credentials if they are not already configured
  WiFiManager wifiManager;
  wifiManager.autoConnect("Temperaturlampan");

  // Load METAR station code from EEPROM
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; ++i) {
    METARStation[i] = EEPROM.read(i);
  }
  EEPROM.end();

  // Fetch METAR data as soon as possible after boot
  fetchMETARData();

  // Start web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check song title every 30 seconds
  static unsigned long lastCheckTime = 0;
  const unsigned long checkInterval = 30000;  // 30 seconds in milliseconds

  if (currentMillis - lastCheckTime >= checkInterval) {
    lastCheckTime = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClientSecure client;
      client.setInsecure(); // Ignore SSL certificate verification

      HTTPClient https;
      https.begin(client, apiUrl);

      int httpCode = https.GET();

      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        https.end();

        StaticJsonDocument<1200> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }

        handleJsonObject(doc.as<JsonObject>());
      } else {
        Serial.printf("[HTTP] GET request failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
    } else {
      Serial.println("WiFi Disconnected. Skipping song check.");
    }
  }

  // Check if it's time to play the song again
  if (songPlayed && currentMillis - lastSongPlayTime >= songPlayInterval) {
    songPlayed = false; // Reset the flag to play the song
  }

  // Check if the LED should blink
  if (blinkLED || (songPlayed && !isBlinking)) {
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      isBlinking = !isBlinking; // Toggle blinking state

      if (isBlinking) {
        // If the song is not playing, blink the LED
        if (!songPlayed) {
          strip.setPixelColor(0, strip.Color(255, 0, 0)); // Röd
          strip.show();
        }
      } else {
        strip.setPixelColor(0, strip.Color(0, 0, 0)); // Turn off the LED
        strip.show();
      }
    }
  }

  // Play the song if the song title is set to true and it hasn't been played recently
  if (songTitle == "Last Christmas" && !songPlayed) {
    playSong();
  }

  // Handle HTTP server requests
  server.handleClient();
}

// Callback function to handle the JSON object
void handleJsonObject(JsonObject obj) {
  const char* trackTitle = obj["TrackTitle"];
  songTitle = trackTitle;
  Serial.print("Received Track Title: ");
  Serial.println(songTitle);

  // If the song title is "Last Christmas", start blinking the LED and set the songPlayed flag to false
  if (songTitle == "Last Christmas") {
    Serial.println("Whamageddon!! Setting blinkLED flag to true.");
    blinkLED = true;
    isBlinking = true; // Set isBlinking to true when starting LED blinking
    songPlayed = false; // Reset the flag indicating the song has been played
  } else {
    // Turn off the LED if the song title is not "Last Christmas". Keep the blinkLED flag false.
    Serial.println("Song is not Last Christmas. Keeping blinkLED flag false.");
    blinkLED = false;
    isBlinking = false; // Set isBlinking to false if not blinking
    setLEDColor(parseTemperature(METAR).toFloat()); // Update LED color based on current METAR data
  }
}

// Function to parse temperature from METAR string
String parseTemperature(const String& metar) {
  // Find the position of "Temperature: " in the METAR string
  int tempPos = metar.indexOf("Temperature: ");
  if (tempPos != -1) {
    // Extract the substring starting from "Temperature: "
    String tempSubstring = metar.substring(tempPos);

    // Find the position of "(" and ")" to extract the Celsius temperature
    int openParenthesisPos = tempSubstring.indexOf('(');
    int closeParenthesisPos = tempSubstring.indexOf(')');

    if (openParenthesisPos != -1 && closeParenthesisPos != -1) {
      // Extract the substring between "(" and ")"
      String celsiusString = tempSubstring.substring(openParenthesisPos + 1, closeParenthesisPos);
      return celsiusString;
    }
  }

  // Return an empty string if parsing fails
  return "";
}

// Function to set LED color based on temperature
void setLEDColor(float tempLed) {
  uint32_t color = 0;

  if (tempLed >= 0 && tempLed <= 5) {
    color = strip.Color(255, 0, 0); // Grön
  } else if (tempLed >= -5 && tempLed < 0) {
    color = strip.Color(0, 0, 255); // Blå
  } else if (tempLed >= -50 && tempLed < -5) {
    color = strip.Color(0, 255, 255); // Lila
  } else if (tempLed >= 6 && tempLed <= 10) {
    color = strip.Color(255, 255, 0); // Gul
  } else if (tempLed >= 11 && tempLed <= 20) {
    color = strip.Color(153, 255, 0); // Orange
  } else if (tempLed >= 21 && tempLed <= 60) {
    color = strip.Color(0, 255, 0); // Röd
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// Function to play the song once
void playSong() {
  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
    tone(buzzerPin, notes[i], durations[i] * 60000 / tempo);
    delay(durations[i] * 60000 / tempo);
  }

  songPlayed = true; // Set the flag indicating the song has been played
  lastSongPlayTime = millis(); // Record the time the song was played
}
