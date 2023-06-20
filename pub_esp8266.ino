#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

//replace with your network credentials
#define WIFI_SSID "testtt"
#define WIFI_PASSWORD "testtest"

// Raspberry Pi Mosquitto MQTT Broker
#define MQTT_HOST "broker.emqx.io" //IPAddress(192, 168, 1, XYZ)
#define MQTT_PORT 1883

//MQTT Topic
#define MQTT_PUB_Output "esp8266/OutputControl"

const int PushButton = 0;
int Push_button_state;
bool Push_button_Prv_state = false;

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;


void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach();
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(PushButton, INPUT);

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void loop() {
  Push_button_state = digitalRead(PushButton);
  if (Push_button_state == HIGH && Push_button_Prv_state == false)
  {
    // Publish an MQTT message on topic esp8266/OutputControl
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_Output, 1, true, "true");
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_Output, packetIdPub1);
    Serial.println(" Message: ON");
    Push_button_Prv_state = true;
  }
  else if (Push_button_state == LOW && Push_button_Prv_state == true)
  {
    // Publish an MQTT message on topic esp8266/OutputControl
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_Output, 1, true, "false");
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_Output, packetIdPub2);
    Serial.println(" Message: OFF");
    Push_button_Prv_state = false;
  }
  delay(1000);
}
