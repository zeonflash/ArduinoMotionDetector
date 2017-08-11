#include <VirtualWire.h>
#include <EasyTransferVirtualWire.h>

const int ledPin = LED_BUILTIN;                // choose the pin for the LED
const int pirInputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int pirVal = 0;                    // variable for reading the pin status

const int transmit_pin = 10;
const int transmit_en_pin = 3;

//create object
EasyTransferVirtualWire ET;

struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  //Struct can'e be bigger then 26 bytes for VirtualWire version
  int16_t blinks;
  int16_t pause;
};
struct SEND_MOTION_STRUCTURE {
  bool motionOn;
  int16_t id;
};

//give a name to the group of data
//SEND_DATA_STRUCTURE mydata;
SEND_MOTION_STRUCTURE myMotionData;

void setup() {
  //start the library, pass in the data details
  //ET.begin(details(mydata));
  ET.begin(details(myMotionData));

  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);   // Bits per sec

  pinMode(pirInputPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("setup");
  randomSeed(analogRead(0));
}

void loop() {
  Serial.println("Checking...");
  if (CheckMotion()) {
    Serial.print("Motion ");
    Serial.println(String(pirState));
    //this is how you access the variables. [name of the group].[variable name]
    //mydata.blinks = 1;
    //mydata.pause = 2;
    myMotionData.motionOn = pirState;
    myMotionData.id = 1;
    //send the data
    ET.sendData();
  }

  delay(1000);
}

bool CheckMotion() { // returns true if there was a change
  pirVal = digitalRead(pirInputPin);  // read input value
  if (pirVal == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    delay(50);

    if (pirState == LOW) {
      // we have just turned on
      // We only want to print on the output change, not state
      pirState = HIGH;
      return true;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    delay(50);
    if (pirState == HIGH) {
      // we have just turned off
      // We only want to print on the output change, not state
      pirState = LOW;
      return true;
    }
  }
  return false;
}

