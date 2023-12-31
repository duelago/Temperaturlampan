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
  String html = "<html><head><style>";
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
html += "<p>ESGG G&ouml;TEBORG/Landvetter<br>";
html += "ESOK KARLSTAD<br>";
html += "ESNQ KIRUNA<br>";
html += "ESMS MALM&ouml;<br>";
html += "ESSA STOCKHOLM/Arlanda<br>";
html += "ESSB STOCKHOLM/Bromma<br>";
html += "ESKN STOCKHOLM/Skavsta<br>";
html += "ESOW STOCKHOLM/V&auml;ster&aring;s<br>";
html += "<p>";
html += "ESNX Arvidsjaur<br>";
html += "ESSD Borl&auml;nge<br>";
html += "ESNG G&auml;llivare<br>";
html += "ESMT Halmstad (F14)<br>";
html += "ESUT Hemavan<br>";
html += "ESGJ J&ouml;nk&ouml;ping<br>";
html += "ESMQ Kalmar (F12)<br>";
html += "ESNK Kramfors/Sollefte&aring;<br>";
html += "ESMK Kristianstad/Ever&ouml;d<br>";
html += "ESCF Link&ouml;ping/Malmen (F3)<br>";
html += "ESTL Ljungbyhed (F5)<br>";
html += "ESPA Lule&aring; (F21)<br>";
html += "ESNL Lycksele<br>";
html += "ESSP Norrk&ouml;ping/Kungs&auml;ngen<br>";
html += "ESUP Pajala<br>";
html += "ESDF Ronneby (F17)<br>";
html += "ESNS Skellefte&aring;<br>";
html += "ESGR Sk&ouml;vde<br>";
html += "ESNN Sundsvall-Timr&aring;<br>";
html += "ESIB S&aring;ten&auml;s (F7)<br>";
html += "ESKS S&auml;len<br>";
html += "ESGT Trollh&auml;ttan-V&auml;nersborg<br>";
html += "ESNU Ume&aring;<br>";
html += "ESNV Vilhelmina<br>";
html += "ESSV Visby<br>";
html += "ESMX V&auml;xj&ouml;/Kronoberg<br>";
html += "ESTA &Auml;ngelholm/Bark&aring;kra (F10)<br>";
html += "ESOE &Ouml;rebro<br>";
html += "ESNO &Ouml;rnsk&ouml;ldsvik/Gide&aring;<br>";
html += "ESNZ &Ouml;stersund/Fr&ouml;s&ouml;n (F4)";
html += "</div>";
html += "<div class='metar-info'>Senaste METAR:<br>";
html += METAR;
html += "<p><a href='/update'>Firmwareuppdatering</a></p><br>";
html += "Version 0.9 Temperaturlampan<br>";
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

  server.send(200, "text/html", "Stationen uppdaterad. <a href='/'>Tillbaka</a>");
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

void loop() {
  
{
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
  const unsigned long fetchInterval = 900000;  // 15 minutes in milliseconds
  unsigned long currentTime = millis();

  if (currentTime - lastFetchTime >= fetchInterval) {
    lastFetchTime = currentTime;
    fetchMETARData();
  }

  // Delay for a short time to prevent excessive loop iterations
   previousMillis = currentMillis;
  delay(100);
  
}}
