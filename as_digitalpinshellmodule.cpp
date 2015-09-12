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

#include "as_commandandparams.h"
#include "as_configblock.h"
#include "as_crc.h"
#include "as_digitalpinshellmodule.h"

#include <EEPROM.h>

DigitalPinShellModule::DigitalPinShellModule(ConfigBlock& configBlock, Stream& serialOut) :
    configBlock(configBlock),
    serialOut(serialOut)
{
}

void DigitalPinShellModule::setup()
{
}

const String& DigitalPinShellModule::help()
{
}

void DigitalPinShellModule::run(String rawCommand)
{
    // Echo the command.
    serialOut.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand, serialOut);
    // cp.print();

    if (!cp.command.equals("pin")) return;

    // Examples: pin 8 in
    //           pin 8 out
    //           pin 8 in_pullup
    //           pin 8 high
    //           pin 8 low
    //           pin save
    if (cp.paramCount == 1)
    {
        if (cp.params[0].equals("save"))
        {
            saveDefaults();
        }
    }
    else
    if (cp.paramCount == 2)
    {
        // Set up I/O pins, turn them on or off (if set to output)
        if (cp.params[0].equals("all"))
        {
            if (cp.params[1].equals("out"))
            {
                for (int i = 0; i < MAX_PINS; i++)
                {
                    
                }
            }
        }
        else
        {
            long int pin = strtol(cp.params[0].c_str(), NULL, 0);
            // serialOut.println(pin);

            if (cp.params[1].equals("in"))
            {
                configPinMode(pin, INPUT);
            }
            else
            if (cp.params[1].equals("out"))
            {
                configPinMode(pin, OUTPUT);
            }
            else
            if (cp.params[1].equals("in_pullup"))
            {
                configPinMode(pin, INPUT_PULLUP);
            }
            else
            if (cp.params[1].equals("high"))
            {
                configPinValue(pin, HIGH);
            }
            else
            if  (cp.params[1].equals("low"))
            {
                configPinValue(pin, LOW);
            }
        }
    }

    // Save the pin configuration to EEPROM?
    // save();
}

void DigitalPinShellModule::saveDefaults()
{
    // EEPROM.put(configBase, config);
    configBlock.save();
}

//void DigitalPinShellModule::loadDefaults()
//{   
//}

void DigitalPinShellModule::configPinMode(const uint8_t pin, const int mode)
{
    uint64_t configBit = 1;

    // Quick exit.
    if (pin >= 64) return;

    // Figure out which bits need twiddling.
    configBit <<= pin;
    
    switch(mode)
    {
        case INPUT:
            config.io     &= ~configBit;
            config.pullup &= ~configBit;
            break;
        case OUTPUT:
            config.io     |=  configBit;
            config.pullup &= ~configBit;
            break;
        case INPUT_PULLUP:
            config.io     &= ~configBit;
            config.pullup |=  configBit;
            break;
    }

    serialOut.print((uint32_t)  (config.io & 0xFFFFFFFF), HEX);
    serialOut.print((uint32_t) ((config.io & 0xFFFFFFFF00000000) >> 32), HEX);

    pinMode(pin, mode);
}

void DigitalPinShellModule::configPinValue(const uint8_t pin, const int mode)
{
    uint64_t configBit = 1;

    // Quick exit.
    if (pin >= 64) return;
    if (!(HIGH == mode || LOW == mode)) return;

    // Figure out which bits represent current pin config.
    configBit <<= pin;
    
    // Pin has to be in OUTPUT mode to set HIGH / LOW.
    if (config.io & configBit)
    {
        if (mode == HIGH)
        {
            config.value |= configBit;
        }
        else
        {
            config.value &= ~configBit;
        }

        digitalWrite(pin, mode);
    }
}

