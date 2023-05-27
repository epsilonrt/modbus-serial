/**
    @file Servo.ino
    Copyright (C) 2023 Pascal JEAN aka epsilonrt
    Copyright (C) 2014 André Sarmento Barbosa
    https://github.com/epsilonrt/modbus-serial

    Use Servo lib
    This library only supports boards with an AVR, SAM, SAMD, NRF52 or STM32F4 processor.
    https://registry.platformio.org/libraries/arduino-libraries/Servo
*/

#include <ModbusSerial.h>
#include <Servo.h>

// Used Pins
const int servoPin = 9;
const int TxenPin = -1; // -1 disables the feature, change that if you are using an RS485 driver, this pin would be connected to the DE and /RE pins of the driver.

const byte SlaveId = 12;
// Modbus Registers Offsets (0-9999)
const int ServoHreg = 0;

#define MySerial Serial // define serial port used, Serial most of the time, or Serial1, Serial2 ... if available
const unsigned long Baudrate = 38400;

// ModbusSerial object
ModbusSerial mb (MySerial, SlaveId, TxenPin);
// Servo object
Servo servo;

void setup() {

  MySerial.begin (Baudrate); // works on all boards but the configuration is 8N1 which is incompatible with the MODBUS standard
  // prefer the line below instead if possible
  // MySerial.begin (Baudrate, MB_PARITY_EVEN);

  mb.config (Baudrate);
  mb.setAdditionalServerData ("SWITCH"); // for Report Server ID function (0x11)

  // Attaches the servo pin to the servo object
  servo.attach (servoPin);
  // Add ServoHreg register - Use addHreg() for analog outpus or to store values in device
  mb.addHreg (ServoHreg, 127);
}

void loop() {

  //Call once inside loop() - all magic here
  mb.task();

  //Attach switchPin to SWITCH_ISTS register
  servo.write (mb.Hreg (ServoHreg));
  delay (15);
}
