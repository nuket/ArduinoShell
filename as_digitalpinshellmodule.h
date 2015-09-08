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

#ifndef __AS_DIGITALPINSHELLMODULE_H__
#define __AS_DIGITALPINSHELLMODULE_H__

#include "as_shellmodule.h"

/**
 * \brief Manipulate the digital I/O pins, and save the config to EEPROM.
 */
class DigitalPinShellModule : public ShellModule
{
public:
    static const uint32_t MODULE_ID = 0x4450534d; //!< 'DPSM' - used to identify the module config block in EEPROM.
    static const uint8_t  MAX_PINS  = 64;

    //! Specifies where defaults are stored in EEPROM.
    const uint32_t configBase;

    //! Sets up the digital I/O pins, using stored EEPROM defaults.
    void setup() override;

    const String& help() override;
    void run(String rawCommand) override;

    void saveDefaults();
    // void loadDefaults() override;

private:
    struct
    {
        uint32_t id;     //!< MODULE_ID needs to be stored here in EEPROM, or the config block SHOULD NOT BE USED.
        uint32_t crc;    //!< Currently using a 16-bit CRC, can shrink this field for other uses if needed.

        uint64_t io;     //!< INPUT = (0 << pin), OUTPUT = (1 << pin)
        uint64_t pullup; //!< INPUT_PULLUP means set this = (1 << pin), when necessary and input has (0 << pin).
        uint64_t value;  //!< HIGH or LOW.
    } config;

    //! The serial output stream to send status and debugging messages.
    Stream& serialOut;

    //! Wrapper for pinMode, to set bits in config block.
    void configPinMode (const uint8_t pin, const int mode);

    //! Wrapper for digitalWrite, to set bits in config block.
    void configPinValue(const uint8_t pin, const int value);
public:
    //! Set the EEPROM config block base address and serial output port.
    DigitalPinShellModule(const uint32_t configBase, Stream& serialOut = Serial);
};

#endif  /* __AS_DIGITALPINSHELLMODULE_H__ */
