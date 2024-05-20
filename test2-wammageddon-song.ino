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
#define EEPROM_SIZE    100   // Size of EEPROM for storing METAR station code (max 4 characters + null terminator)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

char METARStation[5];
String METAR; // Declare METAR as a global variable
String songTitle; // Declare songTitle as a global variable

const unsigned long REBOOT_INTERVAL = 24 * 60 * 60 * 1000; // 24 hours in milliseconds
unsigned long previousMillis = 0;

const char* apiUrl = "https://listenapi.planetradio.co.uk/api9.2/nowplaying/mme";

bool blinkLED = false; // Flag to control LED blinking
bool isBlinking = false; // Flag to track if LED is currently blinking
bool songPlayed = false; // Flag to track if the song has been played

unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500; // Blink interval in milliseconds

void song(int buzzerPin){
  
  tone(buzzerPin, 330);
  delay(266);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(214);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(240);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(230);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(219);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(500);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(250);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(185);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(229);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(230);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(229);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(201);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(258);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(221);
  noTone(buzzerPin);

  tone(buzzerPin, 440);
  delay(229);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(250);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(302);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(380);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(423);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(250);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(250);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(263);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(517);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(500);
  noTone(buzzerPin);

  tone(buzzerPin, 220);
  delay(500);
  noTone(buzzerPin);

  tone(buzzerPin, 220);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(375);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(375);
  noTone(buzzerPin);
}


void handleJsonObject(JsonObject obj) {
    const char* trackTitle = obj["TrackTitle"];
    songTitle = trackTitle;
    Serial.print("Received Track Title: ");
    Serial.println(songTitle);

    String storedSongTitle = readSongTitleFromEEPROM();
    if (songTitle == storedSongTitle) {
        Serial.println("Matched song! Setting blinkLED flag to true.");
        blinkLED = true;
        isBlinking = true; // Set isBlinking to true when starting LED blinking
    } else {
        Serial.println("Song did not match. Keeping blinkLED flag false.");
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
    html += "<div class='container'><h1>Set METAR Station Code and Song Title:</h1>";
    html += "<div class='input-group'><form method='post' action='/submit'>";
    html += "<input type='text' name='stationCode' placeholder='ESSB' />";
    html += "<input type='submit' value='Submit' />";
    html += "</form></div>";
    html += "<div class='input-group'><form method='post' action='/submitSong'>";
    html += "<input type='text' name='songTitle' placeholder='Song Title' />";
    html += "<input type='submit' value='Submit' />";
    html += "</form></div>";
    html += "<div class='metar-info'>Current METAR:<br>";
    html += METAR;
    html += "<p>Song right now on Mix Megapol: ";
    html += songTitle;
    html += "<p><a href='/update'>Firmware update</a></p><br>";
    html += "Version 0.6 Whamageddonlamp<br>";
    html += "</div>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleSubmit() {
    String stationCode = server.arg("stationCode");
    stationCode.toUpperCase();
    stationCode.toCharArray(METARStation, 5);

    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 5; ++i) {
        EEPROM.write(i, METARStation[i]);
    }
    EEPROM.end();

    fetchMETARData();
    server.send(200, "text/html", "Station updated. <a href='/'>Back</a>");
}

void handleSubmitSong() {
    String newSongTitle = server.arg("songTitle");
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < newSongTitle.length() && i < EEPROM_SIZE - 5; ++i) {
        EEPROM.write(5 + i, newSongTitle[i]);
    }
    EEPROM.write(5 + newSongTitle.length(), '\0'); // Null-terminate the stored string
    EEPROM.end();

    server.send(200, "text/html", "Song title updated. <a href='/'>Back</a>");
}

String readSongTitleFromEEPROM() {
    String storedSongTitle = "";
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 5; i < EEPROM_SIZE; ++i) {
        char c = EEPROM.read(i);
        if (c == '\0') break;
        storedSongTitle += c;
    }
    EEPROM.end();
    return storedSongTitle;
}

void fetchMETARData() {
    // Fetch METAR data using METARStation variable (e.g., "ESSA" or user-defined value)
    WiFiClientSecure client;
    client.setInsecure(); // Ignore SSL certificate verification

    HTTPClient https;
    String url = "https://tgftp.nws.noaa.gov/data/observations/metar/decoded/" + String(METARStation) + ".TXT";
    https.begin(client, url); // Use ::begin(WiFiClient, url) syntax

    int httpCode = https.GET();

    if (httpCode == HTTP_CODE_OK) {
        METAR = https.getString();
        https.end();

        // For testing purposes, print METAR data
        Serial.println("Fetching METAR data...");
        Serial.println(METAR);

        // Parse the temperature from the METAR string
        String temperature = parseTemperature(METAR);
        float tempLed = temperature.toFloat();

        // Print the temperature to the serial port
        Serial.print("The temperature is ");
        Serial.print(tempLed);
        Serial.println(" degrees Celsius.");

        display.cp437(true);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(25, 15);
        display.print(int(tempLed));

        display.display();

        // Set LED color based on temperature
        setLEDColor(tempLed);
    } else {
        Serial.println("Failed to fetch METAR data");
        https.end();
    }
}


void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(2000);
    display.clearDisplay();
    Serial.begin(115200);
    MDNS.begin("whamageddonlamp");
    ElegantOTA.begin(&server);

    strip.begin();
    strip.show();  // Initialize all pixels to 'off'

    WiFiManager wifiManager;
    wifiManager.autoConnect("Whamageddonlamp");

    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 5; ++i) {
        METARStation[i] = EEPROM.read(i);
    }
    EEPROM.end();
    METARStation[4] = '\0';

    songTitle = readSongTitleFromEEPROM();

    server.on("/", handleRoot);
    server.on("/submit", HTTP_POST, handleSubmit);
    server.on("/submitSong", HTTP_POST, handleSubmitSong);

    server.on("/update", []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", "<meta http-equiv='refresh' content='5;url=/'><body><h1>Updating...</h1></body>");
        ESP.restart();
    });

    server.begin();

    fetchMETARData();

    Serial.println("Setup complete.");
}


void loop() {
    server.handleClient();
  

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= REBOOT_INTERVAL) {
        Serial.println("Rebooting device to prevent memory leaks.");
        ESP.restart();
    }

    if (blinkLED && !isBlinking) {
        isBlinking = true;
        lastBlinkTime = millis();
        Serial.println("Starting LED blinking...");
    }

    if (isBlinking && blinkLED) {
        if (millis() - lastBlinkTime >= blinkInterval) {
            uint32_t color = strip.getPixelColor(0);

            if (color == 0) {
                strip.setPixelColor(0, 255, 0, 0);
                strip.setBrightness(LED_BRIGHTNESS);
                strip.show();
            } else {
                strip.setPixelColor(0, 0, 0, 0);
                strip.show();
            }
            lastBlinkTime = millis();
        }
    } else {
        if (isBlinking) {
            isBlinking = false;
            strip.setPixelColor(0, 0, 0, 0);
            strip.show();
            Serial.println("Stopping LED blinking...");
        }
    }

    MDNS.update();
}
