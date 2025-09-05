#include <IRremote.h>

int irPin = 2;

// === Right Motor Pins ===
#define motorRightFF 7
#define motorRightFB 8
#define motorRightBF 9
#define motorRightBB 10

// === Left Motor Pins ===
#define motorLeftFF 3
#define motorLeftFB 4
#define motorLeftBF 5
#define motorLeftBB 6

unsigned long moveStartTime = 0;
bool moving = false;

void setup()
{
    Serial.begin(9600);

    IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK); // start receiver

    pinMode(motorRightFF, OUTPUT);
    pinMode(motorRightFB, OUTPUT);
    pinMode(motorRightBF, OUTPUT);
    pinMode(motorRightBB, OUTPUT);
    pinMode(motorLeftFF, OUTPUT);
    pinMode(motorLeftFB, OUTPUT);
    pinMode(motorLeftBF, OUTPUT);
    pinMode(motorLeftBB, OUTPUT);
}


void loop()
{
    if (IrReceiver.decode()) 
    {
        unsigned long value = IrReceiver.decodedIRData.decodedRawData;
        Serial.println(value, HEX); // For Debugging

        if(value == EE11BF00)
        {
            Forward();
        }
        else if(value == E619BF00)
        {
            Backward();
        }
        else if(value == E916BF00)
        {
            Right();
        }
        else if(value == EB14BF00)
        {
            Left();
        }
        else if(value == FF00BF00)
        {
            Stop();
        }

        IrReceiver.resume(); // receive next
   }

    // Move for a specific time => Here 2ms
    if(moving && millis() - moveStartTime >= 200)
    {
        Stop();
    }
}

// === Control ===
void Forward()
{
    digitalWrite(motorRightFF, HIGH);
    digitalWrite(motorRightFB, LOW);
    digitalWrite(motorRightBF, HIGH);
    digitalWrite(motorRightBB, LOW);
    digitalWrite(motorLeftFF, HIGH);
    digitalWrite(motorLeftFB, LOW);
    digitalWrite(motorLeftBF, HIGH);
    digitalWrite(motorLeftBB, LOW);

    moving = true;
    moveStartTime = millis();
}

void Backward()
{
    digitalWrite(motorRightFF, LOW);
    digitalWrite(motorRightFB, HIGH);
    digitalWrite(motorRightBF, LOW);
    digitalWrite(motorRightBB, HIGH);
    digitalWrite(motorLeftFF, LOW);
    digitalWrite(motorLeftFB, HIGH);
    digitalWrite(motorLeftBF, LOW);
    digitalWrite(motorLeftBB, HIGH);

    moving = true;
    moveStartTime = millis();            
}

void Right()
{
    digitalWrite(motorRightFF, LOW);
    digitalWrite(motorRightFB, HIGH);
    digitalWrite(motorRightBF, LOW);
    digitalWrite(motorRightBB, LOW);
    digitalWrite(motorLeftFF, LOW);
    digitalWrite(motorLeftFB, LOW);
    digitalWrite(motorLeftBF, HIGH);
    digitalWrite(motorLeftBB, LOW);

    moving = true;
    moveStartTime = millis();
}

void Left()
{
    digitalWrite(motorRightFF, LOW);
    digitalWrite(motorRightFB, HIGH);
    digitalWrite(motorRightBF, LOW);
    digitalWrite(motorRightBB, LOW);
    digitalWrite(motorLeftFF, HIGH);
    digitalWrite(motorLeftFB, LOW);
    digitalWrite(motorLeftBF, LOW);
    digitalWrite(motorLeftBB, LOW);

    moving = true;
    moveStartTime = millis();
}

void Stop()
{
    digitalWrite(motorRightFF, LOW);
    digitalWrite(motorRightFB, LOW);
    digitalWrite(motorRightBF, LOW);
    digitalWrite(motorRightBB, LOW);
    digitalWrite(motorLeftFF, LOW);
    digitalWrite(motorLeftFB, LOW);
    digitalWrite(motorLeftBF, LOW);
    digitalWrite(motorLeftBB, LOW);

    moving = false;
}