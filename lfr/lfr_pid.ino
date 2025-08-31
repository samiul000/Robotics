// --- Line Follower with 2 IR sensors and PID control ---

int sensorPins[2] = {A0, A1};  // IR sensors

int button = 2;

int en1 = 3; // PWM
int en2 = 5; // PWM

int motorRightF = 9;
int motorRightB = 10;
int motorLeftF = 11;
int motorLeftB = 6;

// Speeds
int motorSpeed = 200;
int rSpeed, lSpeed;

// PID 
float p, i, d, lp, error, corr, sp;
float Kp = 5;
float Ki = 0;
float Kd = 40;

void setup() {
  Serial.begin(9600);

  pinMode(button, INPUT_PULLUP);

  pinMode(motorRightF, OUTPUT);
  pinMode(motorRightB, OUTPUT);
  pinMode(motorLeftF, OUTPUT);
  pinMode(motorLeftB, OUTPUT);

  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  // Wait for button press
  while (digitalRead(button) == HIGH)
  {
  	sp = 0; // setpoint = centered line
  }
}

void loop() {
  pid_calc();
  calc_turn();
}

void pid_calc() 
{
  long s0 = analogRead(sensorPins[0]);  // left sensor
  long s1 = analogRead(sensorPins[1]);  // right sensor

  long sensor_avg = s0 * (-1) + s1 * (+1);
  long sensor_sum = s0 + s1;

  int pos = sensor_avg / sensor_sum; // -1 (left) to +1 (right)

  error = pos - sp;

  p = error; 	// Proportional
  i += p;		// Integral
  d = p - lp;	// Derivative
  lp = p;		// Previous Error

  corr = Kp * p + Ki * i + Kd * d; // PID Calculation

  Serial.print("pos: "); Serial.print(pos);
  Serial.print(" error: "); Serial.print(error);
  Serial.print(" corr: "); Serial.println(corr);
}

void calc_turn() 
{
  rSpeed = motorSpeed + corr;
  lSpeed = motorSpeed - corr;

  rSpeed = constrain(rSpeed, -255, 255);
  lSpeed = constrain(lSpeed, -255, 255);

  motor_drive(rSpeed, lSpeed);
}

void motor_drive(int right, int left) 
{
  // Right motor
  if (right >= 0) {
    digitalWrite(motorRightF, HIGH);
    digitalWrite(motorRightB, LOW);
  } else {
    digitalWrite(motorRightF, LOW);
    digitalWrite(motorRightB, HIGH);
  }
  analogWrite(en1, abs(right));

  // Left motor
  if (left >= 0) {
    digitalWrite(motorLeftF, HIGH);
    digitalWrite(motorLeftB, LOW);
  } else {
    digitalWrite(motorLeftF, LOW);
    digitalWrite(motorLeftB, HIGH);
  }
  analogWrite(en2, abs(left));
}
