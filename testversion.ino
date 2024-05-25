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
#define EEPROM_SIZE    100

#define OLED_RESET 0

Adafruit_SSD1306 display(OLED_RESET);

#define PIN            D4  // Define the pin for WS2811 LEDs
#define NUM_LEDS       1   // Define the number of LEDs
#define LED_BRIGHTNESS 255  // Define LED brightness (0-255)
#define EEPROM_SIZE    100   // Size of EEPROM for storing METAR station code (max 4 characters + null terminator)
#define BUZZER_PIN D5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

char METARStation[5];
char storedSongTitle[EEPROM_SIZE - 5];
String METAR; // Declare METAR as a global variable
String songTitle; // Declare songTitle as a global variable
String currentSongTitle; // Declare currentSongTitle as a global variable


const unsigned long REBOOT_INTERVAL = 24 * 60 * 60 * 1000; // 24 hours in milliseconds
unsigned long previousMillis = 0;

// Add a variable to store the time the song was last played
unsigned long songPlayedTime = 0;
const unsigned long ONE_HOUR = 60 * 60 * 1000; // One hour in milliseconds

const char* apiUrl = "https://listenapi.planetradio.co.uk/api9.2/nowplaying/mme";

bool blinkLED = false; // Flag to control LED blinking
bool isBlinking = false; // Flag to track if LED is currently blinking
bool songPlayed = false; // Flag to track if the song has been played

unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500; // Blink interval in milliseconds

const unsigned long METAR_FETCH_INTERVAL = 30 * 60 * 1000; // 30 minutes in milliseconds
unsigned long lastMETARFetchTime = 0;

void song1(int buzzerPin) {
    // Song 1 notes
    tone(BUZZER_PIN, 330);
  delay(266);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(214);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(240);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(230);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(219);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(500);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 294);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(250);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(185);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(229);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(230);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(229);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(201);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(258);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(221);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 440);
  delay(229);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(250);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(302);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(380);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(423);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(250);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(250);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(263);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(517);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(500);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 220);
  delay(500);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 220);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(375);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(375);
  noTone(BUZZER_PIN);
    // Add remaining tones for Song 1
}

void song2(int buzzerPin) {
    // Song 2 notes
tone(BUZZER_PIN, 208);
  delay(77);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(58);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(79);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(52);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 349);
  delay(285);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 349);
  delay(277);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(439);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(74);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(52);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(82);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(52);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(312);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(280);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(299);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 262);
  delay(77);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(217);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(66);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(55);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(77);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(49);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(419);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(112);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 262);
  delay(334);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(60);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(414);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(121);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(236);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(195);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(482);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(69);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(58);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(82);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(52);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 349);
  delay(439);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 349);
  delay(178);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(491);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(69);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(49);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(85);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(25);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 415);
  delay(455);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(88);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(376);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 262);
  delay(66);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(214);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(66);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(41);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(96);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(41);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(452);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(93);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(112);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 262);
  delay(359);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 233);
  delay(58);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(425);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 208);
  delay(110);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 311);
  delay(447);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(576);
  noTone(BUZZER_PIN);
    // Add remaining tones for Song 2
}

void song3(int buzzerPin) {
    // Song 3 notes
tone(BUZZER_PIN, 330);
  delay(602);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(365);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 294);
  delay(434);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 220);
  delay(255);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(203);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(203);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(226);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 294);
  delay(781);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(185);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(226);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(243);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 330);
  delay(249);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 370);
  delay(336);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 294);
  delay(625);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 294);
  delay(203);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(237);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 294);
  delay(220);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 277);
  delay(214);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 247);
  delay(1169);
  noTone(BUZZER_PIN);
    // Add remaining tones for Song 3
}

void playSelectedSong(int buzzerPin, int songChoice) {
    switch (songChoice) {
        case 0:
            song1(buzzerPin);
            break;
        case 1:
            song2(buzzerPin);
            break;
        case 2:
            song3(buzzerPin);
            break;
    }
}

