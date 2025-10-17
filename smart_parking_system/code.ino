#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD at I2C address 0x27 with 20 columns and 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servo;

// IR sensor pins
#define ir_enter 2   // Sensor at the entrance
#define ir_back 4    // Sensor at the exit

#define ir_slot1 5   // Slot 1 sensor
#define ir_slot2 6   // Slot 2 sensor
#define ir_slot3 7   // Slot 3 sensor

// Variables to hold slot states (1 = occupied, 0 = empty)
int s1 = 0, s2 = 0, s3 = 0;

// Flags to prevent multiple counting when a car is detected once
int flag1 = 0, flag2 = 0;

// Total available slots (initially 3)
int slot = 3;

void setup()
{
    Serial.begin(9600);

    // Configure IR sensor pins as inputs
    pinMode(ir_enter, INPUT);
    pinMode(ir_back, INPUT);
    pinMode(ir_slot1, INPUT);
    pinMode(ir_slot2, INPUT);
    pinMode(ir_slot3, INPUT);

    // Attach servo to pin 3 and position it at 90° (neutral / closed)
    servo.attach(3);
    servo.write(90);

    // Initialize and light up the LCD
    lcd.init();
    lcd.backlight();

    // Read initial sensor status
    Read_Sensor();

    // Calculate available slots at startup
    slot = 3 - (s1 + s2 + s3);
}

void loop()
{
    Read_Sensor();

    // Update LCD with current parking status
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Available Slots: ");
    lcd.print(slot);

    lcd.setCursor(0, 1);
    lcd.print("S1: ");
    lcd.print(s1 ? "Occupied " : "Empty    ");

    lcd.setCursor(0, 2);
    lcd.print("S2: ");
    lcd.print(s2 ? "Occupied " : "Empty    ");

    lcd.setCursor(0, 3);
    lcd.print("S3: ");
    lcd.print(s3 ? "Occupied " : "Empty    ");

    // ===== CAR ENTERING LOGIC =====
    if (digitalRead(ir_enter) == 0 && flag1 == 0) // Car detected at entrance
    {
        if (slot > 0) // Check if space is available
        {
            flag1 = 1;          // Set entry flag
            servo.write(180);   // Open gate
            slot--;             // One slot less available
            delay(1500);        // Wait for car to pass
            servo.write(90);    // Close gate
        }
        else // Parking is full
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Parking Full");
            delay(2000);
        }
    }

    // ===== CAR EXITING LOGIC =====
    if (digitalRead(ir_back) == 0 && flag2 == 0) // Car detected at exit
    {
        flag2 = 1;          // Set exit flag
        servo.write(0);     // Open gate for exit
        slot++;             // Free up one slot
        delay(1500);        // Wait for car to leave
        servo.write(90);    // Close gate
    }

    // ===== FLAG RESETTING =====
    // Reset entry/exit flags once sensors are clear
    if (digitalRead(ir_enter) == 1) flag1 = 0;
    if (digitalRead(ir_back) == 1) flag2 = 0;

    delay(300);
}

// Function to read all slot sensors
void Read_Sensor()
{
    // When IR beam is broken (value = 0), slot is occupied (1)
    s1 = (digitalRead(ir_slot1) == 0) ? 1 : 0;
    s2 = (digitalRead(ir_slot2) == 0) ? 1 : 0;
    s3 = (digitalRead(ir_slot3) == 0) ? 1 : 0;
}
