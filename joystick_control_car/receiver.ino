#include <esp_now.h>
#include <WiFi.h>

// Motor driver pins
#define ENA 14
#define ENB 32
#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21

// Structure to receive direction data
typedef struct struct_message {
    int8_t direction; // 0=Stop, 1=Forward, 2=Backward, 3=Left, 4=Right
} struct_message;

struct_message incomingData;

// Handle received data
void onReceive(const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
    memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

    Serial.print("Direction received: ");
    switch (incomingData.direction) {
        case 0: Serial.println("Stop"); break;
        case 1: Serial.println("Forward"); break;
        case 2: Serial.println("Backward"); break;
        case 3: Serial.println("Left"); break;
        case 4: Serial.println("Right"); break;
        default: Serial.println("Unknown"); break;
    }

    moveMotors(incomingData.direction);
}

// Move motors according to received direction
void moveMotors(int8_t dir) {
    int speedA = 200; // Adjust for motor speed
    int speedB = 200;

    switch (dir) {
        case 0: // Stop
            analogWrite(ENA, 0);
            analogWrite(ENB, 0);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            break;

        case 1: // Forward
            analogWrite(ENA, speedA);
            analogWrite(ENB, speedB);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;

        case 2: // Backward
            analogWrite(ENA, speedA);
            analogWrite(ENB, speedB);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;

        case 3: // Left
            analogWrite(ENA, speedA);
            analogWrite(ENB, speedB);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;

        case 4: // Right
            analogWrite(ENA, speedA);
            analogWrite(ENB, speedB);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize WiFi in station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        while (true);
    }

    // Register receive callback
    esp_now_register_recv_cb(onReceive);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.println("Ready to receive ESP-NOW data...");
}

void loop() {
    // Nothing needed here 
}
