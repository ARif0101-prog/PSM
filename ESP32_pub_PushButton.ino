#include <WiFi.h>
#include "PubSubClient.h"
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";
int port = 1883;

char clientId[50];
WiFiClient WIFI_CLIENT;
PubSubClient MQTT_CLIENT;

const int BUTTON = 4;
int buttonState = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(BUTTON, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  }
  Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}

void reconnect() {
// Set our MQTT broker address and port
MQTT_CLIENT.setServer(mqttServer, port);
MQTT_CLIENT.setClient(WIFI_CLIENT);

// Loop until we're reconnected
while (!MQTT_CLIENT.connected()) {
// Attempt to connect
Serial.println("Attempt to connect to MQTT broker");
MQTT_CLIENT.connect("test123>");
// Wait some time to space out connection requests
delay(3000);
}
Serial.println("MQTT connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000); // this speeds up the simulation
  if (!MQTT_CLIENT.connected()) {
// If we're not, attempt to reconnect
reconnect();
}
buttonState = digitalRead(BUTTON);
if (buttonState == LOW){
  MQTT_CLIENT.publish("/Lantai 1/lampu/TMB/22", "on");
}
else
{
  MQTT_CLIENT.publish("/Lantai 1/lampu/TMB/22", "off");
}
}
