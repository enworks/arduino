#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "tp-link-m4";
const char* password = "no01085385309";
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "rc_car/directions";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Wi-Fi 연결 시작
  WiFi.begin(ssid, password);

  Serial.print("Wi-Fi 연결 시도 중...");

  // Wi-Fi 연결 완료까지 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi에 연결되었습니다.");
  Serial.println("IP 주소: " + WiFi.localIP().toString());

  // mqtt broker에 연결
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.print("mqtt broker에 연결 중...");
    if (client.connect("esp32")) {
      Serial.println("연결되었습니다.");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("실패, rc=");
      Serial.print(client.state());
      Serial.println(" 5초 후 다시 시도합니다.");
      delay(5000);
    }
  }
}

void loop() {
  // mqtt 메시지 처리를 위한 루프
  client.loop();

  // 기타 코드 추가
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("새로운 메시지 도착: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0; i<length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
