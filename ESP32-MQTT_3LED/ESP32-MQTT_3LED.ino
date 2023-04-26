#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your WiFi SSID/Password combination
const char* ssid = "tp-link-m4";
const char* password = "no01085385309";

// Replace the next variable with your MQTT broker address (e.g., "test.mosquitto.org")
const char* mqtt_server = "test.mosquitto.org";

// Replace the next variable with the MQTT topic you want to subscribe to (e.g., "rc_car/directions")
const char* mqtt_topic = "rc_car/directions";

// Replace the next variable with your ESP32 ID (e.g., "enworks0001")
const char* mqtt_client_id = "enworks0001";

// Replace the next variables with the GPIO pins connected to the LEDs
const int led1_pin = 21;
const int led2_pin = 22;
const int led3_pin = 23;
const int led_builtin_pin = 2;  

// Create a WiFiClient object to handle the connection to the MQTT broker
WiFiClient espClient;
// Create a PubSubClient object to handle the MQTT communication
PubSubClient client(espClient);

// This function is called whenever a message is received on the subscribed topic
void callback(char* topic, byte* message, unsigned int length) {
  // Print the received message
  delay(1000);
  Serial.print("Message arrived on topic [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(message, length);
  Serial.println();

  // Convert the message from byte array to integer
  int msg = atoi((const char*)message);

  // Switch on the message value and turn on/off the corresponding LED
  switch(msg) {
    case 1:
      digitalWrite(led1_pin, HIGH);
      digitalWrite(led2_pin, LOW);
      digitalWrite(led3_pin, LOW);
      break;
    case 2:
      digitalWrite(led1_pin, LOW);
      digitalWrite(led2_pin, HIGH);
      digitalWrite(led3_pin, LOW);
      break;
    case 3:
      digitalWrite(led1_pin, LOW);
      digitalWrite(led2_pin, LOW);
      digitalWrite(led3_pin, HIGH);
      break;
    default:
      digitalWrite(led1_pin, LOW);
      digitalWrite(led2_pin, LOW);
      digitalWrite(led3_pin, LOW);
  }
}

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Connect to the WiFi network
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  
  // Set up the LED pins as outputs
  pinMode(led1_pin, OUTPUT);
  pinMode(led2_pin, OUTPUT);
  pinMode(led3_pin, OUTPUT);
  pinMode(led_builtin_pin, OUTPUT); 

  // Set up the MQTT client
  client.setServer(mqtt_server, 1883);
  Serial.println("callback!!!!");
  client.setCallback(callback);

  // Connect to the MQTT broker
  Serial.print("Connecting to MQTT broker...");
  while (!client.connected()) {
    if (client.connect(mqtt_client_id)) {
      Serial.println();
      Serial.println("MQTT client connected");
      // Subscribe to the MQTT topic
      if (client.subscribe(mqtt_topic)) {
          Serial.println("Subscribed to topic");
 //         client.setCallback(callback);
          } else {
          Serial.println("Failed to subscribe to topic");
      }

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  delay(10);
  // Handle the MQTT communication
  client.loop();

  static unsigned long last_blink_time = 0;
  if (millis() - last_blink_time >= 2000) {
    last_blink_time = millis();
    digitalWrite(led_builtin_pin, !digitalRead(led_builtin_pin));  // 내장 LED를 반전시킴
  }
}
