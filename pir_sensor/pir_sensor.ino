/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    PIR sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>
#include "RFduinoCustoms.h"

#define MEASUREMENT_PERIOD  500	    // Value in ms to be delayed between measurements
#define PIN_PIR		    2	    // Defines which pin the PIR input is mapped to
 
// Variable to hold detection of motion
int motion = 0;

// Updates iBeacon major field value
void updateData(void);	

void setup() {
    // do iBeacon advertising
    RFduinoBLE.iBeacon = true;
    // Set Minor field for identification
    RFduinoBLE.iBeaconMinor = 0;

    //Configure pin directions
    pinMode(PIN_PIR, INPUT);
}

void loop() {
    // Read and update motion value (discrete from PIR)
    // motion = 1: motion detected
    // motion = 0: motion not detected
    motion = (int) digitalRead(PIN_PIR);
    
    // Update the major field with the current
    // motion value
    updateData(motion, MEASUREMENT_PERIOD);
}
