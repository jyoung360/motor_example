#include <L298N.h>

// Define the pins for controlling the motor driver
const int ENA = 25;  // Enable pin for motor A
const int IN1 = 26;  // Input 1 pin for motor A
const int IN2 = 27;  // Input 2 pin for motor A

// Define variables
int motorSpeed = 0;  // Starting speed
int rampSpeed = 5;  // Speed increment for ramping up or down
int maxSpeed = 255;  // Maximum speed
int targetSpeed = 0; // Target speed
int currentSpeed = 0; // current motor speed
int speedAchievedTime = 0;

const int topLimitSwitchPin = 33;
const int bottomLimitSwitchPin = 14;
const int reversePin = 12;
const int forwardPin = 13;

bool speedAchieved = false;
volatile bool forward = true;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

L298N motor(ENA, IN1, IN2);

void setup() {
  Serial.begin(9600);
  motor.setSpeed(currentSpeed);
  motor.forward();
  pinMode(topLimitSwitchPin, INPUT_PULLUP);
  pinMode(bottomLimitSwitchPin, INPUT_PULLUP);
  pinMode(forwardPin, INPUT_PULLUP);
  pinMode(reversePin, INPUT_PULLUP);
}

void loop() {
  int topLimitSwitchVal = digitalRead(topLimitSwitchPin);
  int bottomLimitSwitchVal = digitalRead(bottomLimitSwitchPin);
  int forwardVal = digitalRead(forwardPin);
  int reverseVal = digitalRead(reversePin);

  Serial.print("topLimitSwitchVal: ");
  Serial.println(topLimitSwitchVal);

  Serial.print("bottomLimitSwitchVal: ");
  Serial.println(bottomLimitSwitchVal);

  Serial.print("forwardVal: ");
  Serial.println(forwardVal);

  Serial.print("reverseVal: ");
  Serial.println(reverseVal);

  if (forwardVal == 0){
    motorSpeed = 255;
    forward = true;
  }
  
  if (reverseVal == 0){
    motorSpeed = 255;
    forward=false;
  }

  if (topLimitSwitchVal == 0) {
    Serial.print("In topLimitSwitchVal: ");
    if(!forward){
      Serial.println("backward");
      motor.setSpeed(motorSpeed);
      motor.backward();
    }
    else { 
      Serial.println("stop");
      motor.stop();
      motorSpeed=0;
    }
  }
  else if (bottomLimitSwitchVal == 0) {
    Serial.print("In bottomLimitSwitchVal: ");
    if(forward){
      Serial.println("foward");
      motor.setSpeed(motorSpeed);
      motor.forward();
    }
    else { 
      Serial.println("stop");
      motor.stop();
      motorSpeed=0;
    }
  }
  else {
    Serial.println(forward);
    Serial.println(motorSpeed);
    motor.setSpeed(motorSpeed);
    if(forward) {
      motor.forward();
    }
    else {
      motor.backward();
    }
  }

  delay(100);
}
