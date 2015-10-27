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
#include "ConfigShellModule.h"

namespace ArduinoShell 
{

ConfigShellModule::ConfigShellModule(ConfigBlock& configBlock, Stream& serialOut) :
    configBlock(configBlock),
    serialOut(serialOut)
{
}

const String& ConfigShellModule::help()
{
    serialOut.println(F("Config Shell commands:\r\n"
                        "    config print\r\n"
                        "    config save\r\n"));
}

void ConfigShellModule::run(String rawCommand)
{
    // Parse it.
    CommandAndParams cp(rawCommand, serialOut);
    // cp.print();

    if (!cp.command.equals(F("config"))) return;

    if (cp.params[0].equals(F("save")))
    {
        configBlock.save();
        serialOut.println(F("Config saved.\r\n"));
    }
    else
    if (cp.params[0].equals(F("print")))
    {
        print();
    }
}

void ConfigShellModule::print()
{
    char output[80] = {0};
    
    uint8_t typeL = 0;
    uint8_t typeR = 0;
    
    uint8_t maxPins = ConfigBlock::MAX_PINS >> 1;

    snprintf(output, 80, "ConfigBlock CRC: %08x\r\n", configBlock.data.crc);
    serialOut.println(output);
    
    for (int i = 0; i < maxPins; i++)
    {
        memset(output, 0, 80);
        
        typeL = configBlock.data.pinConfig[2 * i].type;
        typeR = configBlock.data.pinConfig[2 * i + 1].type;
        
        snprintf(output, 80, "Pin %02d: %20s    |    Pin %02d: %20s", 
            2 * i,     typeL < ConfigBlock::PinType::LAST_ENTRY ? ConfigBlock::PinTypeStrings[typeL] : "NOT CONFIGURED",
            2 * i + 1, typeR < ConfigBlock::PinType::LAST_ENTRY ? ConfigBlock::PinTypeStrings[typeR] : "NOT CONFIGURED");

        serialOut.println(output);

//        switch (configBlock.data.pinConfig[i].type)
//        {
//            case ConfigBlock::PinType::DIGITAL_OUTPUT:
//                memset(output, 0, 80);
//                snprintf(output, 80, " (%s)", configBlock.data.pinConfig[i].value.digital == HIGH ? "HIGH" : "LOW");
//                serialOut.println(output);
//                break;
////            case PinType::PWM:
////                break;
////            case PinType::SERIAL_HARDWARE:
////                break;
////            case PinType::SERIAL_SOFTWARE:
////                break;
//            default:
//                serialOut.println();
//                break;
//        }
    }

    serialOut.println();
}

} // namespace ArduinoShell

