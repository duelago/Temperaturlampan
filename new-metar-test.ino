#include <ESP8266WiFi.h>  // For ESP8266
//#include <WiFi.h>      // For ESP32
#include <WiFiClientSecure.h>

// Replace these with your network credentials
const char* ssid = "SSID";
const char* password = "passwd";

// METAR data URL
const char* host = "aviationweather.gov";
const int httpsPort = 443;
const char* url = "/api/data/metar?ids=ESNZ";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");

  WiFiClientSecure client;
  client.setInsecure(); // Ignore SSL certificate verification

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed!");
    return;
  }

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String response = client.readString();
  Serial.println("Response: " + response);

  // Parse the METAR data
  int startIndex = response.indexOf("ESNZ");
  if (startIndex != -1) {
    String metarData = response.substring(startIndex, response.indexOf("\n", startIndex));
    Serial.println("METAR Data: " + metarData);
    int temperature = extractTemperature(metarData);
    if (temperature != INT_MIN) {
      Serial.println("Temperature: " + String(temperature) + " degrees");
    } else {
      Serial.println("Temperature not found");
    }
  } else {
    Serial.println("METAR data not found");
  }
}

int extractTemperature(String metar) {
  // Find the part of the string that contains the temperature and dew point
  char str[130];
  metar.toCharArray(str, 130);
  char *parameter = strtok(str, " ");
  while (parameter != NULL) {
    String temp_str = String(parameter);
    if (temp_str.indexOf('/') > 0) {
      String tempPart = temp_str.substring(0, temp_str.indexOf('/'));
      if (tempPart.startsWith("M")) {
        tempPart.replace('M', '-');
      }
      int temperature = tempPart.toInt();
      return temperature;
    }
    parameter = strtok(NULL, " ");
  }
  return INT_MIN; // Temperature not found
}

void loop() {
  // Do nothing
}
