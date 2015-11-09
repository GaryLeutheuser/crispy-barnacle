/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    PIR sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>

#define MEASUREMENT_PERIOD  500	    // Value in ms to be delayed between measurements
#define PIN_PIR		    2	    // Defines which pin the PIR input is mapped to
 
// Variable to hold detection of motion
int motion = 0;

// Updates iBeacon major field value
void updateData(void);	

void setup() {
    // Set UUID for identification
    uint8_t uuid[16] = {0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0};
    memcpy(RFduinoBLE.iBeaconUUID, uuid, sizeof(RFduinoBLE.iBeaconUUID));

    // do iBeacon advertising
    RFduinoBLE.iBeacon = true;

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

// Updates the beacon value for broadcasting
void updateData(int data, int time_delay) {
    // BLE Stack cannot be running while updating value
    RFduinoBLE.end();
    RFduinoBLE.iBeaconMajor = data;
    RFduinoBLE.begin();

    // Allow at least 25 ms for value to broadcast
    RFduino_ULPDelay(time_delay);
}
