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

#include "AdcPinShellModule.h"
#include "ConfigShellModule.h"
#include "DigitalPinShellModule.h"
#include "EepromShellModule.h"
#include "SerialPinShellModule.h"

#include "Task.h"

// Arduino IDE issue:
// This include has to remain in place, otherwise for some reason
// the compiler cannot find it for other .cpp translation units 
// that rely on it. :(
#include <EEPROM.h>

// -----------------------------------------------------------------------
// Flash-free defines.
// -----------------------------------------------------------------------

#define MAX_COMMAND_LENGTH 20

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

class CommandInputTask : public Task
{
public:
    CommandInputTask(Stream & consolePort) :
        consolePort(consolePort)
    {
        clearCommand();
    }

    bool    hasCommand();
    String  getCommand();
private:
    void    main();
    void    clearCommand();
   
    Stream& consolePort;

    char    commandBuffer[MAX_COMMAND_LENGTH];
    uint8_t index;
    bool    newlineFound;
};

void CommandInputTask::main()
{
    char input;
    
    // Read and echo bytes.
    if (consolePort.available() > 0)
    {
        input = consolePort.read();

        switch (input)
        {
            case 0x08:
                // Also: http://www.ibb.net/~anne/keyboard.html
                //
                // If a backspace is pressed, you have to send 
                // a VT100 erase character sequence as well, to clear the character.
                if (index > 0)
                {
                    consolePort.write(input);           // backspace
                    consolePort.print(F("\033\1331\120")); // erase this character
                    
                    index--;
                }
                
                commandBuffer[index] = 0;
                break;
            case '\r':
            case '\n':
                // If a CR / LF was detected in the incoming bytes,
                // then move on to command processing.
                consolePort.print("\r\n");
                newlineFound = true;
                break;
            default:
                if (0x20 <= input && input <= 0x7e &&
                    index < sizeof(commandBuffer) - 1) // Limit text entry to (MAX_COMMAND_LENGTH - 1) characters.
                {
                    // Echo the bytes.
                    consolePort.write(input);
                    
                    commandBuffer[index] = input;
                    index++;
                }
                break;
        }
    }
}

bool CommandInputTask::hasCommand()
{
    return newlineFound;
}

String CommandInputTask::getCommand()
{
    String command(commandBuffer);
    clearCommand();
    
    return command;
}

void CommandInputTask::clearCommand()
{
    memset(commandBuffer, '\0', MAX_COMMAND_LENGTH);
    index = 0;
    newlineFound = false;
}

CommandInputTask commandInputTask(serialPort);

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
    serialPort.begin(115200);

    serialPort.println(F("Arduino Shell"));
    serialPort.println(F("Copyright (c) 2015 Max Vilimpoc (https://github.com/nuket/arduino-shell), MIT licensed.\n"));

    digitalPinShell.setup();
//    eepromShell.setup();
    serialPinShell.setup();

    help();
}

void loop() 
{
    commandInputTask.run();

    // Read and process commands.
    if (millis() % 250 == 0 && commandInputTask.hasCommand())
    {
        String command = commandInputTask.getCommand();
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
    }
}
