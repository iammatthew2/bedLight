#include <SPI.h>
#include "pitches.h"

const int upperBrightSwitchPin =7;
const int lowerBrightSwitchPin = 2;
const int upperBrightLedArray = 9;
const int lowerBrightLedArray = 10;
const int discoButtonPin = 4;
const int discoButtonLed = 5;
const int piezoPin = 6;

unsigned long upperBrightLedToggleTime = 0;
unsigned long lowerBrightLedToggleTime = 0;
unsigned long discoBallToggleTime = 0;

unsigned long currentTime = 0;

int upperBrightSwitchState = 0;
int upperPrevBrightSwitchState = 0;
int upperBrightLedState = 0;

int lowerBrightSwitchState = 0;
int lowerPrevBrightSwitchState = 0;
int lowerBrightLedState = 0;

long bottomBrightInterval = 900000; // 15 miuntes
long topBrightInterval = 3600000; // 60 miuntes
long discoInterval = 420000; // 7 minutes

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  pinMode(upperBrightLedArray, OUTPUT);
  pinMode(lowerBrightLedArray, OUTPUT);
  pinMode(upperBrightSwitchPin, INPUT);
  pinMode(lowerBrightSwitchPin, INPUT);
  pinMode(discoButtonPin, INPUT);
  pinMode(discoButtonLed, OUTPUT);
}

void toggleUpperLight(bool forceOff = false) {
  if (upperBrightLedState == 1 || forceOff) {
    digitalWrite(upperBrightLedArray, LOW);
    upperBrightLedState = 0;
  } else {
    digitalWrite(upperBrightLedArray, HIGH);
    upperBrightLedState = 1;
  }
  upperBrightLedToggleTime = currentTime;
  upperPrevBrightSwitchState = upperBrightSwitchState;
}


void toggleLowerLight(bool forceOff = false) {
  if (lowerBrightLedState == 1 || forceOff) {
    digitalWrite(lowerBrightLedArray, LOW);
    lowerBrightLedState = 0;
  } else {
    digitalWrite(lowerBrightLedArray, HIGH);
    lowerBrightLedState = 1;
  }
  lowerBrightLedToggleTime = currentTime;
  lowerPrevBrightSwitchState = lowerBrightSwitchState;
}


void loop() {
  bool forceOff = true;
  currentTime = millis();

  // handle upper light
  if (currentTime - upperBrightLedToggleTime > topBrightInterval) {
    toggleUpperLight(forceOff);
  }

  upperBrightSwitchState = digitalRead(upperBrightSwitchPin);

  if (upperBrightSwitchState != upperPrevBrightSwitchState) {
    toggleUpperLight();
  }


  // handle lower light
  if (currentTime - lowerBrightLedToggleTime > bottomBrightInterval) {
    toggleLowerLight(forceOff);
  }

  lowerBrightSwitchState = digitalRead(lowerBrightSwitchPin);

  if (lowerBrightSwitchState != lowerPrevBrightSwitchState) {
    toggleLowerLight();
  }


  //handle the blue light button
  if (digitalRead(discoButtonPin) == HIGH) {
    digitalWrite(discoButtonLed, HIGH);
    delay(500);

//    tone(piezoPin, 800, 1000);

    for (int thisNote = 0; thisNote < 8; thisNote++) {

      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(piezoPin, melody[thisNote], noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(piezoPin);
    }
    digitalWrite(discoButtonLed, LOW);
  } 
}
