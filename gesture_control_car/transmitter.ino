#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <MPU6050_light.h>

// Replace with receiver’s MAC address
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};

// Data structure to send
typedef struct {
  bool f;  // Forward
  bool b;  // Backward
  bool l;  // Left
  bool r;  // Right
} message;

message data;

MPU6050 mpu(Wire);
unsigned long timer = 0;
float x, y;

esp_now_peer_info_t peerInfo;

// Callback for data transmission status
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    while (true);
  }

  // Register send callback
  esp_now_register_send_cb(onDataSent);

  // Register peer information
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer!");
    while (true);
  }

  // Initialize MPU6050
  Wire.begin();
  byte status = mpu.begin();
  Serial.print("MPU6050 status: ");
  Serial.println(status);

  while (status != 0);  // Stop if connection fails

  Serial.println("Calibrating... Keep MPU6050 still.");
  delay(1000);
  mpu.calcOffsets();
  Serial.println("Calibration done!");
}

void loop() {
  mpu.update();

  if (millis() - timer > 10) {  // Send data every 10ms
    x = mpu.getAngleX();
    y = mpu.getAngleY();

    // Direction detection based on tilt angle
    data.f = (x >= 30);
    data.b = (x <= -30);
    data.r = (y >= 30);
    data.l = (y <= -30);

    // Debug output
    Serial.printf("X: %.2f | Y: %.2f | F: %d B: %d R: %d L: %d\n",
                  x, y, data.f, data.b, data.r, data.l);

    // Transmit data via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(data));

    timer = millis();
  }
}
