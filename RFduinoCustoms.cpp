#include "RFduinoCustoms.h"
#include <RFduinoBLE.h>
#include <Arduino.h>

void updateData(int data, int time_delay) {
    // BLE stack cannot be running while updating value
    RFduinoBLE.end();
    RFduinoBLE.iBeaconMajor = data;
    RFduinoBLE.begin();

    // Allow at least 25 ms here
    RFduino_ULPDelay(time_delay);
}

// Produces a value of "voltage" on a 5V output (mapped
// from 3.3V)
unsigned char pwmVoltage(float voltage) {
    // Map 0-5 volt value down to 0-3.3 volt
    float mappedTo33 = voltage * (3.3/5.0);
    // Map 0-3.3 volt value to 0-255 value
    unsigned char mappedTo255 = (unsigned char) ( mappedTo33 / (3.3/255.0));
    // Subtract from 255 to handle the inverted nature of PFET
    return (255 - mappedTo255);
}
