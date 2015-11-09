/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    Smoke sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>
#include "RFduinoCustoms.h"

#define PIN_SMOKE_CTRL	3   // Control signal for driving the IR LED
#define PIN_SMOKE_IN	5   // Amplified photodiode voltage - input to RFduino
#define SENSOR_ID	1   // For use in minor field to ID the sensor board
#define THRESHOLD	100 // Threshold value for smoke level - how much indicates smoke is present?

int readout = 0;

void setup() {
  // do iBeacon advertising
  RFduinoBLE.iBeacon = true;
  // Set minor field for identification
  RFduinoBLE.iBeaconMinor = SENSOR_ID;
  
  //Configure pin directions
  pinMode(PIN_SMOKE_CTRL, OUTPUT);
  pinMode(PIN_SMOKE_IN, INPUT);
}

void loop() {

  RFduino_ULPDelay(250);
  
  // turn on LED
  digitalWrite(PIN_SMOKE_CTRL, HIGH);
  
  delay(50);
  
  // Read out smoke sensor  
  readout = analogRead(PIN_SMOKE_IN);

  delay(50);

  //Turn off the LED
  digitalWrite(PIN_SMOKE_CTRL, LOW);
}