void handleJsonObject(JsonObject obj, unsigned long currentMillis) {
    const char* trackTitle = obj["TrackTitle"];
    currentSongTitle = trackTitle;
    Serial.print("Received Track Title: ");
    Serial.println(currentSongTitle);

    if (currentSongTitle == storedSongTitle) {
        Serial.println("Stored song is playing!! Setting blinkLED flag to true.");
        blinkLED = true;
        isBlinking = true; // Set isBlinking to true when starting LED blinking

        if (!songPlayed || (currentMillis - songPlayedTime >= ONE_HOUR)) {
            songPlayed = true;
            songPlayedTime = currentMillis;
            int songChoice = EEPROM.read(EEPROM_SIZE - 1);  // Read the stored buzzer sound choice
            playSelectedSong(BUZZER_PIN, songChoice);  // Play the selected song
        }
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
    html += ".input-group select { width: 80%; padding: 8px; font-size: 16px; border: 1px solid #ccc; border-radius: 5px; }";
    html += ".input-group input[type='submit'] { width: 18%; padding: 8px; font-size: 16px; border: none; background-color: #4caf50; color: #ffffff; cursor: pointer; border-radius: 5px; }";
    html += ".metar-info { font-size: 18px; margin-top: 20px; }";
    html += ".update-link { text-decoration: none; color: #4caf50; font-weight: bold; margin-top: 20px; display: block; }";
    html += "</style></head><body>";
    html += "<div class='container'><h1>Airport code (METAR) and Song Title:</h1>";
    html += "<div class='input-group'><form method='post' action='/submit'>";
    html += "<input type='text' name='stationCode' placeholder='METAR Station Code' />";
    html += "<input type='text' name='songTitle' placeholder='Song Title' />";
    html += "<div class='input-group'><label for='buzzerSound'>Select Buzzer Sound:</label>";
    html += "<select name='buzzerSound' id='buzzerSound'>";
    html += "<option value='0'>SM i Bangolf 2012 i Kalmar</option>";
    html += "<option value='1'>Never gonna give you up</option>";
    html += "<option value='2'>Last Christmas</option>";
    html += "</select></div>";
    html += "<input type='submit' value='Submit' />";
    html += "</form>";
    html += "<p>";
    html += "<p>";
    html += "ESNX Arvidsjaur<br>";
    html += "ESSD Borlänge<br>";
    html += "ESNG Gällivare<br>";
    html += "ESGG Göteborg/Landvetter<br>";
    html += "ESMT Halmstad (F14)<br>";
    html += "ESUT Hemavan<br>";
    html += "ESGJ Jönköping<br>";
    html += "ESMQ Kalmar (F12)<br>";
    html += "ESOK Karlstad<br>";
    html += "ESNQ Kiruna<br>";
    html += "ESNK Kramfors/Sollefteå<br>";
    html += "ESMK Kristianstad/Everöd<br>";
    html += "ESCF Linköping/Malmen (F3)<br>";
    html += "ESTL Ljungbyhed (F5)<br>";
    html += "ESPA Luleå (F21)<br>";
    html += "ESNL Lycksele<br>";
    html += "ESMS Malmö<br>";
    html += "ESSP Norrköping/Kungsängen<br>";
    html += "ESUP Pajala<br>";
    html += "ESDF Ronneby (F17)<br>";
    html += "ESNS Skellefteå<br>";
    html += "ESGR Skövde<br>";
    html += "ESSA Stockholm/Arlanda<br>";
    html += "ESSB Stockholm/Bromma<br>";
    html += "ESKN Stockholm/Skavsta<br>";
    html += "ESOW Stockholm/Västerås<br>";
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
    html += "<div class='metar-info'>Current METAR:<br>";
    html += METAR;
    html += "<p>Song right now on Mix Megapol: ";
    html += currentSongTitle;
    html += "<p>Stored song title: ";
    html += storedSongTitle;
    html += "<p><a href='/update'>Firmware update</a></p><br>";
    html += "Version 0.7 Whamageddonlamp<br>";
    html += "</div>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}



void handleSubmit() {
    String stationCode = server.arg("stationCode");
    String songTitle = server.arg("songTitle");
    int buzzerSound = server.arg("buzzerSound").toInt();

    // Convert the input station code to uppercase
    stationCode.toUpperCase();

    // Copy the uppercase station code to METARStation variable
    stationCode.toCharArray(METARStation, 5);

    // Copy the song title to storedSongTitle variable
    songTitle.toCharArray(storedSongTitle, EEPROM_SIZE - 6);

    // Store the buzzer sound choice
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 5; ++i) {
        EEPROM.write(i, METARStation[i]);
    }
    for (int i = 0; i < EEPROM_SIZE - 6; ++i) {
        EEPROM.write(i + 5, storedSongTitle[i]);
    }
    EEPROM.write(EEPROM_SIZE - 1, buzzerSound);
    EEPROM.end();

    // Fetch METAR data immediately after station code submission
    fetchMETARData();

    server.send(200, "text/html", "Settings updated. <a href='/'>Back</a>");
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

    // Initialize NeoPixel strip
    strip.begin();
    strip.show();  // Initialize all pixels to 'off'

    // Use WiFiManager to set WiFi credentials if they are not already configured
    WiFiManager wifiManager;
    wifiManager.autoConnect("Whamageddonlamp");

    // Load METAR station code and song title from EEPROM
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 5; ++i) {
        METARStation[i] = EEPROM.read(i);
    }
    for (int i = 0; i < EEPROM_SIZE - 5; ++i) {
        storedSongTitle[i] = EEPROM.read(i + 5);
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

                handleJsonObject(doc.as<JsonObject>(), currentMillis);

            } else {
                Serial.printf("[HTTP] GET request failed, error: %s\n", https.errorToString(httpCode).c_str());
            }
        } else {
            Serial.println("WiFi Disconnected. Skipping song check.");
        }
    }

    // Fetch METAR data every 30 minutes
    if (currentMillis - lastMETARFetchTime >= METAR_FETCH_INTERVAL) {
        lastMETARFetchTime = currentMillis;
        fetchMETARData();
    }

    // Handle LED blinking if the flag is set
    if (blinkLED) {
        unsigned long currentBlinkTime = millis();
        if (currentBlinkTime - lastBlinkTime >= blinkInterval) {
            lastBlinkTime = currentBlinkTime;

            // Toggle LED state
            if (isBlinking) {
                strip.setPixelColor(0, strip.Color(255, 0, 0));  // Set LED to red
                strip.show();
            } else {
                strip.setPixelColor(0, strip.Color(0, 255, 0));  // Green
                strip.show();
            }

            isBlinking = !isBlinking;  // Toggle the isBlinking state
        }
    }
    
 

    server.handleClient();

    // Check if it's time to reboot the ESP8266
    if (currentMillis - previousMillis >= REBOOT_INTERVAL) {
        previousMillis = currentMillis;
        ESP.restart();
    }
}
