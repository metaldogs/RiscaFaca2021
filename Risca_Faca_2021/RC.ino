void ManualControl() {
  int forward = PS4.R2Value();
  int steering = PS4.LStickX();
  int backward = PS4.L2Value();

  int leftMotorOutput = constrain(map(forward, 0, 255, 90, 150)  - (map(backward, 0, 255, 0, 60) + (map(steering, -127, 127, 70, 110) - 90)), 30, 150);
  int rightMotorOutput = constrain(map(forward, 0, 255, 90, 150) - (map(backward, 0, 255, 0, 60) - (map(steering, -127, 127, 70, 110) - 90)), 30, 150);
  //Serial.println(map(steering, -127, 127, 30, 150)-90);

  if (leftMotorOutput > 95 || leftMotorOutput < 85) {
    MotorEsquerdo.write(leftMotorOutput);
  } else {
    MotorEsquerdo.write(90);
  }
  if (rightMotorOutput > 95 || rightMotorOutput < 85) {
    MotorDireito.write(rightMotorOutput);
  } else {
    MotorDireito.write(PS4.R2Value());
  }


  Serial.print(rightMotorOutput);
  Serial.print(" ");
  Serial.println(leftMotorOutput);
}
