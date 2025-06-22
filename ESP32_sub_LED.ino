#include <WiFi.h>
#include "PubSubClient.h"
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";
int port = 1883;

char clientId[50];
WiFiClient WIFI_CLIENT;
PubSubClient MQTT_CLIENT;

const int LED = 4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(LED, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  MQTT_CLIENT.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length){
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

      if (messageTemp == "on"){
      digitalWrite(LED, HIGH); 
      Serial.println("LED is now ON!");
      }

      else{
      digitalWrite(LED, LOW); 
      Serial.println("LED is now OFF");
      }
}

void reconnect() {
  MQTT_CLIENT.setServer(mqttServer, port);
  MQTT_CLIENT.setClient(WIFI_CLIENT);
  // Loop until we're reconnected
  while (!MQTT_CLIENT.connected()) 
  {
    Serial.println("Attempt to connect to MQTT broker");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println(clientId);
    if (MQTT_CLIENT.connect(clientId.c_str()))
    {
    Serial.println("connected");
    //once connected to MQTT broker, subscribe command if any
    MQTT_CLIENT.subscribe("/Lantai 1/saklar/TMB/22");
  } 
  else 
  {
    Serial.print("failed, rc=");
    Serial.print(MQTT_CLIENT.state());
    Serial.println(" try again in 5 seconds");
    // Wait 6 seconds before retrying
    delay(6000);
  }
  // Wait some time to space out connection requests
  }
  Serial.println("MQTT connected");
}

void loop() {
  // put your main code here, to run repeatedly:
 
  if (!MQTT_CLIENT.connected()) {
  reconnect();
  }
  MQTT_CLIENT.loop();
}
