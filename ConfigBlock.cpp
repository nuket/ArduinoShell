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
#include "Crc.h"

#include <EEPROM.h>
#include <string.h>

namespace ArduinoShell
{

ConfigBlock::ConfigBlock(uint32_t configBase) : 
    configBase(configBase)
{
    Data eepromConfigData;
    
    // Read in the configuration from EEPROM.
    EEPROM.get(configBase, eepromConfigData);

    // Check the stored module ID and see if it is correct.
    if (MODULE_ID != eepromConfigData.moduleId)
    {
        return;
    }

    // Calculate the ConfigBlock CRC.

    crc_t crc = crc_init();
    crc = crc_update(crc, (const uint8_t *) eepromConfigData.pinConfig, sizeof(PinConfig) * MAX_PINS);
    crc = crc_finalize(crc);

    if (crc == eepromConfigData.crc)
    {
        data.crc = crc;
        memcpy(data.pinConfig, eepromConfigData.pinConfig, sizeof(PinConfig) * MAX_PINS);
    }
}

bool ConfigBlock::save()
{
    crc_t crc = crc_init();
    crc = crc_update(crc, (const uint8_t *) data.pinConfig, sizeof(PinConfig) * MAX_PINS);
    crc = crc_finalize(crc);

    data.crc = crc;

    EEPROM.put(configBase, data);
}

void ConfigBlock::setPinType(uint8_t pin, PinType pinType)
{
    if (pin     >= MAX_PINS)            return;
    if (pinType >= PinType::LAST_ENTRY) return;

    data.pinConfig[pin].type = pinType;
}

void ConfigBlock::setPinValue(uint8_t pin, const PinValue& value)
{
    if (pin     >= MAX_PINS)              return;
    // if (!(HIGH == value || LOW == value)) return;

    data.pinConfig[pin].value = value;
}

bool ConfigBlock::isPinType(uint8_t pin, PinType pinType)
{
    if (pin     >= MAX_PINS)            return false;
    if (pinType >= PinType::LAST_ENTRY) return false;

    return (data.pinConfig[pin].type == pinType);
}

constexpr const char * ConfigBlock::PinTypeStrings[PinType::LAST_ENTRY];

} // namespace ArduinoShell
