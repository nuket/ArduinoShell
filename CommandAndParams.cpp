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

namespace ArduinoShell
{

CommandAndParams::CommandAndParams(String rawCommand, Stream& serialOut) :
    commandUsable(false),
    paramCount(0),
    serialOut(serialOut)
{
    // Trim whitespace in place.
    rawCommand.trim();

    // Check that the command was more than just whitespace.
    if (rawCommand.length() == 0) return;

    // At least a command is available, maybe parameters are not.
    commandUsable = true;

    int spaceA = rawCommand.indexOf(' ');
    int spaceB = -1;

    // .indexOf returns -1 if character is not found.
    if (spaceA == -1)
    {
        command = rawCommand;
        return; // Return early, no parameters to be parsed.
    }
    else
    {
        command = rawCommand.substring(0, spaceA);
    }

    for (unsigned int i = 0; i < MAX_PARAMS; i++)
    {
        spaceA++;
        spaceB = rawCommand.indexOf(' ', spaceA);

//        serialOut.println(spaceA);
//        serialOut.println(spaceB);

        if (spaceB == -1)
        {
            params[i] = rawCommand.substring(spaceA);

            // EOL reached.
            paramCount = i + 1;
            break;
        }
        else
        {
            params[i] = rawCommand.substring(spaceA, spaceB);
            spaceA = spaceB;
        }
    }
}

void CommandAndParams::print()
{
    serialOut.print(F("command:  >>"));
    serialOut.print(command);
    serialOut.println(F("<<"));
    
    for (int i = 0; i < paramCount; i++)
    {
        serialOut.print(F("param["));
        serialOut.print(i);
        serialOut.print(F("]: >>"));
        serialOut.print(params[i]);
        serialOut.println(F("<<"));
    }
}

#if ENABLE_UNIT_TESTS
static void test_command_and_params_class()
{
    serialOut.println(F("CommandAndParams unit test starting."));
    
    CommandAndParams testA("test 1 2 3");
    testA.print();
    assert(String("test").equals(testA.command));
    assert(testA.commandUsable);
    assert(String("1").equals(testA.params[0]));
    assert(String("2").equals(testA.params[1]));
    assert(String("3").equals(testA.params[2]));
    assert(testA.paramCount == 3);

    CommandAndParams testB("wb 0x10 0x41"); // write the byte 'a' to position 0x10.
    testB.print();
    assert(String("wb").equals(testB.command));
    assert(testB.commandUsable);
    assert(String("0x10").equals(testB.params[0]));
    assert(String("0x41").equals(testB.params[1]));
    assert(testB.paramCount == 2);
    
    serialOut.println("CommandAndParams unit test ending OK.");
}
#endif

} // namespace ArduinoShell

