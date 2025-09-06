#include <Servo.h>

const int servoPin = 3;

// === Defines Tirg and Echo pins of the Ultrasonic Sensor ===
const int trigPin = 10;
const int echoPin = 11;

// === Variables for the duration and the distance ===
long duration;
int distance;

Servo radarServo; // Creates a servo object for controlling the servo motor

void setup() {
    
    Serial.begin(9600);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT); 
    radarServo.attach(servoPin);
}

void loop() {
  
  // Rotates the servo motor from 15 to 165 degrees
  for(int i=15; i<=165; i++){  
  radarServo.write(i);
  delay(30);
  distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
  
  Serial.print(i); 
  Serial.print(","); 
  Serial.print(distance); 
  Serial.print("."); 
  }
  
  // Repeats the previous lines from 165 to 15 degrees
  for(int i=165; i>15; i--){  
  radarServo.write(i);
  delay(30);
  distance = calculateDistance();

  Serial.print(i);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
  }
}

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= (duration * 0.034)/2;

  return distance;
}