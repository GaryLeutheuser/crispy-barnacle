/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    Carbon Monoxide sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>
#include "RFduinoCustoms.h"

void setup() {
    // iBeacon advertising
    RFduinoBLE.iBeacon = true;
    RFduinoBLE.iBeaconMinor = 2;
}

void loop() {

}
