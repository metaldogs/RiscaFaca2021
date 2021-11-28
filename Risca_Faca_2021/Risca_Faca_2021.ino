// Code made by Nick
// Contact: nicolas_castrosilva@outlook.com

//Servo Libraries
#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

//PS4 Controller Libraries
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

//Robot states of operation
enum robotStates {
  LOCKED, AUTO, MANUAL
};
robotStates robotState = LOCKED;

bool optionPressed = false;


//IR Remote library and variables
#include <IRremote.h>
#define irReceiverPin 15
IRrecv irrecv(irReceiverPin);
decode_results results;

//Edge Sensors Pins and Variables
#define leftSensorPin 34
#define rightSensorPin 35

int leftSensorRef = 0;
int rightSensorRef = 0;
int sensorsTolerance = 100;

//Auto mode states of operation
enum autoStates {
  STOPPED, READY, RUNNING
};
autoStates autoState = STOPPED;

//PS4 LED status variables
unsigned long blinkTimer;
bool ledOn = true;
int ledIntensity;

//Define Motors Pins and Instances
#define leftMotorPin 25
#define rightMotorPin 26

Servo MotorEsquerdo;
Servo MotorDireito;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn(); //Enable IR Receiver

  PS4.begin("70:77:81:d5:f8:42"); //Start Connection between ESP32 and PS4 Controller

  while (!PS4.isConnected()) {
    Serial.println("WatingConnection");
    delay(250);
  }

  PS4.setLed(100, 0, 0);
  PS4.sendToController();
  
  MotorEsquerdo.attach(leftMotorPin);
  MotorDireito.attach(rightMotorPin);
  MotorEsquerdo.write(90);
  MotorDireito.write(90);

  Serial.println("Ready and LOCKED");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (PS4.isConnected()) {
    Status_Verify();
  } else {
    MotorEsquerdo.write(90);
    MotorDireito.write(90);
  }
}

void Status_Verify() {
  if (PS4.Options()) {
    if (!optionPressed) {
      optionPressed = true;
      if (robotState == LOCKED) {
        robotState = AUTO;
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("AUTO");
      } else if (robotState == AUTO) {
        robotState = MANUAL;
        PS4.setLed(0, 0, 100);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("MANUAL");
      } else if (robotState == MANUAL) {
        robotState = LOCKED;
        PS4.setLed(100, 0, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("LOCKED");
      }
    }
  } else {
    optionPressed = false;
  }

  if (robotState == MANUAL) {
    ManualControl();
  } else if (robotState == AUTO) {
    Auto();
  }
}

void MotorWrite(int ppmDireito, int ppmEsquerdo) {
  MotorDireito.write(ppmDireito);
  MotorEsquerdo.write(ppmEsquerdo);
}


//    // Below has all accessible outputs from the controller
//    if (PS4.isConnected()) {
//      if (PS4.Right()) Serial.println("Right Button");
//      if (PS4.Down()) Serial.println("Down Button");
//      if (PS4.Up()) Serial.println("Up Button");
//      if (PS4.Left()) Serial.println("Left Button");
//
//      if (PS4.Square()) Serial.println("Square Button");
//      if (PS4.Cross()) Serial.println("Cross Button");
//      if (PS4.Circle()) Serial.println("Circle Button");
//      if (PS4.Triangle()) Serial.println("Triangle Button");
//
//      if (PS4.UpRight()) Serial.println("Up Right");
//      if (PS4.DownRight()) Serial.println("Down Right");
//      if (PS4.UpLeft()) Serial.println("Up Left");
//      if (PS4.DownLeft()) Serial.println("Down Left");
//
//      if (PS4.L1()) Serial.println("L1 Button");
//      if (PS4.R1()) Serial.println("R1 Button");
//
//      if (PS4.Share()) Serial.println("Share Button");
//      if (PS4.Options()) Serial.println("Options Button");
//      if (PS4.L3()) Serial.println("L3 Button");
//      if (PS4.R3()) Serial.println("R3 Button");
//
//      if (PS4.PSButton()) Serial.println("PS Button");
//      if (PS4.Touchpad()) Serial.println("Touch Pad Button");
//
//      if (PS4.L2()) {
//        Serial.printf("L2 button at %d\n", PS4.L2Value());
//      }
//      if (PS4.R2()) {
//        Serial.printf("R2 button at %d\n", PS4.R2Value());
//      }

//    if (PS4.LStickX()) {
//      Serial.printf("Left Stick x at %d\n", PS4.LStickX());
//    }
//    if (PS4.LStickY()) {
//      Serial.printf("Left Stick y at %d\n", PS4.LStickY());
//    }
//    if (PS4.RStickX()) {
//      Serial.printf("Right Stick x at %d\n", PS4.RStickX());
//    }
//    if (PS4.RStickY()) {
//      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
//    }
//
//    if (PS4.Charging()) Serial.println("The controller is charging");
//    if (PS4.Audio()) Serial.println("The controller has headphones attached");
//    if (PS4.Mic()) Serial.println("The controller has a mic attached");
//
//    Serial.printf("Battery Level : %d\n", PS4.Battery());
//
//    Serial.println();
//    // This delay is to make the output more human readable
//    // Remove it when you're not trying to see the output
//    delay(1000);




