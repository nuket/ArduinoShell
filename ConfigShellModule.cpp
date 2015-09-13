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
#include "ConfigShellModule.h"

namespace ArduinoShell 
{

ConfigShellModule::ConfigShellModule(ConfigBlock& configBlock, Stream& serialOut) :
    configBlock(configBlock),
    serialOut(serialOut)
{
}

void ConfigShellModule::run(String rawCommand)
{
    // Echo the command.
    serialOut.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand, serialOut);
    cp.print();

    if (!cp.command.equals("config")) return;

    
}

//void ConfigShellModule::cat()
//{
//    char output[80] = {0};
//    uint8_t type = 0;
//    
//    for (int i = 0; i < MAX_PINS; i++)
//    {
//        memset(output, 0, 80);
//        type = configBlock.data[i].type;
//        snprintf(output, 80, "Config for Pin %02d: %s", i, type < PinType::LAST_ENTRY ? PinTypeStrings[type] : "INVALID");
//        serialOut.println(output);
//    }
//}
    
} // namespace ArduinoShell

