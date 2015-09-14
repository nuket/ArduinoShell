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

/**
 * \file
 */

#ifndef __COMMANDANDPARAMS_H__
#define __COMMANDANDPARAMS_H__

#include <Arduino.h>

namespace ArduinoShell
{

/**
 * \class CommandAndParams
 * \brief Simple container to parse a command and parameters out of a String.
 */
struct CommandAndParams
{
    String command;                        //!< The command to be performed.
    bool   commandUsable;                  //!< Was a command available in the input string?
    
    enum { MAX_PARAMS = 8 };

    String  params[MAX_PARAMS];            //!< Up to MAX_PARAMS parsed parameters are stored here.
    uint8_t paramCount;                    //!< Number of parameters actually parsed.

    Stream& serialOut;                     //!< Debug output is printed here.

    /**
     * Ctor.
     * 
     * \param rawCommand  A raw string to parse for a command and parameters.
     */
    CommandAndParams(String rawCommand, Stream& serialOut);

    /**
     * Prints the parsed command and params.
     * Uses #serialOut as its target.
     */
    void print();
};

} // namespace ArduinoShell

#endif // __COMMANDANDPARAMS_H__
