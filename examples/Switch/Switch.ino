/**
    @file Switch.ino
    Modbus-Arduino Example - Switch (Modbus Serial)
    Copyright (C) 2023 Pascal JEAN aka epsilonrt
    Copyright (C) 2014 André Sarmento Barbosa
    https://github.com/epsilonrt/modbus-serial
*/

#include <ModbusSerial.h>

// Used Pins
const int SwitchPin = 3;
const int TxenPin = -1; // -1 disables the feature, change that if you are using an RS485 driver, this pin would be connected to the DE and /RE pins of the driver.

const byte SlaveId = 13;
// Modbus Registers Offsets (0-9999)
const int SwitchIsts = 0;

#define MySerial Serial // define serial port used, Serial most of the time, or Serial1, Serial2 ... if available
const unsigned long Baudrate = 38400;

// ModbusSerial object
ModbusSerial mb (MySerial, SlaveId, TxenPin);

void setup() {
  
  MySerial.begin (Baudrate); // works on all boards but the configuration is 8N1 which is incompatible with the MODBUS standard
  // prefer the line below instead if possible
  // MySerial.begin (Baudrate, MB_PARITY_EVEN);
  
  mb.config (Baudrate);
  mb.setAdditionalServerData ("SWITCH"); // for Report Server ID function (0x11)

  // Set SwitchPin mode
  pinMode (SwitchPin, INPUT);
  // Add SwitchIsts register - Use addIsts() for digital inputs
  mb.addIsts (SwitchIsts);
}

void loop() {
  //Call once inside loop() - all magic here
  mb.task();

  //Attach SwitchPin to SwitchIsts register
  mb.Ists (SwitchIsts, digitalRead (SwitchPin));
}
