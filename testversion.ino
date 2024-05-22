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

void songChristmas(int buzzerPin);
void songGolf(int buzzerPin);
void songRickroll(int buzzerPin);
void playSelectedSong(int buzzerPin);


// Function to play the selected song based on EEPROM stored value
void playSelectedSong(int buzzerPin) {
    int songChoice = EEPROM.read(5); // Assuming 5th position is used to store song choice
    switch (songChoice) {
        case 1:
            songChristmas(buzzerPin);
            break;
        case 2:
            songGolf(buzzerPin);
            break;
        case 3:
            songRickroll(buzzerPin);
            break;
        default:
            songChristmas(buzzerPin); // Default to Christmas if invalid choice
            break;
    }
}

void songChristmas(int buzzerPin) {
  
  Serial.println("Starting song function");
  
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


void songGolf(int buzzerPin) {

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

void songRickroll(int buzzerPin) {

 tone(buzzerPin, 208);
  delay(77);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(58);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(79);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(52);
  noTone(buzzerPin);

  tone(buzzerPin, 349);
  delay(285);
  noTone(buzzerPin);

  tone(buzzerPin, 349);
  delay(277);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(439);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(74);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(52);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(82);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(52);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(312);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(280);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(299);
  noTone(buzzerPin);

  tone(buzzerPin, 262);
  delay(77);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(217);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(66);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(55);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(77);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(49);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(419);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(112);
  noTone(buzzerPin);

  tone(buzzerPin, 262);
  delay(334);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(60);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(414);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(121);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(236);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(195);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(482);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(69);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(58);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(82);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(52);
  noTone(buzzerPin);

  tone(buzzerPin, 349);
  delay(439);
  noTone(buzzerPin);

  tone(buzzerPin, 349);
  delay(178);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(491);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(69);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(49);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(85);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(25);
  noTone(buzzerPin);

  tone(buzzerPin, 415);
  delay(455);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(88);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(376);
  noTone(buzzerPin);

  tone(buzzerPin, 262);
  delay(66);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(214);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(66);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(41);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(96);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(41);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(452);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(93);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(112);
  noTone(buzzerPin);

  tone(buzzerPin, 262);
  delay(359);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(58);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(425);
  noTone(buzzerPin);

  tone(buzzerPin, 208);
  delay(110);
  noTone(buzzerPin);

  tone(buzzerPin, 311);
  delay(447);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(576);
  noTone(buzzerPin);
    
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
            playSelectedSong(14); // Play the selected song on pin 14
        }

   
       
    } else {
        Serial.println("Different song is playing. Keeping blinkLED flag false.");
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
        color = strip.Color(100, 255, 0); // Orange 
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
    html += "<div class='container'><h1>Airport code (METAR) and Song Title:</h1>";
    html += "<div class='input-group'><form method='post' action='/submit'>";
    html += "<input type='text' name='stationCode' placeholder='METAR Station Code' />";
    html += "<input type='text' name='songTitle' placeholder='Song Title' />";
    html += "<select name='buzzerSound'><option value='1'>Last Christmas</option><option value='2'>SM i Bangolf 2012 i Kalmar</option><option value='3'>Never Gonna Give You Up</option></select>";
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
    if (server.method() == HTTP_POST) {
        String stationCode = server.arg("stationCode");
        String songTitle = server.arg("songTitle");
        int buzzerSound = server.arg("buzzerSound").toInt();

        if (stationCode.length() == 4 && !songTitle.isEmpty()) {
            // Save station code and song title to EEPROM
            for (int i = 0; i < 4; i++) {
                EEPROM.write(i, stationCode[i]);
            }
            EEPROM.write(4, '\0'); // Null terminator for the station code
            for (int i = 0; i < songTitle.length() && i < EEPROM_SIZE - 5; i++) {
                EEPROM.write(5 + i, songTitle[i]);
            }
            EEPROM.write(5 + songTitle.length(), '\0'); // Null terminator for the song title
            EEPROM.write(EEPROM_SIZE - 1, buzzerSound); // Save selected buzzer sound

            EEPROM.commit();

            server.send(200, "text/plain", "Settings saved successfully! Rebooting...");
            delay(1000);
            ESP.restart();
        } else {
            server.send(400, "text/plain", "Invalid input! Please provide a 4-character station code and a song title.");
        }
    }
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
    Serial.begin(115200);

    EEPROM.begin(EEPROM_SIZE);

    // Load station code from EEPROM
    for (int i = 0; i < 4; i++) {
        METARStation[i] = EEPROM.read(i);
    }
    METARStation[4] = '\0';

    // Load song title from EEPROM
    for (int i = 0; i < EEPROM_SIZE - 5; i++) {
        storedSongTitle[i] = EEPROM.read(5 + i);
    }
    storedSongTitle[EEPROM_SIZE - 5] = '\0';

    // Load selected buzzer sound from EEPROM
    int selectedBuzzerSound = EEPROM.read(EEPROM_SIZE - 1);

    // WiFiManager
    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");

    // Set up LED strip
    strip.begin();
    strip.setBrightness(LED_BRIGHTNESS);
    strip.show();

    // Set up OLED display
    display.begin(SSD1306_I2C_ADDRESS, OLED_RESET);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("System Ready");
    display.display();

    server.on("/", handleRoot);
    server.on("/submit", HTTP_POST, handleSubmit);
    ElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();

    fetchMETARData();
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
