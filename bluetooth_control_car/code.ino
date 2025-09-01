/* Bluetooth Control Car */

char data;

// === Right Motor Pins ===
#define motorRightFF 4
#define motorRightFB 5
#define motorRightBF 8
#define motorRightBB 9

// === Left Motor Pins ===
#define motorLeftFF 2
#define motorLeftFB 3
#define motorLeftBF 6
#define motorLeftBB 7

void setup()
{
    Serial.begin(9600);

    pinMode(motorRightFF, OUTPUT);
    pinMode(motorRightFB, OUTPUT);
    pinMode(motorRightBF, OUTPUT);
    pinMode(motorRightBB, OUTPUT);
    pinMode(motorLeftFF, OUTPUT);
    pinMode(motorLeftFB, OUTPUT);
    pinMode(motorLeftBF, OUTPUT);
    pinMode(motorLeftBB, OUTPUT);
}

void control()
{
    if (Serial.available() > 0)
    {
        data = Serial.read();   // read the command

        switch (data)
        {
            case 'F':   // Forward
                digitalWrite(motorRightFF, HIGH);
                digitalWrite(motorRightFB, LOW);
                digitalWrite(motorRightBF, HIGH);
                digitalWrite(motorRightBB, LOW);
                digitalWrite(motorLeftFF, HIGH);
                digitalWrite(motorLeftFB, LOW);
                digitalWrite(motorLeftBF, HIGH);
                digitalWrite(motorLeftBB, LOW);
                break;

            case 'B':   // Backward
                digitalWrite(motorRightFF, LOW);
                digitalWrite(motorRightFB, HIGH);
                digitalWrite(motorRightBF, LOW);
                digitalWrite(motorRightBB, HIGH);
                digitalWrite(motorLeftFF, LOW);
                digitalWrite(motorLeftFB, HIGH);
                digitalWrite(motorLeftBF, LOW);
                digitalWrite(motorLeftBB, HIGH);
                break;

            case 'R':   // Right turn
                digitalWrite(motorRightFF, LOW);
                digitalWrite(motorRightFB, HIGH);
                digitalWrite(motorRightBF, LOW);
                digitalWrite(motorRightBB, LOW);
                digitalWrite(motorLeftFF, LOW);
                digitalWrite(motorLeftFB, LOW);
                digitalWrite(motorLeftBF, HIGH);
                digitalWrite(motorLeftBB, LOW);
                break;

            case 'L':   // Left turn
                digitalWrite(motorRightFF, LOW);
                digitalWrite(motorRightFB, HIGH);
                digitalWrite(motorRightBF, LOW);
                digitalWrite(motorRightBB, LOW);
                digitalWrite(motorLeftFF, HIGH);
                digitalWrite(motorLeftFB, LOW);
                digitalWrite(motorLeftBF, LOW);
                digitalWrite(motorLeftBB, LOW);
                break;

            default:    // Stop
                digitalWrite(motorRightFF, LOW);
                digitalWrite(motorRightFB, LOW);
                digitalWrite(motorRightBF, LOW);
                digitalWrite(motorRightBB, LOW);
                digitalWrite(motorLeftFF, LOW);
                digitalWrite(motorLeftFB, LOW);
                digitalWrite(motorLeftBF, LOW);
                digitalWrite(motorLeftBB, LOW);
                break;
        }
    }
}

void loop()
{
    control();
}
