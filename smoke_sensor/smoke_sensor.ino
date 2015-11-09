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

#define LED_ON_TIME	    50	// Time that LED should be left on to "charge" photodiode
#define MEASUREMENT_DELAY   500 // Delay in ms between measurements

int smoke = 0;

void setup() {
    // Do iBeacon advertising
    RFduinoBLE.iBeacon = true;
    // Set minor field for identification
    RFduinoBLE.iBeaconMinor = SENSOR_ID;

    // Configure pin directions
    pinMode(PIN_SMOKE_CTRL, OUTPUT);
    pinMode(PIN_SMOKE_IN, INPUT);
}

void loop() {
    // Turn on LED for 50 ms
    digitalWrite(PIN_SMOKE_CTRL, HIGH);
    delay(LED_ON_TIME);

    // Read smoke sensor output
    smoke = analogRead(PIN_SMOKE_IN);

    // Turn off the LED
    digitalWrite(PIN_SMOKE_CTRL, LOW);

    // Broadcast smoke value and delay until
    // next measurement
    updateData(smoke, MEASUREMENT_DELAY);
}
