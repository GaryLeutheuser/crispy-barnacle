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
