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

#define LED_ON_TIME	    10	// Time that LED should be left on to "charge" photodiode
#define MEASUREMENT_DELAY   250 // Delay in ms between measurements
#define NUM_AVG		    3	// Number of samples to average to create one sample
#define NUM_SAMPLES	    3	// Number of averaged samples to take in a sampling period

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
    
    int i, j;
    
    int smoke_sum = 0;
    int averaged_sample = 0;

    for (j = 0; j < NUM_SAMPLES; j++) {
	
	for (i = 0; i < NUM_AVG; i++) {
	    // Turn on LED
	    digitalWrite(PIN_SMOKE_CTRL, HIGH);
	    delay(LED_ON_TIME);

	    // Read smoke sensor output
	    smoke = analogRead(PIN_SMOKE_IN);
	    
	    // Sum for averaging later
	    smoke_sum += smoke;

	    // Turn off the LED
	    digitalWrite(PIN_SMOKE_CTRL, LOW);
	}

    // Create the average
    averaged_sample = smoke_sum/NUM_AVG;

    // Broadcast averaged sample
    updateData(averaged_sample, 30);

    }
    
    // Delay until next sampling period
    delay(MEASUREMENT_DELAY);
}
