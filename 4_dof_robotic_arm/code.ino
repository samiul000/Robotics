#include <Servo.h>

Servo servo_x_axis;
Servo servo_y_axis;
Servo servo_z_axis;
Servo servo_clamp;

// --- Servo Positions (initial angles) ---
int x_axis_degree = 120;
int y_axis_degree = 90;
int z_axis_degree = 85;
int clamp_degree  = 90;

// --- Joystick Pins ---
#define LEFT_JOYSTICK_X A0
#define LEFT_JOYSTICK_Y A1
#define RIGHT_JOYSTICK_X A2
#define RIGHT_JOYSTICK_Y A3

// --- Configuration ---
const int DEADZONE_LOW  = 470;   // Joystick center tolerance
const int DEADZONE_HIGH = 550;
const int SERVO_STEP    = 2;     // Smaller step = smoother motion
const int UPDATE_DELAY  = 30;    // Minimum time between servo updates (ms)

// --- Servo Limits ---
const int X_MIN = 40,  X_MAX = 175;
const int Y_MIN = 5,   Y_MAX = 150;
const int Z_MIN = 15,  Z_MAX = 145;
const int CLAMP_MIN = 75, CLAMP_MAX = 90;

// --- Timing ---
unsigned long lastUpdateTime = 0;

void setup() {
    Serial.begin(9600);

    // Attach servos to pins
    servo_x_axis.attach(3);
    servo_y_axis.attach(5);
    servo_z_axis.attach(6);
    servo_clamp.attach(9);

    // Move servos to initial positions
    servo_x_axis.write(x_axis_degree);
    servo_y_axis.write(y_axis_degree);
    servo_z_axis.write(z_axis_degree);
    servo_clamp.write(clamp_degree);
}

void loop() {
    
    int left_x  = analogRead(LEFT_JOYSTICK_X);
    int left_y  = analogRead(LEFT_JOYSTICK_Y);
    int right_x = analogRead(RIGHT_JOYSTICK_X);
    int right_y = analogRead(RIGHT_JOYSTICK_Y);

    // Update every few milliseconds (prevents jitter)
    if (millis() - lastUpdateTime >= UPDATE_DELAY) {
        lastUpdateTime = millis();

        // --- Y-Axis control (Left Joystick X) ---
        if (left_x < DEADZONE_LOW)
            y_axis_degree -= SERVO_STEP;
        else if (left_x > DEADZONE_HIGH)
            y_axis_degree += SERVO_STEP;

        // --- Clamp control (Left Joystick Y) ---
        if (left_y < DEADZONE_LOW)
            clamp_degree -= SERVO_STEP;
        else if (left_y > DEADZONE_HIGH)
            clamp_degree += SERVO_STEP;

        // --- X-Axis control (Right Joystick X) ---
        if (right_x < DEADZONE_LOW)
            x_axis_degree -= SERVO_STEP;
        else if (right_x > DEADZONE_HIGH)
            x_axis_degree += SERVO_STEP;

        // --- Z-Axis control (Right Joystick Y) ---
        if (right_y < DEADZONE_LOW)
            z_axis_degree -= SERVO_STEP;
        else if (right_y > DEADZONE_HIGH)
            z_axis_degree += SERVO_STEP;

        // --- Constrain all angles to safe limits ---
        x_axis_degree = constrain(x_axis_degree, X_MIN, X_MAX);
        y_axis_degree = constrain(y_axis_degree, Y_MIN, Y_MAX);
        z_axis_degree = constrain(z_axis_degree, Z_MIN, Z_MAX);
        clamp_degree  = constrain(clamp_degree, CLAMP_MIN, CLAMP_MAX);

        // --- Apply servo angles ---
        servo_x_axis.write(x_axis_degree);
        servo_y_axis.write(y_axis_degree);
        servo_z_axis.write(z_axis_degree);
        servo_clamp.write(clamp_degree);

        Serial.print("X: "); 
        Serial.print(x_axis_degree);
        Serial.print(" | Y: "); 
        Serial.print(y_axis_degree);
        Serial.print(" | Z: "); 
        Serial.print(z_axis_degree);
        Serial.print(" | Clamp: "); 
        Serial.println(clamp_degree);
    }
}
