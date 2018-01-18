/*
  Modbus-Arduino Example - Lamp dimmer (Modbus Serial)
  Copyright by epsilonrt
  http://github.com/epsilonrt/modbus-arduino
*/

#include <Modbus.h>
#include <ModbusSerial.h>

// Serial port used:  Serial most of the time, or Serial1, Serial2 ... if available.
HardwareSerial * port = &Serial;

// our Modbus slave address (1-247)
const int MODBUS_SLAVE = 10;
// Modbus Registers Offsets (0-9999), PDU adressing ! from the data model we have to add 1
const int LAMP1_COIL = 0;
const int LAMP1_HREG = 0;

// Used Pins
const int ledPin = 7; // Must be PWM output !
const int txenPin = -1; // Set to -1 if no TxEN pin used

// Lamp status variables
bool lampOn = true;
word lampDimming = 128;

// ModbusSerial object
ModbusSerial mb;

void setup() {
  // Config Modbus Serial (port, speed, byte format)
  // Remark : the use of no parity requires 2 stop bits.
  mb.config (port, 38400, MB_PARITY_EVEN, txenPin);
  // Set the Slave ID
  mb.setSlaveId (MODBUS_SLAVE);

  // Set ledPin
  pinMode (ledPin, OUTPUT);

  mb.addCoil (LAMP1_COIL, lampOn);
  mb.addHreg (LAMP1_HREG, lampDimming);

  // The led flashes 20 times to signal the end of setup, 
  // then it is set to its default state.
  for (int i = 0; i < 20; i++) {

    digitalWrite (ledPin, 1);
    delay (50);
    digitalWrite (ledPin, 0);
    delay (100);
  }
  analogWrite (ledPin, (lampOn ? lampDimming : 0));
}

void loop() {

  // Call once inside loop() - all magic here
  mb.task();

  if (mb.Coil (LAMP1_COIL) != lampOn) {
    // If coil was modified

    lampOn = mb.Coil (LAMP1_COIL);
    if (lampOn) {

      analogWrite (ledPin, lampDimming);
    }
    else {

      analogWrite (ledPin, 0);
    }
  }

  if ( (mb.Hreg (LAMP1_HREG) & 0x00FF) != lampDimming) {
    // If holding register was modified (only LSB will used)

    lampDimming = (mb.Hreg (LAMP1_HREG) & 0x00FF);
    if (lampOn) {

      analogWrite (ledPin, lampDimming);
    }
  }
}
