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

const byte forwardInterruptPin = 33;
const byte backwardInterruptPin = 32;
bool speedAchieved = false;
volatile bool forward = true;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

L298N motor(ENA, IN1, IN2);

void setup() {
  Serial.begin(9600);
  motor.setSpeed(currentSpeed);
  motor.forward();
  pinMode(forwardInterruptPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(forwardInterruptPin), goForward, RISING);
  pinMode(backwardInterruptPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(backwardInterruptPin), goBackward, RISING);
}

void loop() {
  // Ramp up the speed
  if(targetSpeed > currentSpeed) {
    currentSpeed += rampSpeed;
  }
  else if(targetSpeed < currentSpeed) {
    currentSpeed -= rampSpeed;
  }

  if(currentSpeed <= 0) {
    currentSpeed = 0;
  }
  if(currentSpeed >= 255) {
    currentSpeed = 255;
    if(!speedAchieved) {
      speedAchievedTime = millis();
      speedAchieved = true;
      Serial.println("max speed achieved");
    }
  }

  motor.setSpeed(currentSpeed);

  if(!speedAchieved || (millis() - speedAchievedTime < 1000)) {
    if(forward) {
      motor.forward();
    }
    else {
      motor.backward();
    }
  }
  else {
    motor.stop();
    Serial.println("Time to stop");
    currentSpeed = 0;
    targetSpeed = 0;
  }
  
  // if(currentSpeed < targetSpeed) {
  //   currentSpeed += rampSpeed;
  // }
  // if(currentSpeed > targetSpeed) {
  //   currentSpeed -= rampSpeed;
  // }

  // if(currentSpeed == 255) {
  //   targetSpeed = 0;
  // }
  // if(currentSpeed == 0) {
  //   targetSpeed = 255;
  // }
  // motor.setSpeed(currentSpeed);
  // motor.forward();
  // Serial.print("Current Speed: ");
  // Serial.println(currentSpeed);
  // Serial.print("Target Speed: ");
  // Serial.println(targetSpeed);
  delay(50);
}


void goForward() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    Serial.print("got forward: ");
    targetSpeed = 255;
    forward = true;
    speedAchieved = false;
    Serial.println(targetSpeed);
  }
}

void goBackward() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    Serial.print("got backward: ");
    targetSpeed = 255;
    forward = false;
    speedAchieved = false;
    Serial.println(targetSpeed);
  }
}
