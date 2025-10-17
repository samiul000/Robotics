#include <Servo.h>

Servo servo;

#define echoPin 9
#define trigPin 10
#define sensorPin A0

long duration;
float distance = 0;
int maxDistance = 10;  // Max distance to detect waste (cm)
int maxDryValue = 60;  // Threshold (in %) between DRY and WET — tune this!

void setup() {
    Serial.begin(9600);

    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(sensorPin, INPUT);

    servo.attach(8);
    servo.write(90);  
}

void loop() {

    distance = 0;

    for (int i = 0; i < 2; i++) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        if (duration > 0) {
            distance += (duration * 0.0343) / 2; // Convert to cm
        }
        delay(10);
    }
    distance = distance / 2.0;

    // --- If object detected within range ---
    if (distance < maxDistance && distance > 1) {
        delay(500);

        int sensorVal = 0;
        int value = 0;

        // --- Read and average moisture ---
        for (int i = 0; i < 5; i++) {  // 5 samples for smoother output
            sensorVal = analogRead(sensorPin);

            // Typical water level sensor values: ~200 (wet) to ~1023 (dry)
            sensorVal = constrain(sensorVal, 300, 1023);
            value += map(sensorVal, 300, 1023, 100, 0); // Convert to % moisture
            delay(50);
        }
        value = value / 5;  // Average result

        // --- Display readings ---
        Serial.print("Moisture: ");
        Serial.print(value);
        Serial.print("%  ||  Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        // --- Waste classification ---
        if (value > maxDryValue) {
            Serial.println(" ==> WET Waste Detected");
            servo.write(170);  // Move to WET bin
        } else {
            Serial.println(" ==> DRY Waste Detected");
            servo.write(10);   // Move to DRY bin
        }

        delay(3000); 
        servo.write(90); 
        delay(1000);
    }

    delay(300);
}
