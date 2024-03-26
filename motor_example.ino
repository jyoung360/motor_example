#define MOTOR_IN1 25
#define MOTOR_IN2 26
char receivedChar;
boolean startMotor = false;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
}

void loop() {
  recvOneChar();
  // ramp up forward
  if(startMotor) {
    startMotor = false;
    Serial.println("motor running");
    digitalWrite(MOTOR_IN1, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTOR_IN2, i);
      delay(5);
    }

    // forward full speed for one second
    delay(100);
    
    // ramp down forward
    for (int i=255; i>=0; i--) {
      analogWrite(MOTOR_IN2, i);
      delay(5);
    }

    // ramp up backward
    digitalWrite(MOTOR_IN2, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTOR_IN1, i);
      delay(5);
    }

    // backward full speed for one second
    delay(100);

    // ramp down backward
    for (int i=255; i>=0; i--) {
      analogWrite(MOTOR_IN1, i);
      delay(5);
    }
    
  }
}


void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        if(receivedChar == 'a') {
          startMotor = true;
        }
        Serial.print("got input: ");
        Serial.print(receivedChar);
        Serial.println("!");
    }
}
