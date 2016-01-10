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

#ifndef __CONFIGBLOCK_H__
#define __CONFIGBLOCK_H__

#include <stdint.h>

namespace ArduinoShell
{

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
        DIGITAL_INPUT,
        DIGITAL_INPUT_PULLUP,
        DIGITAL_OUTPUT_HIGH,
        DIGITAL_OUTPUT_LOW,   
        ANALOG_INPUT,
        PWM,
        SERIAL_HARDWARE,
        SERIAL_SOFTWARE,
        SERIAL_CAN,
        SERIAL_I2C,
        SERIAL_SPI,
        LAST_ENTRY
    };

    static constexpr const char * PinTypeStrings[PinType::LAST_ENTRY] = {
        "NO_TYPE",
        "DIGITAL_INPUT",
        "DIGITAL_INPUT_PULLUP",
        "DIGITAL_OUTPUT_HIGH",
        "DIGITAL_OUTPUT_LOW",
        "ANALOG_INPUT",
        "PWM",
        "SERIAL_HARDWARE",
        "SERIAL_SOFTWARE",
        "SERIAL_CAN",
        "SERIAL_I2C",
        "SERIAL_SPI"
    };

    union PinValue
    {
        uint8_t  digital;      //!< HIGH / LOW for DIGITAL_OUTPUT
        uint32_t pwm;          //!< 0 - 255 for PWM
        struct
        {
            uint32_t baud;     //!< Baudrate 9600 - 115200
            uint8_t  settings; //!< See: hardware/arduino/avr/cores/arduino/HardwareSerial.h for settings.
        };
    };

    struct PinConfig
    {
        PinType  type : 4;  //!< Not type-safe.
        PinValue value;
    };

    struct Data
    {
        const uint32_t moduleId = MODULE_ID;
        uint32_t       crc;

        PinConfig      pinConfig[MAX_PINS];

        
    };

    //! Set the EEPROM config block base address.
    ConfigBlock(uint32_t configBase);

    /** 
     * \brief Save the data.
     * 
     * \return true if succeeded, false if not
     */
    bool save();

    void setPinType (uint8_t pin, PinType pinType);
    void setPinValue(uint8_t pin, const PinValue& value);

    PinType getPinType(uint8_t pin);

    /**
     * \brief Allow caller to check whether pin is of a particular type.
     * 
     * Used by the initialization routines for the various pin types.
     */
    bool isPinType(uint8_t pin, PinType pinType);

private:
    friend class ConfigShellModule;

    //! EEPROM location for storing config data.
    const uint32_t configBase;

    //! Data block in RAM.
    Data data;
};

} // namespace ArduinoShell

#endif  /* __CONFIGBLOCK_H__ */
