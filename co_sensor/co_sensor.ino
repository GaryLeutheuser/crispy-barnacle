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

#define RESISTANCE_100PPM   3000 // Coil resistance in 100 PPM CO

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

    // Holds calculated current resistance of coil
    float resistance = 0;
    // Holds ratio of current resistance to 100 PPM resistance
    float resistance_ratio = 0;
    // Holds current CO level in PPM
    int co_ppm = 0;

    //Drive the coil through its cycle
    digitalWrite(PIN_CO_CTRL_N, LOW);
    delay(SECONDS(TIME_COIL_HIGH));
    analogWrite(PIN_CO_CTRL_N, pwmVoltage(1.4));
    delay(SECONDS(TIME_COIL_LOW));

    // Take a measurement, and wait to start the cycle over again
    carbon_monoxide = analogRead(PIN_CO_IN);

    // Calculate the CO concentration in PPM
    resistance = 10000*(1024-carbon_monoxide)/ ((float) carbon_monoxide);
    
    resistance_ratio = resistance/RESISTANCE_100PPM;

    if (resistance_ratio > 4) {
	co_ppm = 10;
    } else if (resistance_ratio > 2) {
	co_ppm = 50;
    } else if (resistance_ratio > 1) {
	co_ppm = 100;
    } else if (resistance_ratio > 0.4) {
	co_ppm = 400;
    } else if (resistance_ratio > 0.2) {
	co_ppm = 1000;
    } else if (resistance_ratio > 0.1) {
	co_ppm = 4000;
    }

    updateData(co_ppm, SECONDS(MEASUREMENT_DELAY));
}
