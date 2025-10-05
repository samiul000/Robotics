#include <esp_now.h>
#include <WiFi.h>

// Joystick Pins
#define JOYSTICK_X 33
#define JOYSTICK_Y 32

// Replace with the receiver's MAC Address (e.g., {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC})
uint8_t receiverAddress[] = {};

// Data structure for sending joystick direction
typedef struct struct_message {
    int8_t direction; // 0 = stop, 1 = forward, 2 = backward, 3 = left, 4 = right
} struct_message;

struct_message Data; // Create a data instance

// Callback function to check if the message was delivered successfully
void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
    Serial.begin(115200);

    // Initialize WiFi in station mode for ESP-NOW
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        while (true);
    }

    // Register the send callback
    esp_now_register_send_cb(onSent);

    // Register peer (receiver)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 1;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer!");
        while (true);
    }

    Serial.println("ESP-NOW Joystick Sender Ready.");
}

void loop() {
    // Read raw joystick values
    int rawX = analogRead(JOYSTICK_X);
    int rawY = analogRead(JOYSTICK_Y);

    // Define deadzone and center values to reduce jitter
    const int deadzone = 2000;
    const int center = 2048;

    // Determine direction based on joystick position
    if (abs(rawX - center) < deadzone && abs(rawY - center) < deadzone) {
        Data.direction = 0; // Stop
    } 
    else if (abs(rawX - center) < deadzone) {
        Data.direction = (rawY > center) ? 1 : 2; // Forward / Backward
    } 
    else if (abs(rawY - center) < deadzone) {
        Data.direction = (rawX > center) ? 4 : 3; // Right / Left
    } 
    else {
        // Choose the stronger axis movement
        if (abs(rawY - center) > abs(rawX - center))
            Data.direction = (rawY > center) ? 1 : 2;
        else
            Data.direction = (rawX > center) ? 4 : 3;
    }

    // Send data via ESP-NOW
    esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&Data, sizeof(Data));
    if (result != ESP_OK) Serial.println("Send Error!");

    // For debugging
    Serial.print("Direction: ");
    switch (Data.direction) {
        case 0: Serial.println("Stop"); break;
        case 1: Serial.println("Forward"); break;
        case 2: Serial.println("Backward"); break;
        case 3: Serial.println("Left"); break;
        case 4: Serial.println("Right"); break;
    }

    delay(50);
}
