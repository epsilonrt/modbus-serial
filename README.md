# Modbus Library for Arduino
Over Serial Line Implementation RTU (OSI 2/OSI 1)

[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/epsilonrt/modbus-serial?include_prereleases)](https://github.com/epsilonrt/modbus-serial/releases) 
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/epsilonrt/library/modbus-serial.svg)](https://registry.platformio.org/libraries/epsilonrt/modbus-serial) 
[![Arduino Registry](https://www.ardu-badge.com/badge/Modbus-Serial.svg)](https://www.arduinolibraries.info/libraries/modbus-serial)

[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)
[![Uno](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_uno.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_uno.yml) 
[![Due](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_due.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_due.yml) 
[![Mega](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_mega.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_mega.yml) 
[![Nano33IoT](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_nano33iot.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_nano33iot.yml)
[![Teensy](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_teensy.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_teensy.yml)
[![Esp8266](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_esp8266.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_esp8266.yml) 
[![Esp32](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_esp32.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build_esp32.yml) 


---

This library allows your Arduino to communicate via Modbus protocol. The Modbus is a master-slave protocol
used in industrial automation and can be used in other areas, such as home automation.

In the current version the library allows the Arduino operate **as a slave**, supporting Modbus over Serial Line RTU.

## How to

There are four classes corresponding to five headers that may be used:

* [Modbus-Arduino](http://github.com/epsilonrt/modbus-arduino ) - Base Library
* Modbus-Serial - Modbus Serial RTU Library (this repository)  
* [Modbus-Ethernet](https://github.com/epsilonrt/modbus-ethernet) - Modbus TCP Library (standard Ethernet Shield)   
* [Modbus-EtherCard](https://github.com/epsilonrt/modbus-ethercard) - Modbus TCP Library (for ENC28J60 chip)  
* [Modbus-Esp8266AT](https://github.com/epsilonrt/modbus-esp8266at) - Modbus IP Library (for ESP8266 chip with AT firmware)   

By opting for Modbus Serial or Modbus TCP you must include in your sketch the corresponding header, eg:

    #include <ModbusSerial.h>

## ModbusSerial

There are five examples that can be accessed from the Arduino IDE or Visual 
Studio Code with Platformio, once you have installed the library.
Let's look at the example Lamp.ino (only the parts concerning Modbus will be commented):

    #include <ModbusSerial.h>

Inclusion of the necessary header.

    const int Lamp1Coil = 0;

Sets the Modbus register to represent a lamp or LED. This value is the offset (0-based) to be placed in its supervisory or testing software.
Note that if your software uses offsets 1-based the set value there should be 1, for this example.

    ModbusSerial mb (Serial, SlaveId);

Create the mb instance (ModbusSerial) to be used with the serial port `Serial`. 
Note that the serial port is passed as reference, which permits the use of other 
serial ports in other Arduino models, also ets the slave Id. 

If you are using RS-485 driver,  the configuration of another pin to control 
transmission/reception is required, this pin would be connected to the DE and 
/RE pins of the driver. This is done as follows:

    const int TxenPin = 4;
    // ....
    ModbusSerial mb (Serial, SlaveId, TxenPin);

In this case, the pin 4 will be used to control TX/RX (DE must be connected with /RE).

    Serial.begin (Baudrate, MB_PARITY_EVEN);
    while (! Serial)
      ;

Configure the serial port and wait for it to be ready.

To simplify the modification of the port, you can define a macro in the following way :

    #define MySerial Serial // define serial port used, Serial most of the time, or Serial1, Serial2 ... if available

Then the block above can be written as follows :

    ModbusSerial mb (MySerial, SlaveId, TxenPin);

    void setup() {

      MySerial.begin (Baudrate, MB_PARITY_EVEN); // prefer this line in accordance with the modbus standard.
      while (! MySerial)
        ;

      mb.config (Baudrate);
    
The last line configures the ModbusSerial object.

    mb.addCoil (Lamp1Coil);

Adds the register type Coil (digital output) that will be responsible for 
activating the LED or lamp and verify their status. 
The library allows you to set an initial value for the register:

    mb.addCoil (Lamp1Coil, true);

In this case the register is added and set to true. If you use the first form 
the default value is false.

    mb.task ();

This method makes all magic, answering requests and changing the registers if 
necessary, it should be called only once, early in the loop.

    digitalWrite (LedPin, mb.Coil (Lamp1Coil));

Finally the value of Lamp1Coil register is used to drive the lamp or LED.

In much the same way, the other examples show the use of other methods available in the library:

    void addCoil (offset word, bool value)
    void addHreg (offset word, word value)
    void addIsts (offset word, bool value)
    void addIreg (offset word, word value)

Adds registers and configures initial value if specified.

    bool setCoil (offset word, bool value)
    bool setHreg (offset word, word value)
    bool setIsts (offset word, bool value)
    bool setIReg (offset word, word value)

Sets a value to the register.

    bool coil (offset word)
    word hreg  (word offset)
    bool ists (offset word)
    word ireg (word offset)

Returns the value of a register.

License
=======
The code in this repo is licensed under the BSD New License. See LICENSE for more info.

