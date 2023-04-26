#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "tp-link-m4";
const char* password = "no01085385309";
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "rc_car/directions";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  randomSeed(micros());
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed to connect to MQTT Broker with error code: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message received:");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed to connect to MQTT Broker with error code: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
