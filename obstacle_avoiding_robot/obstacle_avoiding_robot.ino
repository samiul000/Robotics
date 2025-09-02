#include <Servo.h>
#include <NewPing.h>

const int servoPin = 2;

// === Motor control pins ===
const int rightMotorF = 6;
const int rightMotorB = 9;
const int leftMotorF  = 10;
const int leftMotorB  = 11;
const int en1 = 3;   
const int en2 = 5; 

// === Ultrasonic sensor pins ===
#define trig_pin 12
#define echo_pin 13
#define max_dis 200   // Maximum distance the sensor can measure (cm)

// === Robot state ===
bool goesFor = false;  // Whether the robot is currently moving forward
int distance = 100;    // Last measured distance

NewPing sonar(trig_pin, echo_pin, max_dis);
Servo servo_motor;

// Motor Speed (0-255)
int motorSpeed = 200;

void setup() {
  Serial.begin(9600);

  pinMode(rightMotorF, OUTPUT);
  pinMode(rightMotorB, OUTPUT);
  pinMode(leftMotorF, OUTPUT);
  pinMode(leftMotorB, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  servo_motor.attach(servoPin);

  // Center servo (forward-facing)
  servo_motor.write(115);
  delay(2000);

  // Stabilize ultrasonic sensor readings
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
  int distanceRight = 0;
  int distanceLeft  = 0;
  delay(50);

  // Check if obstacle is close
  if (distance <= 15) {
    moveStop();
    delay(300);

    moveBack();  
    delay(300);

    moveStop();
    delay(300);

    // Look right and left
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    // Decide turn direction adaptively
    if (distanceRight > distanceLeft) {
      turnRight(map(distanceRight, 0, 200, 200, 600));  // Longer turn if path is very open
    } 
    else if (distanceLeft > distanceRight) {
      turnLeft(map(distanceLeft, 0, 200, 200, 600));
    } 
    else {
      moveForward();
    }
    // Update distance for next loop
    distance = readPing();
  } 
  else {
    moveForward();  // No obstacle → keep moving forward
    distance = readPing();
  }
}

// === Sensor Functions ===

int lookRight() {
  servo_motor.write(50);  // Turn servo right
  delay(300);

  int dist = readPing();
  delay(100);

  servo_motor.write(115); // Reset servo to center
  return dist;
}

int lookLeft() {
  servo_motor.write(170); // Turn servo left
  delay(300);

  int dist = readPing();
  delay(100);

  servo_motor.write(115); // Reset servo to center
  return dist;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();

  if (cm == 0) {
    return 250; 	// If no reading, assume "very far"
  }
  return cm;
}

// === Movement Functions ===

void moveStop() {
  analogWrite(en1, 0);
  analogWrite(en2, 0);
  digitalWrite(rightMotorF, LOW);
  digitalWrite(rightMotorB, LOW);
  digitalWrite(leftMotorF, LOW);
  digitalWrite(leftMotorB, LOW);
}

void moveForward() {
  if (!goesFor) {
    goesFor = true;
  }
  digitalWrite(rightMotorF, HIGH);
  digitalWrite(rightMotorB, LOW);
  digitalWrite(leftMotorF, HIGH);
  digitalWrite(leftMotorB, LOW);

  smoothSpeedUp();
}

void moveBack() {
  goesFor = false;
  digitalWrite(rightMotorF, LOW);
  digitalWrite(rightMotorB, HIGH);
  digitalWrite(leftMotorF, LOW);
  digitalWrite(leftMotorB, HIGH);

  analogWrite(en1, motorSpeed);
  analogWrite(en2, motorSpeed);
}

void turnRight(int turnTime) {
  digitalWrite(rightMotorF, LOW);
  digitalWrite(rightMotorB, HIGH);
  digitalWrite(leftMotorF, HIGH);
  digitalWrite(leftMotorB, LOW);

  analogWrite(en1, motorSpeed);
  analogWrite(en2, motorSpeed);

  delay(turnTime);  // Adaptive turn

  moveForward();
}

void turnLeft(int turnTime) {
  digitalWrite(rightMotorF, HIGH);
  digitalWrite(rightMotorB, LOW);
  digitalWrite(leftMotorF, LOW);
  digitalWrite(leftMotorB, HIGH);

  analogWrite(en1, motorSpeed);
  analogWrite(en2, motorSpeed);

  delay(turnTime);

  moveForward();
}

// === Smooth Speed Control ===

void smoothSpeedUp() {
  for (int i = 100; i <= motorSpeed; i += 10) {  // Ramp up gradually
    analogWrite(en1, i);
    analogWrite(en2, i);
    delay(30);
  }
}
