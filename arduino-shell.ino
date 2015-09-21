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
    
    static char    commandBuffer[80] = {0};
    static uint8_t index = 0;
    static bool    newlineFound = false;

    // Read and echo bytes.
    if (serialPort.available() > 0)
    {
        // Append the bytes to the end of commandBuffer.
        commandBuffer[index] = serialPort.read();

        switch (commandBuffer[index])
        {
            case 0x08:
                // Also: http://www.ibb.net/~anne/keyboard.html
                //
                // If a backspace is pressed, you have to send 
                // a DEL byte as well, to clear the character.
                serialPort.write(commandBuffer[index]);
                serialPort.print("\033\1331\120");
                break;
            case '\r':
            case '\n':
                // If a CR / LF was detected in the incoming bytes,
                // then move on to command processing.
                serialPort.print("\r\n");
                newlineFound = true;
                break;
            default:
                // Echo the bytes.
                serialPort.write(commandBuffer[index]);
                break;
        }

        index++;
    }

    // Read and process commands.
    if (millis() % 250 == 0 && newlineFound)
    {
        String command(commandBuffer);
        command.trim();

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

        // Reset command input buffer, and indexing.
        memset(commandBuffer, 0, sizeof(commandBuffer));
        index = 0;
        newlineFound = false;
    }
}
