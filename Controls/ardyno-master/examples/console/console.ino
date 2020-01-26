#include "SoftwareSerial.h"
#include "DynamixelConsole.h"

// This is a basic console that allow you to experiment easily with your servos,
// from the arduino serial terminal (or any other serial terminal)
// One limitation for arduino models with only one hardware UART, like Uno, is that
// you have to use a software serial to communicate with the servos, and thus it 
// works only at low baudrate. See change_baudrate example to see how to change the baud
// rate of your servos.
// If you have an arduino model with more than one UART, you won't have this problem,
// just adapt the interface to your need (see blink_led example).
//
// For now, only a few low level functions are implemented.
// Call "help" to get a list of the commands.
// You can find the register description table in the doc directory usefull
// Note that all values, including adresses must be passed in decimal format

#define SOFT_RX_PIN 3
#define SOFT_TX_PIN 4

const unsigned long dynamixel_baudrate=57600;
const unsigned long serial_baudrate=57600;

// software serial interface for Uno, see above
SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);

DynamixelConsole console(interface, Serial);

void setup() {
  interface.begin(dynamixel_baudrate);
  Serial.begin(serial_baudrate);
}

void loop() {
  console.loop();
}
