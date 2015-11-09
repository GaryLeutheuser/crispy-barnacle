#include <RFduinoBLE.h>
#include <Wire.h>

const int smokeLEDCtrl = 3;
const int smokeIn = 5;
const int threshold = 100;

bool newadv = false;

int readout = 0;

void setup() {
  // do iBeacon advertising
 /* RFduinoBLE.iBeacon = true;
  
  // override the default iBeacon settings
  uint8_t uuid[16] = {0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0};
  memcpy(RFduinoBLE.iBeaconUUID, uuid, sizeof(RFduinoBLE.iBeaconUUID));
  RFduinoBLE.iBeaconMajor = 1234;
  RFduinoBLE.iBeaconMinor = 5678;
  RFduinoBLE.iBeaconMeasuredPower = 0xC6;
  
  // start the BLE stack
  RFduinoBLE.begin();
*/
  //Configure pin directions
  pinMode(smokeLEDCtrl, OUTPUT);
  pinMode(smokeIn, INPUT);

  Serial.begin(9600);
}

void loop() {

  RFduino_ULPDelay(250);
  
  // turn on LED
  digitalWrite(smokeLEDCtrl, HIGH);
  
  delay(50);
  
  // Read out smoke sensor  
  readout = analogRead(smokeIn);

  delay(50);

  //Turn off the LED
  digitalWrite(smokeLEDCtrl, LOW);
  
  // if we dont see smoke, sleep for a while

  Serial.println(readout);
  // if we do see smoke, wait a bit, then check again

  // if we see smoke for sure, send out an alert for 10 seconds then reset

}

void RFduinoBLE_onAdvertisement(bool start)
{
  // turn the green led on if we start advertisement, and turn it
  // off if we stop advertisement
  
  newadv = true;
}
