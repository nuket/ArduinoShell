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

#include <ArduinoShell.h>

// -----------------------------------------------------------------------
// Flash-free defines.
// -----------------------------------------------------------------------

#define COMMAND_PROCESS_INTERVAL 250  //!< Process commands every 250ms.
#define MAX_COMMAND_LENGTH       20   //!< Commands can be this long.

// -----------------------------------------------------------------------
// Imports
// -----------------------------------------------------------------------

using ArduinoShell::ConfigBlock;

using ArduinoShell::AdcPinShellModule;
using ArduinoShell::ConfigShellModule;
using ArduinoShell::DigitalPinShellModule;
using ArduinoShell::EepromShellModule;
using ArduinoShell::SerialPinShellModule;

// -----------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------

HardwareSerial&         serialPort(Serial);

ConfigBlock             configBlock(0x0000);

AdcPinShellModule       adcPinShell(configBlock, serialPort);
ConfigShellModule       configShell(configBlock, serialPort);
DigitalPinShellModule   digitalPinShell(configBlock, serialPort);
EepromShellModule       eepromShell(serialPort);
SerialPinShellModule    serialPinShell(configBlock, serialPort);

// -----------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------

void help()
{
    adcPinShell.help();
    configShell.help();
    digitalPinShell.help();
    eepromShell.help();
    serialPinShell.help();
}

void setup() 
{
    serialPort.begin(57600);

    serialPort.println(F("Arduino Shell"));
    serialPort.println(F("Copyright (c) 2015 Max Vilimpoc (https://github.com/nuket/arduino-shell), MIT licensed.\n"));

    digitalPinShell.setup();
//    eepromShell.setup();
    serialPinShell.setup();

    help();
}

void loop() 
{
    const char TERMINATOR = '\n';
  
    static char     commandBuffer[MAX_COMMAND_LENGTH] = {0};
    static uint8_t  index = 0;
    static bool     newlineFound = false;

    static uint32_t processNextCommand = millis() + COMMAND_PROCESS_INTERVAL;

    char input;

    // Read and echo bytes.
    if (serialPort.available() > 0)
    {
        input = serialPort.read();

        switch (input)
        {
            case 0x08:
                // Also: http://www.ibb.net/~anne/keyboard.html
                //
                // If a backspace is pressed, you have to send 
                // a VT100 erase character sequence as well, to clear the character.
                if (index > 0)
                {
                    serialPort.write(input);           // backspace
                    serialPort.print(F("\033\1331\120")); // erase this character
                    
                    index--;
                }
                
                commandBuffer[index] = 0;
                break;
            case '\r':
            case '\n':
                // If a CR / LF was detected in the incoming bytes,
                // then move on to command processing.
                serialPort.print("\r\n");
                newlineFound = true;
                break;
            default:
                if (0x20 <= input && input <= 0x7e &&
                    index < sizeof(commandBuffer) - 1) // Limit text entry to (MAX_COMMAND_LENGTH - 1) characters.
                {
                    // Echo the bytes.
                    serialPort.write(input);
                    
                    commandBuffer[index] = input;
                    index++;
                }
                break;
        }

    }

    // Read and process commands.
    if (millis() > processNextCommand && newlineFound)
    {
        String command(commandBuffer);
        command.trim();

        if (command.equals(F("help")))
        {
            help();
        }
        else
        {
            adcPinShell.run(command);
            configShell.run(command);
            digitalPinShell.run(command);
            eepromShell.run(command);
            serialPinShell.run(command);
        }

        // Reset command input buffer, and indexing.
        memset(commandBuffer, 0, sizeof(commandBuffer));
        index = 0;
        newlineFound = false;

        processNextCommand += COMMAND_PROCESS_INTERVAL;
    }
}
