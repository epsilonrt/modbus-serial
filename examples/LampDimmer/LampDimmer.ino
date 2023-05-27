/**
    @file LampDimmer.ino
    Modbus-Arduino Example - Lamp dimmer (Modbus Serial)
    Copyright (C) 2023 Pascal JEAN aka epsilonrt
    https://github.com/epsilonrt/modbus-serial
*/

#include <ModbusSerial.h>

// Used Pins
const int LedPin = 3; // Must be PWM output !
const int TxenPin = -1; // -1 disables the feature, change that if you are using an RS485 driver, this pin would be connected to the DE and /RE pins of the driver.

const byte SlaveId = 11; // our Modbus slave address (1-247)
// Modbus Registers Offsets (0-9999), PDU adressing ! from the data model we have to add 1
const int Lamp1Coil = 0;
const int Lamp1Hreg = 0;

#define MySerial Serial // define serial port used, Serial most of the time, or Serial1, Serial2 ... if available
const unsigned long Baudrate = 38400;

// ModbusSerial object
ModbusSerial mb (MySerial, SlaveId, TxenPin);

// Lamp status variables
bool lampOn = true;
word lampDimming = 128;

void setup() {

  MySerial.begin (Baudrate); // works on all boards but the configuration is 8N1 which is incompatible with the MODBUS standard
  // prefer the line below instead if possible
  // MySerial.begin (Baudrate, MB_PARITY_EVEN);

  mb.config (Baudrate);
  mb.setAdditionalServerData ("LDIMMER"); // for Report Server ID function (0x11)

  // Set LedPin
  pinMode (LedPin, OUTPUT);
  mb.addCoil (Lamp1Coil, lampOn);
  mb.addHreg (Lamp1Hreg, lampDimming);

  // The led flashes 20 times to signal the end of setup,
  // then it is set to its default state.
  for (int i = 0; i < 20; i++) {

    digitalWrite (LedPin, HIGH);
    delay (50);
    digitalWrite (LedPin, LOW);
    delay (100);
  }
  analogWrite (LedPin, (lampOn ? lampDimming : 0));
}

void loop() {

  // Call once inside loop() - all magic here
  mb.task();

  if (mb.Coil (Lamp1Coil) != lampOn) {
    // If coil was modified

    lampOn = mb.Coil (Lamp1Coil);
    if (lampOn) {

      analogWrite (LedPin, lampDimming);
    }
    else {

      analogWrite (LedPin, 0);
    }
  }

  if ( (mb.Hreg (Lamp1Hreg) & 0x00FF) != lampDimming) {
    // If holding register was modified (only LSB will used)

    lampDimming = (mb.Hreg (Lamp1Hreg) & 0x00FF);
    if (lampOn) {

      analogWrite (LedPin, lampDimming);
    }
  }
}
