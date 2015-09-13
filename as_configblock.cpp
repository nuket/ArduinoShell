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

#include "as_configblock.h"

#include <EEPROM.h>

namespace ArduinoShell
{

ConfigBlock::ConfigBlock(uint32_t configBase) : 
    configBase(configBase)
{
    // Read in the configuration from EEPROM.
    EEPROM.get(configBase, configBlock);

//    // Check the config block, and if the .id and .crc are valid,
//    // load the defaults and apply them.
//    // EEPROM.get(configBase, config);
//
//    // Calculate the CRC of the config block, minus the .crc field
//    // i.e. clear .crc first, so that it is always 0 when doing the
//    // calculation.
//    const uint32_t storedCrc = config.crc;
//    config.crc = 0;
//    
//    crc_t crc = crc_init();
//    crc = crc_update(crc, (const uint8_t *) &config, sizeof(config));
//    crc = crc_finalize(crc);
//
//    // serialOut.println(crc, HEX);
//
//    if (MODULE_ID == config.id && crc == storedCrc)
//    {
//        // Process the configuration block, and set all of the I/O
//        // pins accordingly.
//        
//    }
}

bool ConfigBlock::save()
{
    // Calculate the CRC for the config block, and save it to EEPROM.
    
    EEPROM.put(configBase, configBlock);
}

void ConfigBlock::setPinType(uint8_t pin, PinType pinType)
{
    if (pin     >= MAX_PINS)            return;
    if (pinType >= PinType::LAST_ENTRY) return;

    configBlock.data[pin].type = pinType;
}

void ConfigBlock::setPinValue(uint8_t pin, const PinValue& value)
{
    if (pin     >= MAX_PINS)              return;
    // if (!(HIGH == value || LOW == value)) return;

    configBlock.data[pin].value = value;
}

bool ConfigBlock::isPinType(uint8_t pin, PinType pinType)
{
    if (pin     >= MAX_PINS)            return false;
    if (pinType >= PinType::LAST_ENTRY) return false;

    return (configBlock.data[pin].type == pinType);
}

} // namespace ArduinoShell
