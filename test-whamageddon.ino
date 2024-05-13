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

const int pin = D5; // Pin D5

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

  // Check if 30 minutes have passed since the last METAR data fetch
  const unsigned long METARFetchInterval = 30 * 60 * 1000; // 30 minutes in milliseconds
  static unsigned long lastMETARFetchTime = 0;

  if (currentMillis - lastMETARFetchTime >= METARFetchInterval) {
    lastMETARFetchTime = currentMillis;
    fetchMETARData();
  }

  // Non-blocking LED blinking
  if (blinkLED) {
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      isBlinking = !isBlinking; // Toggle blinking state

      if (isBlinking) {
        strip.setPixelColor(0, strip.Color(255, 0, 0)); // Röd
        strip.show();
        playSong(); // Play the song when LED blinks
      } else {
        strip.setPixelColor(0, strip.Color(0, 255, 0)); // Grön
        strip.show();
        noTone(pin); // Stop playing the song when LED stops blinking
      }
    }
  }

  // Handle HTTP server requests
  server.handleClient();
}

void playSong() {
  // Play each note in the sequence
  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
    tone(pin, notes[i], durations[i] * 60000 / tempo);
    delay(durations[i] * 60000 / tempo);
  }
}

// Callback function to handle the JSON object
void handleJsonObject(JsonObject obj) {
  const char* trackTitle = obj["TrackTitle"];
  songTitle = trackTitle;
  Serial.print("Received Track Title: ");
  Serial.println(songTitle);

  // If the song title is "Last Christmas", start blinking the LED
  if (songTitle == "Bättre Nu") {
    Serial.println("Whamageddon!! Setting blinkLED flag to true.");
    blinkLED = true;
    isBlinking = true; // Set isBlinking to true when starting LED blinking
  } else {
    // Turn off the LED if the song title is not "X"
    Serial.println("Song is not Last Christmas. Keeping blinkLED flag false.");
    blinkLED = false;
    isBlinking = false; // Set isBlinking to false if not blinking
    setLEDColor(parseTemperature(METAR).toFloat()); // Update LED color based on current METAR data
  }
}

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

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f4f4f4; margin: 0; padding: 0; }";
  html += ".container { max-width: 800px; margin: 0 auto; padding: 20px; background-color: #ffffff; box-shadow: 0 0 10px rgba(0,0,0,0.1); border-radius: 10px; margin-top: 20px; }";
  html += ".input-group { margin-bottom: 20px; }";
  html += ".input-group input[type='text'] { width: 80%; padding: 8px; font-size: 16px; border: 1px solid #ccc; border-radius: 5px; }";
  html += ".input-group input[type='submit'] { width: 18%; padding: 8px; font-size: 16px; border: none; background-color: #4caf50; color: #ffffff; cursor: pointer; border-radius: 5px; }";
  html += ".metar-info { font-size: 18px; margin-top: 20px; }";
  html += ".update-link { text-decoration: none; color: #4caf50; font-weight: bold; margin-top: 20px; display: block; }";
  html += "</style></head><body>";
  html += "<div class='container'><h1>METAR-kod:</h1>";
  html += "<div class='input-group'><form method='post' action='/submit'>";
  html += "<input type='text' name='stationCode' placeholder='ESSB' />";
  html += "<input type='submit' value='Skicka' />";
  html += "</form>";
  html += "<p>ESGG GöTEBORG/Landvetter<br>";
  html += "ESOK KARLSTAD<br>";
  html += "ESNQ KIRUNA<br>";
  html += "ESMS MALMö<br>";
  html += "ESSA STOCKHOLM/Arlanda<br>";
  html += "ESSB STOCKHOLM/Bromma<br>";
  html += "ESKN STOCKHOLM/Skavsta<br>";
  html += "ESOW STOCKHOLM/Västerås<br>";
  html += "<p>";
  html += "ESNX Arvidsjaur<br>";
  html += "ESSD Borlänge<br>";
  html += "ESNG Gällivare<br>";
  html += "ESMT Halmstad (F14)<br>";
  html += "ESUT Hemavan<br>";
  html += "ESGJ Jönköping<br>";
  html += "ESMQ Kalmar (F12)<br>";
  html += "ESNK Kramfors/Sollefteå<br>";
  html += "ESMK Kristianstad/Everöd<br>";
  html += "ESCF Linköping/Malmen (F3)<br>";
  html += "ESTL Ljungbyhed (F5)<br>";
  html += "ESPA Luleå/Kallax<br>";
  html += "ESMH Mora/Siljan<br>";
  html += "ESNZ Örnsköldsvik<br>";
  html += "ESGY Östersund/Frösön<br>";
  html += "ESGK Skövde<br>";
  html += "ESSE Uppsala<br>";
  html += "ESSF Hagshult (F17)<br>";
  html += "ESSV Visby<br>";
  html += "<p>";
  html += "ESFI Arvidsjaur<br>";
  html += "ESFH Barkarby<br>";
  html += "ESFI Frösön<br>";
  html += "ESFQ Hallviken<br>";
  html += "ESFH Hemavan<br>";
  html += "ESFK Jokkmokk<br>";
  html += "ESUJ Jukkasjärvi<br>";
  html += "ESGT Siljansnäs<br>";
  html += "ESFR Sundsvall-Härnösand<br>";
  html += "ESVQ Sveg<br>";
  html += "ESGC Tierp<br>";
  html += "ESGL Vidsel<br>";
  html += "</div>";
  html += "<div class='metar-info'>";
  html += "<h2>Aktuell METAR för ";
  html += METARStation;
  html += ":</h2>";
  html += METAR;
  html += "</div>";
  html += "<a href='/update' class='update-link'>Uppdatera lampans mjukvara</a>";
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void handleSubmit() {
  if (server.hasArg("stationCode")) {
    String code = server.arg("stationCode");
    code.toCharArray(METARStation, EEPROM_SIZE);

    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < EEPROM_SIZE; ++i) {
      EEPROM.write(i, METARStation[i]);
    }
    EEPROM.end();

    Serial.print("New METAR station code set: ");
    Serial.println(METARStation);
  }
  server.sendHeader("Location", String("/"), true);
  server.send(303, "text/plain", "Redirecting to /");
}

void fetchMETARData() {
  // Send HTTP request to obtain METAR data
  String url = "http://tgftp.nws.noaa.gov/data/observations/metar/stations/";
  url += METARStation;
  url += ".TXT";

  Serial.print("Sending request to URL: ");
  Serial.println(url);

  WiFiClient client;
  if (client.connect("tgftp.nws.noaa.gov", 80)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: tgftp.nws.noaa.gov\r\n" +
                 "Connection: close\r\n\r\n");
    delay(500);

    // Read response from server
    String response = "";
    while (client.available()) {
      String line = client.readStringUntil('\r');
      response += line;
    }

    // Extract METAR data from response
    int startPos = response.indexOf("METAR ");
    if (startPos != -1) {
      METAR = response.substring(startPos + 6);
      Serial.print("Received METAR data: ");
      Serial.println(METAR);
    } else {
      Serial.println("Failed to parse METAR data.");
    }

    client.stop();
  } else {
    Serial.println("Failed to connect to server.");
  }
}
