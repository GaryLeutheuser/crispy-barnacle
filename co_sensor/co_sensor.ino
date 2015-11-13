/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    Carbon Monoxide sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>
#include "RFduinoCustoms.h"

#define PIN_CO_CTRL_N 3	// Control signal for driving the heater coil
#define PIN_CO_IN     5	// Resistive element voltage - input to RFduino
#define SENSOR_ID     2	// For use in minor field to ID the sensor board

#define TIME_COIL_HIGH	    60	// Amount of time the coil should spend high, in seconds
#define TIME_COIL_LOW	    90	// Amount of time the coil should spend low, in seconds
#define MEASUREMENT_DELAY   0	// Amount of time to wait before starting next measurement cycle, in seconds

int carbon_monoxide = 0;

void setup() {
    // iBeacon advertising
    RFduinoBLE.iBeacon = true;
    RFduinoBLE.iBeaconMinor = SENSOR_ID;

    // Configure pin directions
    pinMode(PIN_CO_CTRL_N, OUTPUT);
    pinMode(PIN_CO_IN, INPUT);
}

void loop() {
    //Drive the coil through its cycle
    digitalWrite(PIN_CO_CTRL_N, LOW);
    delay(SECONDS(TIME_COIL_HIGH));
    analogWrite(PIN_CO_CTRL_N, pwmVoltage(1.4));
    delay(SECONDS(TIME_COIL_LOW));

    // Take a measurement, and wait to start the cycle over again
    carbon_monoxide = analogRead(PIN_CO_IN);
    updateData(carbon_monoxide, SECONDS(MEASUREMENT_DELAY));
}
