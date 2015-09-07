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
#include "as_crc.h"
#include "as_digitalpinshellmodule.h"

#include <EEPROM.h>

DigitalPinShell::DigitalPinShell(const uint32_t configBase) :
    configBase(configBase)
{
}

void DigitalPinShell::setup()
{
    // Check the config block, and if the .id and .crc are valid,
    // load the defaults and apply them.
    EEPROM.get(configBase, config);

    // Calculate the CRC of the config block, minus the .crc field
    // i.e. clear .crc first, so that it is always 0 when doing the
    // calculation.
    const uint32_t storedCrc = config.crc;
    config.crc = 0;
    
    crc_t crc = crc_init();
    crc = crc_update(crc, (const uint8_t *) &config, sizeof(config));
    crc = crc_finalize(crc);

    // Serial.println(crc, HEX);

    if (MODULE_ID == config.id && crc == storedCrc)
    {
        // Process the configuration block, and set all of the I/O
        // pins accordingly.
        
    }
}

void DigitalPinShell::help()
{
}

void DigitalPinShell::run(String rawCommand)
{
    // Echo the command.
    Serial.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand);
    cp.print();

    if (!cp.command.equals("pin")) return;

    // Examples: pin 8 in
    //           pin 8 out
    //           pin 8 in_pullup
    //           pin 8 high
    //           pin 8 low
    //           pin save
    if (cp.paramCount == 1)
    {
        if (String("save").equals(cp.params[0]))
        {
            saveDefaults();
        }
    }
    else
    if (cp.paramCount == 2)
    {
        // Set up I/O pins, turn them on or off (if set to output)
        if (String("all").equals(cp.params[0]))
        {
            if (String("out").equals(cp.params[1]))
            {
                for (int i = 0; i < MAX_PINS; i++)
                {
                    
                }
            }
        }
        else
        {
            long int pin = strtol(cp.params[0].c_str(), NULL, 0);
            // Serial.println(pin);

            if (String("in").equals(cp.params[1].c_str()))
            {
                configPinMode(pin, INPUT);
            }
            else
            if (String("out").equals(cp.params[1].c_str()))
            {
                configPinMode(pin, OUTPUT);
            }
            else
            if (String("in_pullup").equals(cp.params[1].c_str()))
            {
                configPinMode(pin, INPUT_PULLUP);
            }
            else
            if (String("high").equals(cp.params[1].c_str()))
            {
                configPinValue(pin, HIGH);
            }
            else
            if (String("low").equals(cp.params[1].c_str()))
            {
                configPinValue(pin, LOW);
            }
        }
    }

    // Save the pin configuration to EEPROM?
    // save();
}

void DigitalPinShell::saveDefaults()
{
    EEPROM.put(configBase, config);
}

//void DigitalPinShell::loadDefaults()
//{   
//}

void DigitalPinShell::configPinMode(const uint8_t pin, const int mode)
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

    Serial.print((uint32_t)  (config.io & 0xFFFFFFFF), HEX);
    Serial.print((uint32_t) ((config.io & 0xFFFFFFFF00000000) >> 32), HEX);

    pinMode(pin, mode);
}

void DigitalPinShell::configPinValue(const uint8_t pin, const int mode)
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

