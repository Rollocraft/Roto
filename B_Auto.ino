#include <Arduino.h>

int triggerVorne = 8;
int echoVorne = 9;

int triggerLinks = 7;
int echoLinks = 6;

int triggerRechts = 4;
int echoRechts = 5;

int triggerHinten = 2;
int echoHinten = 3;

int motorENA = 11;
int motorENB = 10;
int motorIN1 = A0;
int motorIN2 = A1;
int motorIN3 = A2;
int motorIN4 = A3;

int returnwert;

int run = 0;

int maxEntfernung = 200;

int Zeit;

void TurnRight(int time) {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
  analogWrite(motorENA, 100);
  analogWrite(motorENB, 100);
  delay(time);
}

void TurnLeft(int time) {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
  analogWrite(motorENA, 100);
  analogWrite(motorENB, 100);
  delay(time);
}

void Forward() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, HIGH);
  analogWrite(motorENA, 200);
  analogWrite(motorENB, 200);
}

void Backward() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, HIGH);
  digitalWrite(motorIN4, LOW);
  analogWrite(motorENA, 200);
  analogWrite(motorENB, 200);
}

void Stop() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
  analogWrite(motorENA, 0);
  analogWrite(motorENB, 0);
}
void setup() {
  pinMode(motorENA, OUTPUT);
  pinMode(motorENB, OUTPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorIN3, OUTPUT);
  pinMode(motorIN4, OUTPUT);

  pinMode(triggerVorne, OUTPUT);
  pinMode(triggerLinks, OUTPUT);
  pinMode(triggerRechts, OUTPUT);
  pinMode(triggerHinten, OUTPUT);

  pinMode(echoVorne, INPUT);
  pinMode(echoLinks, INPUT);
  pinMode(echoRechts, INPUT);
  pinMode(echoHinten, INPUT);

  Serial.begin(9600);
}

void loop() {
  int entfernungVorne = messeEntfernung(triggerVorne, echoVorne);

  if (entfernungVorne > 20) {
    Forward();
  } else {
    Stop();
    
    //Messe Entfernung Links und Rechts und fahre in die Richtung mit mehr Platz
    int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);
    int entfernungRechts = messeEntfernung(triggerRechts, echoRechts);

    if (entfernungRechts > entfernungLinks) {
      Serial.println("Moving Right");
      TurnRight(480);
      Serial.println("done");
    } else {
      Serial.println("Moving Left");
      TurnLeft(480);
      Serial.println("done");
    }
  }
}

int messeEntfernung(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delay(5);
  digitalWrite(triggerPin, HIGH);
  delay(10);
  digitalWrite(triggerPin, LOW);
  long Zeit = pulseIn(echoPin, HIGH);
  returnwert = (Zeit / 2) * 0.03432;

  return returnwert; //Returnt die Entfernung in cm
}