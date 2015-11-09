#include <RFduinoBLE.h>

const int PIRin = 2;

bool newadv = false;

int motion = 0;

void sendStuff(void);

void setup() {
  // do iBeacon advertising
 RFduinoBLE.iBeacon = true;
  
  // override the default iBeacon settings
  uint8_t uuid[16] = {0xE2, 0xC5, 0x6D, 0xB5, 0xDF, 0xFB, 0x48, 0xD2, 0xB0, 0x60, 0xD0, 0xF5, 0xA7, 0x10, 0x96, 0xE0};
  memcpy(RFduinoBLE.iBeaconUUID, uuid, sizeof(RFduinoBLE.iBeaconUUID));
  RFduinoBLE.iBeaconMajor = 1234;
  RFduinoBLE.iBeaconMinor = 5678;
  RFduinoBLE.iBeaconMeasuredPower = 0xC6;
  
  // start the BLE stack
  //RFduinoBLE.begin();

  //Configure pin directions
  pinMode(2, INPUT);
  //RFduino_pinWake(2, HIGH); //Wake device when 2 goes high
}

void loop() {

  //RFduino_ULPDelay(500);
  motion = (int) digitalRead(2);
  sendStuff(motion);
  //pinMode(2, INPUT);
  
  /*if (RFduino_pinWoke(PIRin)){
     RFduinoBLE.end();
     RFduinoBLE.iBeaconMajor = 1111;
     RFduinoBLE.begin(); 
     
     RFduino_ULPDelay(SECONDS(4));
     
     RFduinoBLE.end();
     RFduinoBLE.iBeaconMajor = 0000;
     RFduinoBLE.begin(); 
     
     RFduino_resetPinWake(PIRin);*/
    
  
    
//  }

}

// If RFduinoBLE.begin has been run, and is currently running, you cannot use the GPIO apparently

void sendStuff(int data) {
     RFduinoBLE.end();
     RFduinoBLE.iBeaconMajor = data;
     RFduinoBLE.begin();  
     RFduino_ULPDelay(5000);
     RFduinoBLE.end();
}

void RFduinoBLE_onAdvertisement(bool start)
{
  // turn the green led on if we start advertisement, and turn it
}
