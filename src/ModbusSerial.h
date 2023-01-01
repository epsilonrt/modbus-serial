/*
    ModbusSerial.h - Header for ModbusSerial Library
    Copyright (C) 2023 Pascal JEAN aka epsilonrt
    Copyright (C) 2014 André Sarmento Barbosa
*/
#include <Arduino.h>
#include <Modbus.h>

#pragma once

/**
 * @enum MB_PARITY
 * @brief Parity
 *
 * See paragraph [2.5.1 RTU Transmission Mode] from
 * MODBUS over serial line specification and implementation guide V1.02
 * http://www.modbus.org/docs/Modbus_over_serial_line_V1_02.pdf
 *
 * The format (11 bits) for each byte in RTU mode is :
 * Coding System:
 *
 * * 8–bit binary
 *
 * Bits per Byte:
 *
 *  * 1 start bit
 *  * 8 data bits, least significant bit sent first
 *  * 1 bit for parity completion
 *  * 1 stop bit
 *  .
 *
 * Even parity is required, other modes ( odd parity, no parity )
 * may also be used. In order to ensure a maximum compatibility with
 * other products, it is recommended to support also No parity mode.
 * The default parity mode must be even parity.
 * Remark : the use of no parity requires 2 stop bits.
 */
enum MB_PARITY {
  MB_PARITY_NONE = SERIAL_8N2, ///< No parity
  MB_PARITY_EVEN = SERIAL_8E1, ///< Even parity
  MB_PARITY_ODD  = SERIAL_8O1  ///< Odd parity
};

/**
 * @class ModbusSerial
 * @brief Modbus over serial line Class
 */
class ModbusSerial : public Modbus {

  public:
    /**
     * @brief Constructor
     * @param port serial port to use
     * @param txenPin if an RS485 circuit is used, this corresponds to the
     * pin number connected to the transmit enable (DE) and receive disable (/RE) pin.
     * -1 if not used.
     */
    ModbusSerial (Stream & port, byte slaveId, int txenPin = -1);

    /**
     * @brief Configure ModbusSerial object
     * 
     * configures txenPin on output if necessary and calculates the times of the RTU frame.
     * 
     * @warning the \c begin() function of the serial port passed to the constructor 
     * must be called **BEFORE** calling this function.
     * 
     * @param baud baudrate in accordance with that used by the serial port
     */
    void config (unsigned long baud);

    /**
     * @brief Task that performs all operations on MODBUS
     *
     * Call once inside loop(), all magic here !
     */
    void task();

    /**
     * @brief Change the value of slave identifier
     * @param slaveId identifier 1-247
     */
    void setSlaveId (byte slaveId);

    /**
     * @brief Return slave identifier
     */
    byte getSlaveId();

  protected:
    bool receive (byte * frame);
    bool sendPDU (byte * pduframe);
    bool send (byte * frame);
    void reportServerId();

  private:
    Stream * m_stream;
    int   m_txenPin;
    byte  m_slaveId;

    unsigned int m_t15; // inter character time out (us)
    unsigned int m_t35; // frame delay  (us)

  private:
    word calcCrc (byte address, byte * pduframe, byte pdulen);
};
