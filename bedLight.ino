#include <SPI.h>

const int upperBrightSwitchPin = 3;
const int lowerBrightSwitchPin = 2;
const int upperBrightLedArray = 11;
const int lowerBrightLedArray = 10;

unsigned long upperBrightLedToggleTime = 0;
unsigned long lowerBrightLedToggleTime = 0;

unsigned long currentTime = 0;

int upperBrightSwitchState = 0;
int upperPrevBrightSwitchState = 0;
int upperBrightLedState = 0; //light is off if 0;

int lowerBrightSwitchState = 0;
int lowerPrevBrightSwitchState = 0;
int lowerBrightLedState = 0; //light is off if 0;

long brightInterval = 900000;

void setup() {
  pinMode(upperBrightLedArray, OUTPUT);
  pinMode(lowerBrightLedArray, OUTPUT);
  pinMode(upperBrightSwitchPin, INPUT);
  pinMode(lowerBrightSwitchPin, INPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("ready to log stuff");
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
  if (currentTime - upperBrightLedToggleTime > brightInterval) {
    toggleUpperLight(forceOff);
  }

  upperBrightSwitchState = digitalRead(upperBrightSwitchPin);

  if (upperBrightSwitchState != upperPrevBrightSwitchState) {
    toggleUpperLight();
  }

  // handle lower light
  if (currentTime - lowerBrightLedToggleTime > brightInterval) {
    toggleLowerLight(forceOff);
  }

  lowerBrightSwitchState = digitalRead(lowerBrightSwitchPin);

  if (lowerBrightSwitchState != lowerPrevBrightSwitchState) {
    toggleLowerLight();
  }
}
