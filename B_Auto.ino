#include <Arduino.h>

// Pinbelegung
const int triggerVorne = 8;
const int echoVorne = 9;

const int triggerLinks = 7;
const int echoLinks = 6;

const int triggerRechts = 4;
const int echoRechts = 5;

const int triggerHinten = 2;
const int echoHinten = 3;

const int motorENA = 11;
const int motorENB = 10;
const int motorIN1 = A0;
const int motorIN2 = A1;
const int motorIN3 = A2;
const int motorIN4 = A3;

// Konstanten
const int normale_geschwindigkeit = 100;
const int schnell_geschwindigkeit = 150;
const int rueckwaerts_geschwindigkeit = 60;

// Statusvariablen
bool debug;

// Variablen
int drehung = 480;
int returnwert;

int maxEntfernung = 200;



// Funktion zum Debuggen
void print(String text) {
  if (debug) {
    Serial.println(text);
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

  Serial.begin(9600);

  TurnRight(10000);

  debug = false;  //Debugging aktivieren/deaktivieren
}

void loop() {
  int decision = driveDecision();
  print("Decision: " + String(decision));
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
      Stop();
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
  if (entfernungVorne > 20) {
    return 1;
  } else {
    // !KEIN ELSE IF! Da das sonst die Leistung stark beeinträchtigt wird, da alle Sensoren gemessen werden müssen!
    int entfernungLinks = messeEntfernung(triggerLinks, echoLinks);
    int entfernungRechts = messeEntfernung(triggerRechts, echoRechts);
    if (entfernungLinks > 30 || entfernungRechts > 30) {
      //Fahre in die Richtung mit mehr Platz
      if (entfernungLinks > entfernungRechts) {
        return 3;
      } else {
        return 4;
      }
    } else {
      int entfernungHinten = messeEntfernung(triggerHinten, echoHinten);
      if (entfernungHinten > 20) {
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

  return returnwert;  //Returnt die Entfernung in cm
}

