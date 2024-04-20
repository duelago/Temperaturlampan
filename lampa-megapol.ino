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

const unsigned long REBOOT_INTERVAL = 24 * 60 * 60 * 1000; // 24 hours in milliseconds
unsigned long previousMillis = 0;

const char* apiUrl = "https://listenapi.planetradio.co.uk/api9.2/nowplaying/mme";

// Callback function to handle the JSON object
void handleJsonObject(JsonObject obj) {
    const char* trackTitle = obj["TrackTitle"];
    Serial.print("Track Title: ");
    Serial.println(trackTitle);
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
        color = strip.Color(255, 0, 0);
    } else if (tempLed >= -5 && tempLed < 0) {
        color = strip.Color(0, 0, 255);
    } else if (tempLed >= -50 && tempLed < -5) {
        color = strip.Color(0, 255, 255);
    } else if (tempLed >= 6 && tempLed <= 10) {
        color = strip.Color(255, 153, 0);
    } else if (tempLed >= 11 && tempLed <= 45) {
        color = strip.Color(0, 255, 0);
    }

    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void handleRoot() {
    // Your existing handleRoot function
}

void handleSubmit() {
    // Your existing handleSubmit function
}

void fetchMETARData() {
    // Your existing fetchMETARData function
}

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(2000);
    display.clearDisplay();
    Serial.begin(115200);
    MDNS.begin("temperaturlampan");
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

    if (currentMillis - previousMillis >= REBOOT_INTERVAL) {
        // It's time to reboot
        Serial.println("Rebooting ESP8266...");
        delay(100); // Print the message before rebooting
        ESP.restart(); // Reboot the ESP8266
    }

    MDNS.update();
    server.handleClient();

    // Fetch METAR data every 30 minutes
    static unsigned long lastFetchTime = 0;
    const unsigned long fetchInterval = 1800000;  // 30 minutes in milliseconds
    unsigned long currentTime = millis();

    if (currentTime - lastFetchTime >= fetchInterval) {
        lastFetchTime = currentTime;
        fetchMETARData();
    }

    // Check song title every 30 seconds
    static unsigned long lastCheckTime = 0;
    const unsigned long checkInterval = 30000;  // 30 seconds in milliseconds

    if (currentMillis - lastCheckTime >= checkInterval) {
        lastCheckTime = currentMillis;
        
        if (WiFi.status() == WL_CONNECTED) {
            WiFiClientSecure client;
            client.setInsecure(); // Ignore SSL certificate verification

            HTTPClient https;

            Serial.print("Sending HTTP request to: ");
            Serial.println(apiUrl);

            if (https.begin(client, apiUrl)) {
                int httpCode = https.GET();

                if (httpCode > 0) {
                    if (httpCode == HTTP_CODE_OK) {
                        // Read response into a buffer
                        String payload = https.getString();
                        Serial.println("Received JSON data:");
                        Serial.println(payload); // Print raw JSON data

                        // Parse JSON from the buffer
                        StaticJsonDocument<1200> doc;
                        DeserializationError error = deserializeJson(doc, payload);

                        if (!error && doc.is<JsonObject>()) {
                            JsonObject obj = doc.as<JsonObject>();
                            handleJsonObject(obj);
                        } else {
                            Serial.print("deserializeJson() failed: ");
                            Serial.println(error.c_str());
                        }
                    } else {
                        Serial.print("HTTP request failed with error code: ");
                        Serial.println(httpCode);
                    }
                } else {
                    Serial.println("HTTP request failed");
                }

                https.end();
            } else {
                Serial.println("Unable to connect");
            }
        } else {
            Serial.println("WiFi Disconnected. Reconnecting...");
            WiFiManager wifiManager;
            wifiManager.autoConnect("Temperaturlampan");
            delay(5000);
        }
    }

    delay(100); // Delay for a short time to prevent excessive loop iterations
}
