//https://github.com/finnurtorfa/nrf51/blob/master/lib/nrf51sdk/Nordic/nrf51822/Board/nrf6310/timer_example/main.c

#include <RFduinoBLE.h>
#include <Wire.h>

const int ledPinGreen = 3;
const int humidityIn = 5;

int ledState = LOW;
bool newadv = false;
bool sampling = true;
int readout = 0;

void timer_config(void)
{
  NRF_TIMER2->TASKS_STOP = 1;  // Stop timer
  NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;  // taken from Nordic dev zone
  NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
  NRF_TIMER2->PRESCALER = 1;  // 32us resolution
  NRF_TIMER2->TASKS_CLEAR = 1; // Clear timer
  
  // With 32 us ticks, we need to multiply by 31.25 to get milliseconds
  //NRF_TIMER2->CC[0] = 7;
  //NRF_TIMER2->CC[0] += number_of_ms / 4;
  //NRF_TIMER2->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;  // taken from Nordic dev zone
  //NRF_TIMER2->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
  //attachInterrupt(TIMER2_IRQn, TIMER2_Interrupt);    // also used in variant.cpp to configure the RTC1
  //NRF_TIMER2->TASKS_START = 1;  // Start TIMER
}

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
  RFduinoBLE.begin();
  
  //setup and start the timer
  
  //Serial.println("LOLSIMON");
  timer_config();
 
  pinMode(ledPinGreen, OUTPUT);
  pinMode(humidityIn, INPUT);

  //Set up GPIO Settings
  //NVIC_EnableIRQ(GPIOTE_IRQn);
  NRF_GPIOTE->CONFIG[0] =  (1 << GPIOTE_CONFIG_POLARITY_Pos)
              | (humidityIn << GPIOTE_CONFIG_PSEL_Pos) // using GPIO 5 as input
              | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);
  NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;
  attachInterrupt(GPIOTE_IRQn, GPIOTE_IRQHandler);

  //Serial.begin(9600);
  
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
 RFduino_ULPDelay(200);
 if (newadv){
 RFduinoBLE.end();
 RFduinoBLE.iBeaconMajor = readout;
 newadv = false;
 RFduinoBLE.begin(); 
 }
  //Do something in here with your I2C device

}

void RFduinoBLE_onAdvertisement(bool start)
{
  // turn the green led on if we start advertisement, and turn it
  // off if we stop advertisement
  
  newadv = true;
}

void GPIOTE_IRQHandler(void)
{
  // Event causing the interrupt must be cleared
  if ((NRF_GPIOTE->EVENTS_IN[0] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))
  {
    if(sampling == false){
      //When we get a humidity sensor pos edge start timer
      NRF_TIMER2->TASKS_START = 1;  // Start TIMER
      sampling = true;
    }
    
    else{
      //Stop Timer
      NRF_TIMER2->TASKS_STOP = 1;
      //read counter value
      NRF_TIMER2->TASKS_CAPTURE[0] = 1;
      
      readout = NRF_TIMER2->CC[0];
      
      
      //Serial.begin(9600);
      //Serial.println(readout, DEC);
      
      NRF_TIMER2->TASKS_CLEAR = 1;
      
      //Test: Toggle GPIO3 to test if this function is entered
      digitalWrite(ledPinGreen, HIGH);
      //delay(250);
      digitalWrite(ledPinGreen, LOW);

      sampling = false;
      
    }
    //Serial.println(NRF_TIMER2->CC[0], DEC);
   
    NRF_GPIOTE->EVENTS_IN[0] = 0;
  }
  //DO SOMETHING HERE WHEN PIN CHANGES STATES
}

