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

#include "ConfigBlock.h"
#include "ConfigShellModule.h"
#include "Crc.h"
#include "DigitalPinShellModule.h"
#include "EepromShellModule.h"
#include "SerialPinShellModule.h"

// Arduino IDE issue:
// This include has to remain in place, otherwise for some reason
// the compiler cannot find it for other .cpp translation units 
// that rely on it. :(
#include <EEPROM.h>

// -----------------------------------------------------------------------
// Imports
// -----------------------------------------------------------------------

using ArduinoShell::ConfigBlock;
using ArduinoShell::ConfigShellModule;
using ArduinoShell::DigitalPinShellModule;
using ArduinoShell::EepromShellModule;
using ArduinoShell::SerialPinShellModule;

// -----------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------

HardwareSerial&         serialPort(Serial);

ConfigBlock             configBlock(0x0000);
ConfigShellModule       configShell(configBlock, serialPort);
DigitalPinShellModule   digitalPinShell(configBlock, serialPort);
EepromShellModule       eepromShell(serialPort);
SerialPinShellModule    serialPinShell(configBlock, serialPort);

// -----------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------

static void help()
{
    configShell.help();
    digitalPinShell.help();
    eepromShell.help();
    serialPinShell.help();
}

void setup() 
{
    // Single hardware-specific serial port call.
    serialPort.begin(115200);

    serialPort.println("Arduino Shell");
    serialPort.println("(c)2015 Max Vilimpoc (https://github.com/nuket/arduino-shell), MIT licensed.");
    serialPort.println();

    digitalPinShell.setup();
//    eepromShell.setup();
    serialPinShell.setup();

    help();
}

void loop() 
{
    const char TERMINATOR = '\n';

    // if (
    
    // Read and process commands.
    if (millis() % 1000 == 0 && serialPort.available())
    {
        String command = serialPort.readStringUntil(TERMINATOR);
        command.trim();
        serialPort.println(command);

        if (command.equals("help"))
        {
            help();
        }
        else
        {
            configShell.run(command);
            digitalPinShell.run(command);
            eepromShell.run(command);
            serialPinShell.run(command);
        }
    }
}
