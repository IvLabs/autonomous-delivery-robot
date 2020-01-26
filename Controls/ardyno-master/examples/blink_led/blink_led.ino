// This is a simple example to test communication with the motors
// It should make all motor led blink once per second
// Make sure to select the correct type of interface (hardware or serial, with or without tristate buffer)
// and baudrate depending of your configuration.
// Default baudrate is 1000000 (only hardware serial is capable of that speed)

#include <DynamixelMotor.h>

// direction pin, if you use tristate buffer
#define DIR_PIN 2

// software serial pins, if you use software serial
#define SOFT_RX_PIN 3
#define SOFT_TX_PIN 4

// Use this for hardware serial without tristate buffer
HardwareDynamixelInterface interface(Serial);

// Use this for hardware serial with tristate buffer
//HardwareDynamixelInterface interface(Serial, DIR_PIN);

// Use this for software serial without tristate buffer
//SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);

// Use this for software serial with tristate buffer
//SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN, DIR_PIN);

const long unsigned int baudrate = 1000000;

// Use broadcast address to affect all connected motors
DynamixelDevice broadcast_device(interface, BROADCAST_ID);

uint8_t led_state=true;

void setup() {
  interface.begin(1000000);
  delay(100);
}

void loop() {
  broadcast_device.write(DYN_ADDRESS_LED, led_state);
  led_state=!led_state;
  delay(1000);
}

