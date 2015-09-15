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

#ifndef __DIGITALPINSHELLMODULE_H__
#define __DIGITALPINSHELLMODULE_H__

#include "ShellModule.h"

namespace ArduinoShell
{

class ConfigBlock;

/**
 * \brief Manipulate the digital I/O pins, and save the config to EEPROM.
 */
class DigitalPinShellModule : public ShellModule
{
private:
    //! The serial output stream to send status and debugging messages.
    Stream& serialOut;

    //! Shared Config Block.
    ConfigBlock& configBlock;
public:
    //! Sets up the digital I/O pins, using stored EEPROM defaults.
    void setup() override;

    const String& help() override;
    void run(String rawCommand) override;

    //! Dependency-inject the config block and serial output port.
    DigitalPinShellModule(ConfigBlock& configBlock, Stream& serialOut);
};

} // namespace ArduinoShell

#endif  /* __DIGITALPINSHELLMODULE_H__ */
