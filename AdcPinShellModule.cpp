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

#include "AdcPinShellModule.h"
#include "CommandAndParams.h"
#include "ConfigBlock.h"

namespace ArduinoShell
{

AdcPinShellModule::AdcPinShellModule(ConfigBlock& configBlock, Stream& serialOut) :
    configBlock(configBlock),
    serialOut(serialOut)
{
}

const String& AdcPinShellModule::help()
{
    serialOut.println(F("ADC Pin Shell commands:\r\n"
                        "    adc aref <reference>\r\n"
                        "    adc read <n>\r\n"));
}

void AdcPinShellModule::run(String rawCommand)
{
    // Parse it.
    CommandAndParams cp(rawCommand, serialOut);

    if (!cp.command.equals(F("adc"))) return;
    if (cp.paramCount != 2) return;

    if (cp.params[0].equals(F("aref")))
    {
        serialOut.println(F("Setting A/D converter voltage reference source."));
        if (cp.params[1].equals(F("default")))
        {
            analogReference(DEFAULT);
        }
        else
        if (cp.params[1].equals(F("internal")))
        {
            // analogReference(INTERNAL);
        }
        /*
        else
        if (cp.params[1].equals("1.1v"))
        {
            analogReference(INTERNAL1V1);
        }
        else
        if (cp.params[1].equals("2.56v"))
        {
            analogReference(INTERNAL2V56);
        }
        */
        else
        if (cp.params[1].equals(F("external")))
        {
            analogReference(EXTERNAL);
        }
    }
    else
    if (cp.params[0].equals(F("read")))
    {
        serialOut.println(F("Reading ADC pin value."));

        long int pin = strtol(cp.params[1].c_str(), NULL, 0);
        // serialOut.println(pin);

        if (!(0 <= pin && pin <= 64)) return; // ConfigBlock::MAX_ANALOG_PINS)

        serialOut.println(analogRead(pin));
    }
}

} // namespace ArduinoShell

