# Modbus Library for Arduino
Over Serial Line Implementation RTU (OSI 2/OSI 1)

[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/epsilonrt/modbus-serial?include_prereleases)](https://github.com/epsilonrt/modbus-serial/releases) 
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue)](https://www.arduino.cc/)
[![Build](https://github.com/epsilonrt/modbus-serial/actions/workflows/build.yml/badge.svg)](https://github.com/epsilonrt/modbus-serial/actions/workflows/build.yml) 

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/epsilonrt/library/modbus-serial.svg)](https://registry.platformio.org/libraries/epsilonrt/modbus-serial) 
[![Platform ATMELAVR](https://img.shields.io/badge/Platform-AtmelAVR-blue)](#)
[![Platform ATMELSAM](https://img.shields.io/badge/Platform-AtmelSAMD-blue)](#)
[![Platform ESP32](https://img.shields.io/badge/Platform-Espressif32-orange)](#)

---

This library allows your Arduino to communicate via Modbus protocol. The Modbus is a master-slave protocol
used in industrial automation and can be used in other areas, such as home automation.

In the current version the library allows the Arduino operate **as a slave**, supporting Modbus over Serial Line RTU.

## How to

There are four classes corresponding to five headers that may be used:

* [Modbus](http://github.com/epsilonrt/modbus-arduino ) - Base Library
* ModbusSerial - Modbus Serial RTU Library (this repository)  
* [ModbusEthernet](https://github.com/epsilonrt/modbus-ethernet) - Modbus TCP Library (standard Ethernet Shield)   
* [ModbusEthercard](https://github.com/epsilonrt/modbus-ethercard) - Modbus TCP Library (for ENC28J60 chip)  
* [ModbusEsp8266AT](https://github.com/epsilonrt/modbus-esp8266at) - Modbus IP Library (for ESP8266 chip with AT firmware)   

By opting for Modbus Serial or Modbus TCP you must include in your sketch the corresponding header and the base library header, eg:

    #include <Modbus.h>
    #include <ModbusSerial.h>

## ModbusSerial

There are four examples that can be accessed from the Arduino interface, once you have installed the library.
Let's look at the example Lamp.ino (only the parts concerning Modbus will be commented):


    #include <Modbus.h>
    #include <ModbusSerial.h>

Inclusion of the necessary libraries.

    const int LAMP1_COIL = 100;

Sets the Modbus register to represent a lamp or LED. This value is the offset (0-based) to be placed in its supervisory or testing software.
Note that if your software uses offsets 1-based the set value there should be 101, for this example.

    ModbusSerial mb;

Create the mb instance (ModbusSerial) to be used.

    mb.config (&Serial, 38400, MB_PARITY_EVEN);
    mb.setSlaveId (10);

Sets the serial port and the slave Id. Note that the serial port is passed as reference, which permits the use of other serial ports in other Arduino models.
The bitrate and parity is being set. If you are using RS-485 the configuration of another pin to control transmission/reception is required.
This is done as follows:

    mb.config (& Serial, 38400, MB_PARITY_EVEN, 2);

In this case, the pin 2 will be used to control TX/RX.

    mb.addCoil (LAMP1_COIL);

Adds the register type Coil (digital output) that will be responsible for activating the LED or lamp and verify their status.
The library allows you to set an initial value for the register:

    mb.addCoil (LAMP1_COIL, true);

In this case the register is added and set to true. If you use the first form the default value is false.


    mb.task ();

This method makes all magic, answering requests and changing the registers if necessary, it should be called only once, early in the loop.

    digitalWrite (ledPin, mb.Coil (LAMP1_COIL));

Finally the value of LAMP1_COIL register is used to drive the lamp or LED.


In much the same way, the other examples show the use of other methods available in the library:

    void addCoil (offset word, bool value)
    void addHreg (offset word, word value)
    void addIsts (offset word, bool value)
    void addIreg (offset word, word value)

Adds registers and configures initial value if specified.

    bool Coil (offset word, bool value)
    bool Hreg (offset word, word value)
    bool Ists (offset word, bool value)
    bool IReg (offset word, word value)

Sets a value to the register.

    bool Coil (offset word)
    word Hreg  (word offset)
    bool Ists (offset word)
    word IReg (word offset)

Returns the value of a register.

Contributions
=============
http://github.com/epsilonrt/modbus-arduino  
prof (at) andresarmento (dot) com
epsilonrt (at) gmail (dot) com

License
=======
The code in this repo is licensed under the BSD New License. See LICENSE for more info.

