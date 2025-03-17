#include <Wire.h>
#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"

const int DHT_PIN = 15;
const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";
const int myChannelNumber = YOUR CHANNEL NUMBER;
const char* myApiKey = "YOUR WRITE API KEY";
const char* server = "api.thingspeak.com";

DHTesp dhtSensor;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Serial.println("Wifi connected !");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}
void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  ThingSpeak.setField(1, data.temperature);
  ThingSpeak.setField(2, data.humidity);
  int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);
  
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
 if (x == 200) {
    Serial.println("Data pushed successfully");
  } else {
    Serial.println("Push error " + String(x));
  }
  Serial.println("---");
  delay(10000);
}
