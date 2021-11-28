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
    if (autoState == RUNNING) {
      //Serial.println("STOP");
      autoState = STOPPED;
    }
  }
}

void Auto() {
  IRRead();
  if (autoState == RUNNING) {
    int leftSensor = analogRead(leftSensorPin);
    int rightSensor = analogRead(rightSensorPin);
    //Serial.print(leftSensor);
    //Serial.print(" ");
    //Serial.println(rightSensor);

    if (leftSensor < leftSensorRef) {
      while (leftSensor < leftSensorRef && autoState == RUNNING) {
        MotorWrite(70, 70);
        leftSensor = analogRead(leftSensorPin);
        IRRead();
      }
      delay(250);
      MotorWrite(80, 100);
      delay(150);
    }
    else if (rightSensor < rightSensorRef) {
      while (rightSensor < rightSensorRef && autoState == RUNNING) {
        MotorWrite(70, 70);
        rightSensor = analogRead(rightSensorPin);
        IRRead();
      }
      delay(250);
      MotorWrite(100, 80);
      delay(150);
    }
    MotorWrite(120, 120);

  } else if (autoState == READY) {
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
  leftSensorRef = analogRead(leftSensorPin) - sensorsTolerance;
  rightSensorRef = analogRead(rightSensorPin) - sensorsTolerance;
}
