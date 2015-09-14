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

#ifndef __EEPROMSHELLMODULE_H__
#define __EEPROMSHELLMODULE_H__

#include "ShellModule.h"

namespace ArduinoShell
{

class EepromShellModule : public ShellModule
{
    Stream& serialOut;
    
    /**
     * Prints the contents of EEPROM from startAddress to startAddress + length.
     * 
     * Works best with multiples of EEPROM_ROW_LENGTH (16) byte-sized blocks.
     * 
     * Will not print less than EEPROM_ROW_LENGTH bytes, and will round down
     * to the next-lower block size.
     */
    void printContents(uint32_t startAddress, uint32_t length);
public:
    void setup() override;
    const String& help() override;
    void run(String rawCommand) override;

    EepromShellModule(Stream& serialOut);
};

} // namespace ArduinoShell

#endif // __EEPROMSHELLMODULE_H__
