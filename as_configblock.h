/*
    Arduino Shell
    Copyright (c) 2015 Max Vilimpoc
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

/**
 * \file
 */

#ifndef __AS_CONFIGBLOCK_H__
#define __AS_CONFIGBLOCK_H__

#include <Arduino.h>
#include <stdint.h>

class ConfigBlock
{
public:
    //! 'CNFG' - used to identify the module config block in EEPROM.
    static const uint32_t MODULE_ID = 0x434e4647;  

    //! Maximum number of pins to configure.
    static const uint8_t MAX_PINS = 64;

    enum PinType
    {
        NO_TYPE              = 0,  //!< Don't bother configuring at setup time.
        DIGITAL_INPUT        = 1,
        DIGITAL_INPUT_PULLUP = 2,
        DIGITAL_OUTPUT       = 3,
        ANALOG_INPUT         = 4,
        PWM                  = 5,
        SERIAL_HARDWARE      = 6,
        SERIAL_SOFTWARE      = 7,
        SERIAL_CAN           = 8,
        SERIAL_I2C           = 9,
        SERIAL_SPI           = 10,
        LAST_ENTRY
    };

    const char * PinTypeStrings[PinType::LAST_ENTRY] = {
        "NO_TYPE",
        "DIGITAL_INPUT",
        "DIGITAL_INPUT_PULLUP",
        "DIGITAL_OUTPUT",
        "ANALOG_INPUT",
        "PWM",
        "SERIAL_HARDWARE",
        "SERIAL_SOFTWARE",
        "SERIAL_CAN",
        "SERIAL_I2C",
        "SERIAL_SPI"
    };

    struct PinConfig
    {
        PinType type : 4;  //!< Not type-safe.
        int     value;     //!< Starting value, 0 / 1 for DIGITAL_OUTPUT, 0 - 255 for PWM?
    };

    struct Data
    {
        const uint32_t moduleId = MODULE_ID;
        uint32_t       crc;

        PinConfig data[MAX_PINS];
    };

    //! Set the EEPROM config block base address.
    ConfigBlock(uint32_t configBase, Stream& serialOut);

    /** 
     * \brief Save the data.
     * 
     * \return true if succeeded, false if not
     */
    bool save();

    void setPinType(uint8_t pin, PinType pinType);
    void setPinValue(uint8_t pin, uint32_t value);

    /**
     * \brief Print out the contents of the ConfigBlock data in a human-readable form.
     */
    void cat();

private:
    Stream& serialOut;

    //! EEPROM location for storing config data.
    const uint32_t configBase;

    //! Data block in RAM.
    Data configBlock;
};

#endif  /* __AS_CONFIGBLOCK_H__ */
