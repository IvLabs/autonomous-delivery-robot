// This test suite is built for Arduino MEGA versions only;
// it tests with servos attached to one serial port
// (Serial1 is the hard-coded default) and will report 
// test results on Serial.

// Hardware Requirements:
// - Arduino MEGA, MEGA ADK
// - AX-12 connected directly to Serial1 port, 
//   without tristate buffer

// - By C. Matlack 2017-05-08


#include <ArduinoUnit.h>
#include <Dynamixel.h>
#include <DynamixelInterface.h>
#include <DynamixelMotor.h>
#include <SoftwareSerial.h>               

const unsigned int id = 2;
const long unsigned int baudrate = 1000000;

HardwareDynamixelInterface interface(Serial1);
DynamixelMotor motor(interface, id);


void setup() {
  Serial.begin(115200);
}

//------------ Custom Assertions ----------------------------
// Custom Assertion for memory leak in function
void assertNoLeak(void (*callback)())
{
  int start = freeMemory();
  (*callback)();
  int end = freeMemory();
  assertEqual(0, (start - end));
}

// Custom Assertion for comparing byte arrays
void assertBytesEqual(const uint8_t* expected, const uint8_t* actual, int size) {
  for (int i = 0; i < size; i++) {
    // Serial.print("> ");Serial.print(expected[i]);Serial.print(" ");Serial.println(actual[i]);
    if (expected[i] != actual[i]) {
      Serial.print("\nassertBytesEqual() failing at index ");
      Serial.println(i);
    }
    assertEqual(expected[i], actual[i]);
  }
}

// Custom assertion for empty receive buffer
bool BufferIsClear() {
    Serial1.flush();
  uint8_t count = 0;
  while (Serial1.available()) {
    Serial.print(Serial1.read());
    count++;
  }
  if (count != 0) { 
    Serial.print("\n chars left in Serial1 buffer: ");
    Serial.println(count);
  }
  Serial.flush();
  return (count == 0);
}


//--------------- Custom Setup ---------------------------------
void test_setup() {
  interface.begin(baudrate);
  assertTrue(BufferIsClear());
}

//--------------- Test Definitions ------------------------------

// Just make sure the test apparatus works.
test(0_dummy) {
  test_setup();
  assertTrue(true);
}

test(1_communication_test) {
  test_setup();
  assertEqual(motor.init(), DYN_STATUS_OK);
  uint8_t led_state = 1;
  assertEqual(motor.write(DYN_ADDRESS_LED, led_state), DYN_STATUS_OK);
  assertEqual(motor.read(DYN_ADDRESS_LED, led_state), DYN_STATUS_OK);
  assertEqual(led_state, 1);
  led_state = 0;
  assertEqual(motor.write(DYN_ADDRESS_LED, led_state), DYN_STATUS_OK);
  assertEqual(motor.read(DYN_ADDRESS_LED, led_state), DYN_STATUS_OK);
  assertEqual(led_state, 0);
}

test(2_junk_in_buffer) {
  test_setup();
  motor.init();
  motor.enableTorque();
  motor.jointMode();
  motor.speed(256);

  for (uint16_t pos = 400; pos < 600; pos+=10) {
     motor.goalPosition(pos);
     delay(15);
     assertTrue(BufferIsClear());
     uint16_t actual = motor.currentPosition();
      Serial.print(pos);
     Serial.print(" target/actual ");
     Serial.println(actual);
     assertTrue(actual != 0);
     assertTrue(BufferIsClear());           // This loop will be fine 
  }

  Serial.println("\nRepeating with increased delay\n");

  for (uint16_t pos = 400; pos < 600; pos+=10) {
     motor.goalPosition(pos);
     delay(150);                            // Note time delay difference
     assertTrue(BufferIsClear());
     Serial.print(pos);
     Serial.print(" target/actual ");
     Serial.println(motor.currentPosition());
     assertTrue(BufferIsClear());           // This fails, but not on first iteration
  }
  
}

test(3_compliance_margins) {
  test_setup();
  motor.init();

  // Intentional minor deviations from factory default for AX-12
  uint8_t cw_margin = 0x05;
  uint8_t ccw_margin = 0x03;
  uint8_t cw_slope = 0x44;
  uint8_t ccw_slope = 0x3A;

  uint8_t orig[4] = {0x0};
  uint8_t read[4] = {0x0};

  assertEqual(DYN_STATUS_OK, motor.getComplianceMargins(orig[0], orig[1], orig[2], orig[3]));

  assertEqual(DYN_STATUS_OK, motor.setComplianceMargins(cw_margin, ccw_margin, cw_slope, ccw_slope));
  assertEqual(DYN_STATUS_OK, motor.getComplianceMargins(read[0], read[1], read[2], read[3]));
  Serial.print("cw_margin set = ");
  Serial.print(cw_margin, 16);
  Serial.print(" got ");
  Serial.println(read[0], 16);
  assertTrue(read[0] == cw_margin);
  assertTrue(read[1] == ccw_margin);
  assertTrue(read[2] == cw_slope);
  assertTrue(read[3] == ccw_slope);

  // restore state
  assertEqual(DYN_STATUS_OK, motor.setComplianceMargins(orig[0], orig[1], orig[2], orig[3]));
}


void loop() {
  Test::run();
}
