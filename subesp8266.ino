#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const int LEDPIN = 2; 
const char *WIFI_SSID = "testtt";
const char *WIFI_PASSWORD = "testtest";

const char *MQTT_HOST = "broker.emqx.io";//"192.168.0.184";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266 NodeMCU";
const char *MQTT_USER = "";
const char *MQTT_PASSWORD = "";
const char *TOPIC = "nodemcu/test";

WiFiClient espClient;
//PubSubClient mqttClient(client);

PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    //int value = String((char*) payload).toInt();

    Serial.println(topic);
    Serial.println(String((char*) payload));

      String messageTemp;
      for (int i = 0; i < length; i++) {
        messageTemp += (char)payload[i];
      }
        Serial.print("Message: ");
        Serial.println(messageTemp);

      if (messageTemp == "ON"){
      digitalWrite(LEDPIN, HIGH); 
      Serial.println("LED is now ON!");
      }

      else{
      digitalWrite(LEDPIN, LOW); 
      Serial.println("LED is now OFF");
      }
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);
    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");

    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(callback);

    while (!client.connected()) {
        if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
            Serial.println("Connected to MQTT broker");
        } else {
            delay(500);
            Serial.print(".");
        }
    }

    client.subscribe(TOPIC);
}

void loop()
{
    client.loop();
}
