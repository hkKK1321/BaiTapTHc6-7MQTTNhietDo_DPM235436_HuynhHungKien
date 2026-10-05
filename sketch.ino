#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topic_k = "agu/iot/nhietdo";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[Board 1] WiFi da ket noi!");
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32_Board_ReadTemp_";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("[Board 1] Da ket noi HiveMQ!");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    // Chuyển nhiệt độ sang chuỗi String
    String tempStr = String(temp, 2);
    
    // c_str() chuyển String thành const char* chuẩn cho MQTT publish
    client.publish(topic_k, tempStr.c_str());
    
    Serial.print("[Board 1 - DHT22] Publish nhiet do: ");
    Serial.println(tempStr);
  }

  delay(5000);
}