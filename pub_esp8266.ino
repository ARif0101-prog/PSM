#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int Push_button_state;
bool Push_button_Prv_state = false;
const int PushButton = 0;
const char *WIFI_SSID = "testtt";
const char *WIFI_PASSWORD = "testtest";

const char *MQTT_HOST = "broker.emqx.io";
const int MQTT_PORT = 1883;
const char *MQTT_CLIENT_ID = "ESP8266 NodeMCU";
const char *MQTT_USER = "";
const char *MQTT_PASSWORD = "";
const char *TOPIC = "nodemcu/test";

WiFiClient client;
PubSubClient mqttClient(client);

void setup()
{
    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    while (!client.connected()) {
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
            Serial.println("Connected to MQTT broker");
        } else {
            delay(500);
            Serial.print(".");
        }
    }
}

void loop()
{
  Push_button_state = digitalRead(PushButton);
 if (Push_button_state == HIGH && Push_button_Prv_state == false)
  {
    mqttClient.publish(TOPIC,String("true").c_str());
    // // Publish an MQTT message on topic esp8266/OutputControl
    //  uint16_t packetIdPub1 = mqttClient.publish(TOPIC,("true").c_str());//uint16_t packetIdPub1 = mqttClient.publish(TOPIC, 1,"true");
    //  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", TOPIC;, packetIdPub1);
    //  Serial.println(" Message: ON");
    Push_button_Prv_state = true;
  }
  else if (Push_button_state == LOW && Push_button_Prv_state == true)
  {
    mqttClient.publish(TOPIC,String("false").c_str());
    //mqttClient.publish(TOPIC,"false");
    // Publish an MQTT message on topic esp8266/OutputControl
    // uint16_t packetIdPub2 = mqttClient.publish(TOPIC,("false").c_str());// uint16_t packetIdPub2 = mqttClient.publish(TOPIC, 1,"false");
    //  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", TOPIC, packetIdPub2);
    //  Serial.println(" Message: OFF");
    Push_button_Prv_state = false;
  }

    // int value = random(1, 101);
    // mqttClient.publish(TOPIC, String(value).c_str());

    // delay(1000);
}
