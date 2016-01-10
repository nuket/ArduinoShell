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

#ifndef __CONFIGSHELLMODULE_H__
#define __CONFIGSHELLMODULE_H__

#include "ShellModule.h"

class Stream;

namespace ArduinoShell
{

class ConfigBlock;

class ConfigShellModule : public ShellModule
{
private:
    ConfigBlock& configBlock;
    Stream&      serialOut;
public:
    ConfigShellModule(ConfigBlock& configBlock, Stream& serialOut);

    const String& help();
    void run(String rawCommand);

    /**
     * \brief Print out the contents of the ConfigBlock data in a human-readable form.
     */
    void print();
};

} // namespace ArduinoShell

#endif // __CONFIGSHELLMODULE_H__
