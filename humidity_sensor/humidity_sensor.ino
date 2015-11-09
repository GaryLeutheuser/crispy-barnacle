/*
    Gary Leutheuser, Robert Simon, Robert Short, 2015
    Humidity sensor firmware
    Targets RFD221301
*/

#include <RFduinoBLE.h>

// Set humidity sensor input pin
const int humidityIn = 5;

// Sampling describes if we are currently
// timing a humidity pulse width or not,
// sampling = 1: currently timing
// sampling = 0: not timing
bool sampling = true;

// Stores the timer count accured during
// one humidity pulse width
int readout = 0;

// Timer configuration function
void timer_config(void) {
    
    // Stop timer
    NRF_TIMER2->TASKS_STOP = 1;

    // Set to timer mode (not counter)
     NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
    
    // Set to 16-bit timer
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    
    // f_timer = 16 MHz / (2^PRESCALER)
    // 16/2 = 8 MHz timer frequency, or 125 ns resolution
    NRF_TIMER2->PRESCALER = 1;
    
    // Clear the timer
    NRF_TIMER2->TASKS_CLEAR = 1;
}

// Setup function: runs once
void setup() {
    // Use iBeacon protocol
    RFduinoBLE.iBeacon = true;

    // Start Bluetooth
    RFduinoBLE.begin();

    // Setup timer
    timer_config();

    // Setup humidity pin interrupt
    pinMode(humidityIn, INPUT);
    NRF_GPIOTE->CONFIG[0] =  (1 << GPIOTE_CONFIG_POLARITY_Pos)
	      | (humidityIn << GPIOTE_CONFIG_PSEL_Pos) // using GPIO 5 as input
	      | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);
    NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;

    // Attach interrupt handler
    attachInterrupt(GPIOTE_IRQn, GPIOTE_IRQHandler);
}

// Main loop, repeats forever
void loop() {
    // Low power mode delay between readings;
    // The argument is the number of miliseconds
    // between readings.
    RFduino_ULPDelay(200);

    // Stop Bluetooth
    RFduinoBLE.end();
    // Update the field with new humidity pulse count
    RFduinoBLE.iBeaconMajor = readout;
    // Start Bluetooth
    RFduinoBLE.begin(); 
}

// Interrupt handler
void GPIOTE_IRQHandler(void)
{
  // Event causing the interrupt must be cleared
  if ((NRF_GPIOTE->EVENTS_IN[0] == 1) && (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk)) {
    if (sampling == false) {

      // Start timer on humidity pos edge
      NRF_TIMER2->TASKS_START = 1;
      
      // Set sampling flag
      sampling = true;    
    } else {
     
      //Stop Timer
      NRF_TIMER2->TASKS_STOP = 1;

      //read counter value
      NRF_TIMER2->TASKS_CAPTURE[0] = 1;
      readout = NRF_TIMER2->CC[0];
      
      // Clear the timer
      NRF_TIMER2->TASKS_CLEAR = 1;
      
      // Reset sampling flag
      sampling = false;
    }
   
    NRF_GPIOTE->EVENTS_IN[0] = 0;
  }
}
