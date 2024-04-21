// Exempelfil på hur man hämtar låttitel från Mix Megapol


#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "Alter_3G";
const char* password = "xxx";

const char* apiUrl = "https://listenapi.planetradio.co.uk/api9.2/nowplaying/mme";

// Callback function to handle the JSON object
void handleJsonObject(JsonObject obj) {
  const char* trackTitle = obj["TrackTitle"];
  Serial.print("Track Title: ");
  Serial.println(trackTitle);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
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
    WiFi.begin(ssid, password);
    delay(5000);
  }

  delay(30000); // Wait 30 seconds before making the next request
}
