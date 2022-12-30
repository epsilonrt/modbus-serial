/**
  @file TempSensor.ino
  Modbus-Arduino Example - TempSensor (Modbus Serial)
  Copyright by André Sarmento Barbosa
  https://github.com/epsilonrt/modbus-serial
*/
 
#include <Modbus.h>
#include <ModbusSerial.h>


//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG = 100; 
//Used Pins
const int sensorPin = A0;

// ModbusSerial object
ModbusSerial mb;

long ts;

void setup() {
    // Config Modbus Serial (port, speed, byte format) 
    mb.config(&Serial, 38400, MB_PARITY_EVEN);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);  
    // mb.setAdditionalServerData ("TEMP_SENSOR"); // for Report Server ID function

    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addIreg(SENSOR_IREG);
    
    ts = millis();
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   
   //Read each two seconds
   if (millis() > ts + 2000) {   
       ts = millis();
       //Setting raw value (0-1024)
       mb.Ireg(SENSOR_IREG, analogRead(sensorPin));
   } 
}
