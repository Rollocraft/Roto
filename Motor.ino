/*
    Motor.ino - Funktionen zum Fahren.
    Created by Rollocraft
    
*/

void TurnRight(int time) {
  digitalWrite(GelbeLEDRechts, HIGH);
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
  analogWrite(motorENA, 100);
  analogWrite(motorENB, 100);
  delay(time);
  digitalWrite(GelbeLEDRechts, LOW);
}

void TurnLeft(int time) {
  digitalWrite(GelbeLEDLinks, HIGH);
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
  analogWrite(motorENA, 100);
  analogWrite(motorENB, 100);
  delay(time);
  digitalWrite(GelbeLEDLinks, LOW);
}

void Forward(int speed) {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
  analogWrite(motorENA, speed);
  analogWrite(motorENB, speed);
}

void Backward(int speed) {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
  analogWrite(motorENA, speed);
  analogWrite(motorENB, speed);
}

void Stop() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
  analogWrite(motorENA, 0);
  analogWrite(motorENB, 0);
}