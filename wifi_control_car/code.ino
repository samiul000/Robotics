#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ENA D0
#define ENB D5
#define IN_1 D1
#define IN_2 D2
#define IN_3 D3
#define IN_4 D4

String command;
int speed = 200; // Speed of motor -- 0-255

const char* ssid = "WiFi Car";

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN_1, OUTPUT);
    pinMode(IN_2, OUTPUT);
    pinMode(IN_3, OUTPUT);
    pinMode(IN_4, OUTPUT);

    // WiFi Setup
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Server Setup
    server.on("/", HTTP_handleRoot);
    server.onNotFound(HTTP_handleRoot);
    server.begin();
}

// === Control Functions ===
void forward() {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(ENA, speed);

    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(ENB, speed);
}

void backward() {
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(ENA, speed);

    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(ENB, speed);
}

void right() {
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(ENA, speed);

    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(ENB, speed);
}

void left() {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(ENA, speed);

    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(ENB, speed);
}

void stop() {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    analogWrite(ENA, 0);

    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
    analogWrite(ENB, 0);
}

// === Main Loop ===
void loop() {
    server.handleClient();

    if (server.hasArg("State")) {
        command = server.arg("State");

        if (command == "F") forward();
        else if (command == "B") backward();
        else if (command == "R") right();
        else if (command == "L") left();
        else if (command == "S") stop();
        else Serial.println("Invalid command!");
    }
}

void HTTP_handleRoot(void) {
    if (server.hasArg("State")) {
        Serial.print("Received: ");
        Serial.println(server.arg("State"));
    }
    server.send(200, "text/plain", "OK");
}
