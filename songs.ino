#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>

#define BUZZER_PIN D5
#define EEPROM_SIZE 1

ESP8266WebServer server(80);

// Function prototypes
void handleRoot();
void handleSave();
void playSong(int songIndex);
void playBangolf();
void playNeverGonnaGiveYouUp();
void playLastChristmas();

void setup() {
  Serial.begin(115200);
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  
  // Initialize the file system
  SPIFFS.begin();

  // Setup WiFi using WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("Wemos_D1_Mini");

  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.begin();

  // Read saved song from EEPROM
  int savedSong = EEPROM.read(0);
  Serial.println("Playing saved song index: " + String(savedSong));
  
  // Play the saved song
  playSong(savedSong);
}

void loop() {
  server.handleClient();
}

// Serve the main web page
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Select a Song</h1>";
  html += "<form action=\"/save\" method=\"POST\">";
  html += "<select name=\"song\">";
  html += "<option value=\"0\">Bangolf</option>";
  html += "<option value=\"1\">Never gonna give you up</option>";
  html += "<option value=\"2\">Last Christmas</option>";
  html += "</select>";
  html += "<input type=\"submit\" value=\"Save\">";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Handle saving the selected song
void handleSave() {
  if (server.hasArg("song")) {
    int songIndex = server.arg("song").toInt();
    EEPROM.write(0, songIndex);
    EEPROM.commit();
    Serial.println("Saved song index: " + String(songIndex));
    server.send(200, "text/plain", "Saved! Rebooting...");
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// Play the selected song
void playSong(int songIndex) {
  switch (songIndex) {
    case 0:
      playBangolf();
      break;
    case 1:
      playNeverGonnaGiveYouUp();
      break;
    case 2:
      playLastChristmas();
      break;
    default:
      Serial.println("Invalid song index");
      break;
  }
}

// Define the songs
void playBangolf() {
  Serial.println("Playing Bangolf");
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
}

void playNeverGonnaGiveYouUp() {
  Serial.println("Playing Never gonna give you up");
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
}

void playLastChristmas() {
  Serial.println("Playing Last Christmas");
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
}
