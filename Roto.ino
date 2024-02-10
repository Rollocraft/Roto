#include <Arduino.h>

// Pinbelegung
// Trigger und Echo Pins
const int triggerVorne = 8;
const int echoVorne = 9;

const int triggerLinks = 7;
const int echoLinks = 6;

const int triggerRechts = 4;
const int echoRechts = 5;

const int triggerHinten = 2;
const int echoHinten = 3;

// Motor Pins
const int motorENA = 11;
const int motorENB = 10;
const int motorIN1 = A0;
const int motorIN2 = A1;
const int motorIN3 = A2;
const int motorIN4 = A3;

// LED Pins
const int WeiseLED = A4;
const int GelbeLEDLinks = 12;
const int GelbeLEDRechts = 0;
const int RoteLED = 1;

// Sensoren
const int LDR = A5;

//Piezo
const int piezo = 13;

// Konstanten um den Ram zu "schonen"
const int normale_geschwindigkeit = 100;
const int schnell_geschwindigkeit = 150;
const int rueckwaerts_geschwindigkeit = 60;
const int maxEntfernung = 10;
const int drehung = 480;

// Statusvariablen
bool debug;

// Variablen
int returnwert;
int run;
int entfernungLinks;
int entfernungLinksOld;

// Funktion zum Debuggen
void print(String text) {
  if (debug) {
    Serial.println(text);
  }
}

void LDRMessung() {
  int LDR = analogRead(A5);
  print("LDR: " + String(LDR));
  if (LDR < 10) {
    digitalWrite(WeiseLED, HIGH);
  } else {
    digitalWrite(WeiseLED, LOW);
  }
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

  debug = false;  //Debugging aktivieren/deaktivieren

  if (debug) {
    Serial.begin(9600);
  }
   int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);
   entfernungLinksOld = entfernungLinks;
}

bool wait(int time) { // wartet für eine bestimmte Zeit
  static unsigned long startTime = 0;
  if (startTime == 0) {
    startTime = millis();
  }
  if (millis() - startTime >= time) {
    startTime = 0;
    return true;
  }
  return false;
}

void loop() {
  LDRMessung();
  int decision = driveDecision(); // entscheide aufgrund der Funktion wie gefahren werden soll
  print("Decision: " + String(decision));
  if(run > 8) { // Um eine Schrägfahrt zu vermeiden
    run = 0;
    int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);
    if(entfernungLinksOld > entfernungLinks) {
      TurnLeft(10);
    } else if(entfernungLinksOld < entfernungLinks) {
      TurnRight(10);
    } else {
    }
  }
  switch (decision) {
    case 1:
      Forward(normale_geschwindigkeit);
      break;
    case 2:
      Forward(schnell_geschwindigkeit);
      break;
    case 3:
      TurnLeft(drehung);
      break;
    case 4:
      TurnRight(drehung);
      break;
    case 5:
      Backward(rueckwaerts_geschwindigkeit);
      break;
    case 6:
      digitalWrite(RoteLED, HIGH);
      if (wait(250)) {
        digitalWrite(RoteLED, LOW);
      }
      Stop();
      tone(piezo, 439);
      delay(100);
      tone(piezo, 539);
      delay(100);
      tone(piezo, 439);
      delay(100);
      noTone(piezo);
      break;
  }
}

int driveDecision() {
  /*
  0: Fehler 
  1: Forwärts 
  2: Forwärts Schnell
  3: Links 
  4: Rechts
  5: Rückwärts
  6: Stop
  */
  int entfernungVorne = messeEntfernung(triggerVorne, echoVorne);
  run++;
  if (entfernungVorne > (maxEntfernung*5)) {
    if (entfernungVorne > maxEntfernung) {
      return 1;
    }
    return 2;
  } else {
    // !KEIN ELSE IF! Da das sonst die Leistung stark beeinträchtigt wird, da alle Sensoren direkt gemessen werden müssen!
    int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);
    int entfernungRechts = messeEntfernung(triggerRechts, echoRechts);
    if (entfernungLinks > (maxEntfernung*2) || entfernungRechts > (maxEntfernung*2)) {
      //Fahre in die Richtung mit mehr Platz
      if (entfernungLinks > entfernungRechts) {
        return 3;
      } else {
        return 4;
      }
    } else {
      int entfernungHinten = messeEntfernung(triggerHinten, echoHinten);
      if (entfernungHinten > maxEntfernung) {
        return 5;
      } else {
        return 6;
      }
    }
    return 0;
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
  print("Entfernung: " + String(returnwert));

  return returnwert;  //Returnt die Entfernung in cm
}