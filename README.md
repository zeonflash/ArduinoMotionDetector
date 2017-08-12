# Arduino Motion Detector

## Description
An Arduino Nano board (remote) with a PIR motion sensor and RF transmitter alerts a second Arduino (base station) via an RF receiver, which flashes an LED strip to alert that motion is detected.

For this project I chose two Arduino Nano boards, although Uno boards were used for prototyping.  Use whatever makes the most sense for your situation.

## Parts
### Remote station
1. [Arduino Nano](http://a.co/3v95r7O)
1. [PIR motion sensor](http://a.co/aFcuxUe)
1. [RF transmitter](http://a.co/1EtcDh4)
1. [Toggle switch](http://a.co/aFcuxUe)
1. 9V battery
1. [9V battery connector](http://a.co/9nJ0lZJ)

### Base station
1. [Arduino Nano](http://a.co/3v95r7O)
1. [RF receiver](http://a.co/1EtcDh4)
1. 3x [MOSFET (N-channel)](http://a.co/irDUT6u)
1. 3x 10K resistors
1. [LED strip (5050)](http://a.co/0xbpgEF)
1. [DC power jack](http://a.co/hLfMvS4)
1. DC power converter (12V, <= ~1A)

### Software
1. [Arduino IDE](https://www.arduino.cc/en/Main/Software)
1. [VirtualWire library](https://www.pjrc.com/teensy/td_libs_VirtualWire.html)
1. [EasyTransfer library](https://github.com/madsci1016/Arduino-EasyTransfer/tree/master/EasyTransferVirtualWire)

## Creating the motion detector
The remote station has two functions:  detect motion and transmit data to the base station.  Start by attaching the PIR motion sensor GND pin to the ground rail and the VCC pin to the power rail.  The DATA pin is connected to a digital (D) pin.  Doesn't matter which one.

Next, hook up the RF transmitter.  GND to the ground rail, VCC to the power rail, and DATA (or ATAD) to a different digital pin.  An antenna can be attached to boost the signal.  One quick and easy antenna can be made by wrapping some wire around a pen (5-8 rotations) with one end wired to the transmitter.

Now to setup the Nano.  Attach the 5V pin to the power rail and the GND to the ground rail.  With the 9V battery connector, attach power (red) wire to the VIN and the ground (black) to the GND next to the VIN.  Optionally, cut the power wire on the connector and splice in the toggle switch.  This will allow you to turn off the system to conserve power.

Open the Tx script in the Arduino IDE.  Verify that the pin you designated for the PIR DATA pin and the RF Transmitter output pin are correct.  Upload the Tx script to the Arduino.  Note that you'll have to install the two libraries first in order to include them in the sketch.  I had a particularly difficult time getting VirtualWire to work on Mac, but had no issues on Win10.  

To check if everything is working, hook up the battery, toggle the switch on (if used), and wait 30-60 seconds.  This will give the sensor time to get acclimated to the light in the room.  Afterwards, wave your hand near the motion sensor and you should see the onboard LED light up.  After a few seconds, the LED should turn off.

## Creating the base station
The base station will be responsible for driving the LED strip when the RF receiver is triggered.  Here we use MOSFETs in conjunction with the PWM pins on the Arduino to signal which colors are displayed.

The RF receiver is hooked up similar to the RF transmitter.  GND to the ground rail, DATA to a digital pin (e.g., D2).  Since the LED strip uses a 12V power source and the receiver uses 5V, don't hook the VCC to the power rail.  Instead, wire it directly to the 5V in on the Arduino.

Insert the 3 MOSFETs onto a board with a 10K resistor inline with the leftmost ("gate") pin.  This is the pin that determines if the circuit is open or closed.  Inline before each resistor, connect a digital PWM (usually denoted with ~) pin, such as D9-D11.  Connect the rightmost ("source") pin to the ground rail.  Finally, the middle ("drain") pin connects to the respective wire on the LED strip.  A typical "5050" LED strip has 4 wires:  Red, Green, Blue, and power.  Connect the LED strip power wire to the power rail.

The DC jack should have positive and negative hookups.  After inserting wires into each, attach them to the respective power/ground rails.

Wire the power rail to the VIN and the ground rail to the GND on the Arduino.

Verify the pin designations on the Rx sketch and upload to the Arduino.  Afterwards, plug in the DC adapter to the power jack.

### Testing
With both stations turned on (and the remote station given ample time to 'warm up'), you should be able to wave your hand in front of the motion detector and see the LED strip flash.
