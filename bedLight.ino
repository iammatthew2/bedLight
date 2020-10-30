#include <SPI.h>
#include "pitches.h"

const int upperBrightSwitchPin =7;
const int lowerBrightSwitchPin = 2;
const int upperBrightLedArray = 9;
const int lowerBrightLedArray = 10;
const int blueButtonPin = 4;
const int blueButtonLed = 5;
const int buzzer = 6;


unsigned long upperBrightLedToggleTime = 0;
unsigned long lowerBrightLedToggleTime = 0;

unsigned long currentTime = 0;

int upperBrightSwitchState = 0;
int upperPrevBrightSwitchState = 0;
int upperBrightLedState = 0;

int lowerBrightSwitchState = 0;
int lowerPrevBrightSwitchState = 0;
int lowerBrightLedState = 0;

long bottomBrightInterval = 900000; // 15 miuntes
long topBrightInterval = 3600000; // 60 miuntes


// beging music stuff
int tempo = 140;
int melody[] = {
// grabbed from: https://github.com/robsoncouto/arduino-songs/blob/master/takeonme/takeonme.ino
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
// end music stuff


void setup() {
  pinMode(upperBrightLedArray, OUTPUT);
  pinMode(lowerBrightLedArray, OUTPUT);
  pinMode(upperBrightSwitchPin, INPUT);
  pinMode(lowerBrightSwitchPin, INPUT);
  pinMode(blueButtonPin, INPUT);
  pinMode(blueButtonLed, OUTPUT);
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
  if (digitalRead(blueButtonPin) == HIGH) {
    digitalWrite(blueButtonLed, HIGH);

    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
      // calculates the duration of each note
      divider = melody[thisNote + 1];
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }
      // we only play the note for 90% of the duration, leaving 10% as a pause
      tone(buzzer, melody[thisNote], noteDuration*0.9);
      // Wait for the specief duration before playing the next note.
      delay(noteDuration);
      // stop the waveform generation before the next note.
      noTone(buzzer);
    }
    digitalWrite(blueButtonLed, LOW);
  } 
}
