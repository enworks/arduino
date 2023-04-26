#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "tp-link-m4";
const char* password = "no01085385309";
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic = "rc_car/directions";

WiFiClient espClient;
PubSubClient client(espClient);

// LED를 연결한 GPIO 핀 번호
const int PIN_21 = 21;
const int PIN_22 = 22;
const int PIN_23 = 23;

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

  // LED 제어를 위한 GPIO 핀 설정
  pinMode(PIN_21, OUTPUT);
  pinMode(PIN_22, OUTPUT);
  pinMode(PIN_23, OUTPUT);

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

  int msg = payload[0] - '0';  // ASCII 문자를 숫자로 변환

  if (msg >= 0 && msg <= 3) {
    // 메시지 값이 0, 1, 2, 3인 경우, 해당하는 GPIO 핀에 LED 켜기
    if (msg == 0) {
      digitalWrite(PIN_21, HIGH);
    } else if (msg == 1) {
      digitalWrite(PIN_22, HIGH);
    } else if (msg == 2) {
      digitalWrite(PIN_23, HIGH);
    } else {
      // msg가 3인 경우
      digitalWrite(PIN_21, HIGH);
      digitalWrite(PIN_22, HIGH);
      digitalWrite(PIN_23, HIGH);
    }
  } else if (strcmp((char*)payload, "1") == 0) {
    digitalWrite(PIN_22, HIGH);
  } else if (strcmp((char*)payload, "2") == 0) {
    digitalWrite(PIN_23, HIGH);
  } else if (strcmp((char*)payload, "3") == 0) {
    digitalWrite(PIN_21, HIGH);
    digitalWrite(PIN_22, HIGH);
    digitalWrite(PIN_23, HIGH);
  }
}
