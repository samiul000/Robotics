// C++ code
//

int IR_Pin1 = 4;
int IR_Pin2 = 3;

int en1 = 5; // PWM
int en2 = 6; // PWM

int motorRightF = 9;
int motorRightB = 10;
int motorLeftF = 11;
int motorLeftB = 12;

int motorSpeed = 200;

int IR_Val1, IR_Val2;

void setup()
{
  Serial.begin(9600);
  
  pinMode(IR_Pin1, INPUT);
  pinMode(IR_Pin2, INPUT);
  pinMode(motorRightF, OUTPUT);
  pinMode(motorRightB, OUTPUT);
  pinMode(motorLeftF, OUTPUT);
  pinMode(motorLeftB, OUTPUT);
  
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  
  rotate(0,0);
  
}

void loop()
{
  IR_Val1 = digitalRead(IR_Pin1);
  IR_Val2 = digitalRead(IR_Pin2);
  
  if(IR_Val1 == 0 && IR_Val2 == 0)
  {
    rotate(motorSpeed, motorSpeed); 	// Forward
  }
  else if(IR_Val1 == 1 && IR_Val2 == 0) 
  {
    rotate(-motorSpeed,motorSpeed);		// Left
  }
  else if(IR_Val1 == 0 && IR_Val2 == 1) 
  {
    rotate(motorSpeed, -motorSpeed);	// Right
  }
  else{
    rotate(0,0);
  }
  
  Serial.println(IR_Val1);
  Serial.println(IR_Val2);
}

void rotate(int speedRight, int speedLeft)
{
  
  // === Right Motor ===
  if(speedRight < 0)
  {
    digitalWrite(motorRightF, LOW);
    digitalWrite(motorRightB, HIGH);
  }
  else if(speedRight > 0)
  {
    digitalWrite(motorRightF, HIGH);
    digitalWrite(motorRightB, LOW);
  }
  else{
    digitalWrite(motorRightF, LOW);
    digitalWrite(motorRightB, LOW);
  }
  
  
  // === Left Motor ===
  if(speedLeft < 0)
  {
    digitalWrite(motorLeftF, LOW);
    digitalWrite(motorLeftB, HIGH);
  }
  else if(speedLeft > 0)
  {
    digitalWrite(motorLeftF, HIGH);
    digitalWrite(motorLeftB, LOW);
  }
  else{
    digitalWrite(motorLeftF, LOW);
    digitalWrite(motorLeftB, LOW);
  }
  
  analogWrite(en1, abs(speedRight));
  analogWrite(en2, abs(speedLeft));
}