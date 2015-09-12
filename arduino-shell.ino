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

// #define ENABLE_UNIT_TESTS 0

#include "as_commandandparams.h"
#include "as_crc.h"
#include "as_digitalpinshellmodule.h"
#include "as_eepromshellmodule.h"

// Arduino IDE issue:
// This include has to remain in place, otherwise for some reason
// the compiler cannot find it for other .cpp translation units 
// that rely on it. :(
#include <EEPROM.h>

// -----------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------

DigitalPinShellModule digitalPinShell(0x0100);
EepromShellModule     eepromShell(Serial);

// -----------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------

void setup() 
{
    Serial.begin(115200);

    Serial.println("Arduino Shell");
    Serial.println("(c)2015 Max Vilimpoc (https://github.com/nuket/arduino-shell), MIT licensed.");
    Serial.println("");

    eepromShell.setup();
    digitalPinShell.setup();

#if ENABLE_UNIT_TESTS
    test_command_and_params_class();
#endif
}

void loop() 
{
    const char TERMINATOR = '\n';
    
    // Read and process commands.
    if (millis() % 1000 == 0 && Serial.available())
    {
        String command = Serial.readStringUntil(TERMINATOR);
        
        // digitalPinShell.run(command);
        eepromShell.run(command);
    }
}
