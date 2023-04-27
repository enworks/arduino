#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "tp-link-m4";
const char* password = "no01085385309";
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic = "rc_car/directions";
const char* mqtt_client_id = "enworks0001";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  delay(10);
  client.loop();
}


