#include <WiFi.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32wifi"); // ESP32wifi라는 이름의 블루투스 모듈 생성
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  String mac = WiFi.macAddress(); // ESP32wifi 보드의 Mac address 가져오기
  Serial.println(mac);
  delay(1000);
}