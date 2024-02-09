// Variablen

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

int LDR = A5;

int LedWeiss = A4;
int LedGelb = 13;
int LedRot = 12;

int piezo = 11;

int entfernungLinksOld;
int entfernungLinks;
int entfernungHinten;
int entfernungRechts;
int entfernungVorne;

int returnwert;

int run;

int maxEntfernung = 200;

int Zeit;

int turn = 620;
int turn_dif = 545;

// setup

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

  pinMode(LDR, INPUT);

  pinMode(piezo, OUTPUT);

  pinMode(LedWeiss, OUTPUT);
  pinMode(LedGelb, OUTPUT);
  pinMode(LedRot, OUTPUT);

  Serial.begin(9600);
}

void forward() {}

void backward() {}

void turnRight(int time) {}

void turnLeft(int time) {}

void stop() {}

void LDRM() {
  int LDR_Wert = analogRead(LDR);

  if (!LDR_Wert > 400) {
    digitalWrite(LedWeiss, HIGH);
  } else {
    digitalWrite(LedWeiss, LOW);
  }
}


void loop() {

  LDRM();

  int entfernungVorne = messeEntfernung(triggerVorne, echoVorne);
  int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);

  if (entfernungVorne > 20) {
    if (entfernungLinks != entfernungLinksOld) {
      if (entfernungLinks > entfernungLinksOld) {
        Serial.println("Moving Left");
        digitalWrite(motorIN1, LOW);
        digitalWrite(motorIN2, HIGH);
        digitalWrite(motorIN3, HIGH);
        digitalWrite(motorIN4, LOW);
        analogWrite(motorENA, 100);
        analogWrite(motorENB, 100);
        delay(12);
        Serial.println("done");
      } else {
        Serial.println("Moving Right");
        digitalWrite(motorIN1, HIGH);
        digitalWrite(motorIN2, LOW);
        digitalWrite(motorIN3, LOW);
        digitalWrite(motorIN4, HIGH);
        analogWrite(motorENA, 100);
        analogWrite(motorENB, 100);
        delay(12);
        Serial.println("done");
      }
      digitalWrite(motorIN1, LOW);
      digitalWrite(motorIN2, HIGH);
      digitalWrite(motorIN3, LOW);
      digitalWrite(motorIN4, HIGH);
      analogWrite(motorENA, 200);
      analogWrite(motorENB, 200);

      entfernungLinksOld = entfernungLinks;
    }
  } else if (entfernungRechts > 30 || entfernungLinks > 30) {

    digitalWrite(LedGelb, HIGH);
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, LOW);
    digitalWrite(motorIN3, LOW);
    digitalWrite(motorIN4, LOW);
    analogWrite(motorENA, 0);
    analogWrite(motorENB, 0);


    int entfernungRechts = messeEntfernung(triggerRechts, echoRechts);
    int entfernungHinten = messeEntfernung(triggerHinten, echoHinten);

    if (entfernungRechts > entfernungLinks) {
      if (!run > 4) {
        run = run++;
        Serial.println("Moving Right");
        digitalWrite(motorIN1, HIGH);
        digitalWrite(motorIN2, LOW);
        digitalWrite(motorIN3, LOW);
        digitalWrite(motorIN4, HIGH);
        analogWrite(motorENA, 100);
        analogWrite(motorENB, 100);
        delay(turn);
        Serial.println("done");
      } else {
        Serial.println("Moving Right");
        digitalWrite(motorIN1, HIGH);
        digitalWrite(motorIN2, LOW);
        digitalWrite(motorIN3, LOW);
        digitalWrite(motorIN4, HIGH);
        analogWrite(motorENA, 100);
        analogWrite(motorENB, 100);
        delay(turn_dif);
        Serial.println("done");
        run = 0;
      }
    } else {
      if (!run > 4) {
        run = run++;
        Serial.println("Moving Left");
        digitalWrite(motorIN1, LOW);
        digitalWrite(motorIN2, HIGH);
        digitalWrite(motorIN3, HIGH);
        digitalWrite(motorIN4, LOW);
        analogWrite(motorENA, 100);
        analogWrite(motorENB, 100);
        delay(turn);
        Serial.println("done");
      } else {
        Serial.println("Moving Left");
        digitalWrite(motorIN1, HIGH);
        digitalWrite(motorIN2, LOW);
        digitalWrite(motorIN3, LOW);
        digitalWrite(motorIN4, HIGH);
        analogWrite(motorENA, 100);
        analogWrite(motorENB, 100);
        delay(turn_dif);
        Serial.println("done");
        run = 0;
      }
    }

    digitalWrite(LedGelb, LOW);

    int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);

    entfernungLinksOld = entfernungLinks;

  } else if (entfernungHinten > 20) {


      digitalWrite(motorIN1, HIGH);
      digitalWrite(motorIN2, LOW);
      digitalWrite(motorIN3, HIGH);
      digitalWrite(motorIN4, LOW);
      analogWrite(motorENA, 200);
      analogWrite(motorENB, 200);
    }

  else {
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, LOW);
    digitalWrite(motorIN3, LOW);
    digitalWrite(motorIN4, LOW);
    analogWrite(motorENA, 0);
    analogWrite(motorENB, 0);
    tone(piezo, 750);
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
    return returnwert;
  }