/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    Carbon Monoxide sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>
#include "RFduinoCustoms.h"

#define SENSOR_ID   2	// For use in minor field to ID the sensor board

void setup() {
    // iBeacon advertising
    RFduinoBLE.iBeacon = true;
    RFduinoBLE.iBeaconMinor = SENSOR_ID;
}

void loop() {

}
