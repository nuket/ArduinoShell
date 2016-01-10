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

#include "CommandAndParams.h"
#include "ConfigBlock.h"
#include "DigitalPinShellModule.h"

#include <EEPROM.h>

namespace ArduinoShell
{

DigitalPinShellModule::DigitalPinShellModule(ConfigBlock& configBlock, Stream& serialOut) :
    configBlock(configBlock),
    serialOut(serialOut)
{
}

/**
 * Restore the saved configuration of each of pins.
 */
void DigitalPinShellModule::setup()
{
    for (int i = 0; i < ConfigBlock::MAX_PINS; i++)
    {
        if (configBlock.isPinType(i, ConfigBlock::PinType::DIGITAL_INPUT)) 
        { 
            pinMode(i, INPUT);
        }
        else
        if (configBlock.isPinType(i, ConfigBlock::PinType::DIGITAL_INPUT_PULLUP)) 
        { 
            pinMode(i, INPUT_PULLUP);
        }
        else
        if (configBlock.isPinType(i, ConfigBlock::PinType::DIGITAL_OUTPUT_HIGH))
        { 
            pinMode(i, OUTPUT);
            digitalWrite(i, HIGH);
        }
        else
        if (configBlock.isPinType(i, ConfigBlock::PinType::DIGITAL_OUTPUT_LOW))
        {
            pinMode(i, OUTPUT);
            digitalWrite(i, LOW);
        }
    }
}

const String& DigitalPinShellModule::help()
{
    serialOut.println(F("Digital Pin Shell commands:\r\n"
                        "    pin <n> in\r\n"
                        "    pin <n> in_pullup\r\n"
                        "    pin <n> high\r\n"
                        "    pin <n> low\r\n"
                        "    pin <n> status\r\n"
                        "    pin <n> reset\r\n"));
    
}

void DigitalPinShellModule::run(String rawCommand)
{
    // Parse it.
    CommandAndParams cp(rawCommand, serialOut);
    // cp.print();

    if (!cp.command.equals(F("pin"))) return;

    // Examples: pin 8 in
    //           pin 8 in_pullup
    //           pin 8 high
    //           pin 8 low
    //           pin 8 reset
    if (cp.paramCount == 2)
    {
        // Set up I/O pins, turn them on or off (if set to output)
        if (cp.params[0].equals(F("all")))
        {
            if (cp.params[1].equals(F("out")))
            {
                for (int i = 0; i < ConfigBlock::MAX_PINS; i++)
                {
                }
            }
        }
        else
        {
            long int pin = strtol(cp.params[0].c_str(), NULL, 0);
            // serialOut.println(pin);

            if (cp.params[1].equals(F("in")))
            {
                configBlock.setPinType(pin, ConfigBlock::PinType::DIGITAL_INPUT);
                pinMode(pin, INPUT);
            }
            else
            if (cp.params[1].equals(F("in_pullup")))
            {
                configBlock.setPinType(pin, ConfigBlock::PinType::DIGITAL_INPUT_PULLUP);
                pinMode(pin, INPUT_PULLUP);
            }
            else
            if (cp.params[1].equals(F("high")))
            {
                configBlock.setPinType(pin, ConfigBlock::PinType::DIGITAL_OUTPUT_HIGH);
                pinMode(pin, OUTPUT);

//                ConfigBlock::PinValue pinValue = { .digital = HIGH };
//                configBlock.setPinValue(pin, pinValue);
                digitalWrite(pin, HIGH);
            }
            else
            if  (cp.params[1].equals(F("low")))
            {
                configBlock.setPinType(pin, ConfigBlock::PinType::DIGITAL_OUTPUT_LOW);
                pinMode(pin, OUTPUT);

//                ConfigBlock::PinValue pinValue = { .digital = LOW };
//                configBlock.setPinValue(pin, pinValue);
                digitalWrite(pin, LOW);
            }
            else
            if (cp.params[1].equals(F("status")))
            {
                ConfigBlock::PinType type = configBlock.getPinType(pin);
                serialOut.println(ConfigBlock::PinTypeStrings[type]);
            }
            else
            if (cp.params[1].equals(F("reset")))
            {
                configBlock.setPinType(pin, ConfigBlock::PinType::NO_TYPE);
            }
        }
    }
}

} // namespace ArduinoShell
