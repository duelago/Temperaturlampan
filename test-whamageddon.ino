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
bool songTitleFlag = false; // Flag to track if song title flag is set

const unsigned long REBOOT_INTERVAL = 24 * 60 * 60 * 1000; // 24 hours in milliseconds
unsigned long previousMillis = 0;

const char* apiUrl = "https://listenapi.planetradio.co.uk/api9.2/nowplaying/mme";
const int GPIO14 = D5;
bool blinkLED = false; // Flag to control LED blinking
bool isBlinking = false; // Flag to track if LED is currently blinking

unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500; // Blink interval in milliseconds

// D5 control variables
bool D5High = false;
unsigned long D5StartTime = 0;
const unsigned long D5Duration = 300; // 0.3 seconds duration for D5 high

// Callback function to handle the JSON object
void handleJsonObject(JsonObject obj) {
    const char* trackTitle = obj["TrackTitle"];
    songTitle = trackTitle;
    Serial.print("Received Track Title: ");
    Serial.println(songTitle);

    // If the song title is "Last Christmas", start blinking the LED and set D5 flag
    if (songTitle == "Unforgettable") {
        Serial.println("Whamageddon!! Setting blinkLED flag to true.");
        blinkLED = true;
        isBlinking = true; // Set isBlinking to true when starting LED blinking
        songTitleFlag = true; // Set songTitleFlag to true
        // Pull D5 high for 0.3 seconds
        digitalWrite(D5, HIGH);
        D5High = true;
        D5StartTime = millis(); // Record the start time
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
    html += "ESPA Luleå (F21)<br>";
    html += "ESNL Lycksele<br>";
    html += "ESSP Norrköping/Kungsängen<br>";
    html += "ESUP Pajala<br>";
    html += "ESDF Ronneby (F17)<br>";
    html += "ESNS Skellefteå<br>";
    html += "ESGR Skövde<br>";
    html += "ESNN Sundsvall-Timrå<br>";
    html += "ESIB Såtenäs (F7)<br>";
    html += "ESKS Sälen<br>";
    html += "ESGT Trollhättan-Vänersborg<br>";
    html += "ESNU Umeå<br>";
    html += "ESNV Vilhelmina<br>";
    html += "ESSV Visby<br>";
    html += "ESMX Växjö/Kronoberg<br>";
    html += "ESTA Ängelholm/Barkåkra (F10)<br>";
    html += "ESOE Örebro<br>";
    html += "ESNO Örnsköldsvik/Gideå<br>";
    html += "ESNZ Östersund/Frösön (F4)";
    html += "</div>";
    html += "<div class='metar-info'>Senaste METAR:<br>";
    html += METAR;
    html += "<p>Låt just nu: ";
    html += songTitle;
    html += "<p><a href='/update'>Firmwareuppdatering</a></p><br>";
    html += "Version 0.5 Whamageddonlampan<br>";
    html += "</div>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleSubmit() {
    String stationCode = server.arg("stationCode");

    // Convert the input station code to uppercase
    stationCode.toUpperCase();

    // Copy the uppercase station code to METARStation variable
    stationCode.toCharArray(METARStation, EEPROM_SIZE);

    // Update EEPROM with the new METAR station code
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < EEPROM_SIZE; ++i) {
        EEPROM.write(i, METARStation[i]);
    }
    EEPROM.end();

    // Fetch METAR data immediately after station code submission
    fetchMETARData();

    server.send(200, "text/html", "Stationen uppdaterad. <a href='/'>Tillbaka</a>");
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
    MDNS.begin("whamageddonlampan");
    ElegantOTA.begin(&server);

    pinMode(D5, OUTPUT); // Set D5 as output

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
            } else {
                strip.setPixelColor(0, strip.Color(0, 255, 0)); // Grön
                strip.show();
            }
        }
    }

    // Handle D5 control
    if (songTitleFlag) {
        // If D5 is not high, set it to high and start the timer
        if (!D5High) {
            digitalWrite(D5, HIGH);
            D5High = true;
            D5StartTime = currentMillis; // Record the start time
        }
        // Check if D5 has been high for the specified duration, if so reset flag
        if (D5High && (currentMillis - D5StartTime >= D5Duration)) {
            digitalWrite(D5, LOW);
            D5High = false;
        }
    }

    // Handle HTTP server requests
    server.handleClient();
}
