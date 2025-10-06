#include <esp_now.h>
#include <WiFi.h>

// Motor driver pin definitions
#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21

// Direction flags
bool front = false, back = false, right = false, left = false;

// Data structure (must match sender)
typedef struct {
  bool f;
  bool b;
  bool l;
  bool r;
} message;

message data;

// Handle incoming ESP-NOW messages
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(message)) {
    memcpy(&data, incomingData, sizeof(data));

    // Update control flags
    front = data.f;
    back  = data.b;
    right = data.r;
    left  = data.l;

    Serial.printf("Received -> F:%d B:%d R:%d L:%d\n", front, back, right, left);
  } else {
    Serial.println("Invalid data length received");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP-NOW Receiver Initializing...");

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  carStop();

  // WiFi in station mode (required for ESP-NOW)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("WiFi set to STA mode");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    ESP.restart();
  }
  Serial.println("ESP-NOW Initialized Successfully");

  // Register receive callback
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("Receive Callback Registered");
}

void loop() {
  // Drive motor based on received direction
  if (front)      carForward();
  else if (back)  carBackward();
  else if (right) carRight();
  else if (left)  carLeft();
  else            carStop();
}

// Motor control functions
void carForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Moving Forward");
}

void carBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Moving Backward");
}

void carRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Turning Right");
}

void carLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Turning Left");
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Stopped");
}
