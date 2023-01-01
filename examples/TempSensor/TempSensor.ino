/**
  @file TempSensor.ino
  Modbus-Arduino Example - TempSensor (Modbus Serial)
    Copyright (C) 2023 Pascal JEAN aka epsilonrt
    Copyright (C) 2014 André Sarmento Barbosa
    https://github.com/epsilonrt/modbus-serial
*/

#include <ModbusSerial.h>

// Used Pins
const int SensorPin = A0;
const int TxenPin = -1; // -1 disables the feature, change that if you are using an RS485 driver, this pin would be connected to the DE and /RE pins of the driver.

const byte SlaveId = 14;
// Modbus Registers Offsets (0-9999)
const int SensorIreg = 0;

#define MySerial Serial // define serial port used, Serial most of the time, or Serial1, Serial2 ... if available
const unsigned long Baudrate = 38400;

// ModbusSerial object
ModbusSerial mb (MySerial, SlaveId, TxenPin);

long ts;

void setup() {

  MySerial.begin (Baudrate); // works on all boards but the configuration is 8N1 which is incompatible with the MODBUS standard
  // prefer the line below instead if possible
  // MySerial.begin (Baudrate, MB_PARITY_EVEN);
  while (! MySerial)
    ;

  mb.config (Baudrate);
  mb.setAdditionalServerData ("TEMP_SENSOR"); // for Report Server ID function (0x11)


  // Add SensorIreg register - Use addIreg() for analog Inputs
  mb.addIreg (SensorIreg);

  ts = millis();
}

void loop() {
  
  // Call once inside loop() - all magic here
  mb.task();

  // Read each two seconds
  if (millis() > ts + 2000) {
    
    ts = millis();
    // Setting raw value (0-1024)
    mb.Ireg (SensorIreg, analogRead (SensorPin));
  }
}
