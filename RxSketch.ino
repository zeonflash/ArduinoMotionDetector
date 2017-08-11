#include <VirtualWire.h>
#include <EasyTransferVirtualWire.h>

//create object
EasyTransferVirtualWire ET;

//THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
//Struct can't be bigger then 26 bytes for VirtualWire version
struct SEND_MOTION_STRUCTURE {
  bool motionOn;
  int16_t id;
};

// Constants
#define REDPIN 9      // Red RGB pin - digital PWM
#define GREENPIN 10   // Green RGB pin - digital PWM
#define BLUEPIN 11    // Blue RGB pin - digital PWM
#define receive_pin 2  // Digital pin to receive data from RF receiver
#define MAX_LOOP_COUNT 15 // Maximum number of times the LEDs will be flashed before turning off (in case "OFF" message not received from transmitter)

//give a name to the group of data
SEND_MOTION_STRUCTURE myMotionData;

// State of the system.
//  True = motion sensed, should be flashing.
//  False = no motion.  LEDs off.
boolean state = false;
int currentLoop = 0;

void setup() {
  //start the library, pass in the data details
  ET.begin(details(myMotionData));

  // Initialise the IO and ISR
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);   // Bits per sec
  vw_rx_start();       // Start the receiver PLL running

  // Setup the pins
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // Reset the LEDs to an off state
  reset();

  // not needed?
  //randomSeed(analogRead(0));
}

void loop() {
  //check and see if a data packet has come in.
  if (ET.receiveData()) {    
    Serial.print(String(myMotionData.id));
    Serial.print(" - ");
    Serial.print(String(myMotionData.motionOn));

    // if motion sensed, turn the LEDs on
    if (myMotionData.motionOn == true) {
      motionSensed();
    } else { // motion has stopped, reset the LEDs
      reset();
    }
  } else if (state == true) { // haven't received a message since the last ON transmission
    // if the LEDs have flashed the max number of times, reset and turn it all off
    if (currentLoop > MAX_LOOP_COUNT) {
      reset();
    } else { // haven't looped enough times.  Flash again!
      motionSensed();
    }
  }

  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(250);
}

// Motion was sensed.  Flash the LEDs.
void motionSensed() {
  state = true;
  analogWrite(REDPIN, 255);
  delay(200);
  analogWrite(REDPIN, 0);
  delay(200);
  analogWrite(GREENPIN, 255);
  delay(200);
  analogWrite(GREENPIN, 0);
  delay(200);
  analogWrite(BLUEPIN, 255);
  delay(200);
  analogWrite(BLUEPIN, 0);
  delay(200);
  currentLoop++;
  Serial.print(": ");
  Serial.println(String(currentLoop));  
}

// Reset the system to the starting state
void reset() {
  analogWrite(REDPIN, 0);
  analogWrite(GREENPIN, 0);
  analogWrite(BLUEPIN, 0);
  digitalWrite(LED_BUILTIN, LOW);
  state = false;
  currentLoop = 0;
  Serial.println("Reset");
}

