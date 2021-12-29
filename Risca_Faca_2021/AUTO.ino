void IRRead() {
  String value;
  if (irrecv.decode(&results))
  {
    value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume();
  }

  if (value == "10") {
    if (autoState == STOPPED) {
      //Serial.println("ReadyToGo");
      autoState = READY;
      MotorWrite(90, 90);
      CalibrateSensors();
    }
  } else if (value == "810") {
    if (autoState == READY) {
      autoState = RUNNING;
      PS4.setLed(0, 100, 0);
      PS4.sendToController();
      //Serial.println("LET'S GO!!!");
    }
  } else if ( value == "410") {
    if (autoState == RUNNING || autoState == READY) {
      //Serial.println("STOP");
      autoState = STOPPED;
      MotorWrite(90, 90);
    }
  }
}

void Auto() {
  IRRead();
  if (autoState == RUNNING) {
    // if (rightReading) {
    rightSensor = analogRead(rightSensorPin);
    //} else {
    leftSensor = analogRead(leftSensorPin);
    //}
    //rightReading = !rightReading;

    //Serial.print(0);
    //Serial.print(" ");
    //Serial.print(leftSensor);
    //Serial.print(" ");
    //Serial.println(rightSensor);
    if (leftSensor < leftSensorRef && rightSensor < rightSensorRef) {
      MotorWrite(80, 80);
      delay(200);
      MotorWrite(100, 80);
      delay(200);
    } else if (leftSensor < leftSensorRef) {
      MotorWrite(100, 80);
      //leftSensor = analogRead(leftSensorPin);
      //        Serial.print(0);
      //        Serial.print(" ");
      //        Serial.print(leftSensor);
      //        Serial.print(" ");
      //        Serial.println(rightSensor);
      //Serial.println("Esquerda");
      //IRRead();
    } else  if (rightSensor < rightSensorRef) {
      MotorWrite(80, 95);
      //rightSensor = analogRead(rightSensorPin);
      //        Serial.print(0);
      //        Serial.print(" ");
      //        Serial.print(leftSensor);
      //        Serial.print(" ");
      //        Serial.println(rightSensor);
      //Serial.println("Direita");
      //IRRead();

      //delay(200);
      //MotorWrite(80, 100);
      //delay(250);
    } else {
      MotorWrite(110, 110);
    }


  } else if (autoState == READY) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, 0, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
      }
      ledOn = !ledOn;
      blinkTimer = millis() + 200;
    }
  } else if (autoState == STOPPED) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, ledIntensity++, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, ledIntensity--, 0);
        PS4.sendToController();
      }
      if (ledIntensity == 0 || ledIntensity == 100) {
        ledOn = !ledOn;
      }
      blinkTimer = millis() + 10;
    }
  }
}

void CalibrateSensors() {
  leftSensorRef = analogRead(leftSensorPin) - leftSensorTolerance;
  rightSensorRef = analogRead(rightSensorPin) - rightSensorTolerance;
}
