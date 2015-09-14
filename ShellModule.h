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

#ifndef __SHELLMODULE_H__
#define __SHELLMODULE_H__

#include <Arduino.h>

/**
 * Declares the interface that all shell modules implement.
 */
class ShellModule
{
public:
    /**
     * \brief Module initialization code goes here.
     * 
     * Setup actions specific to the module should be handled by
     * the derived class, though this has an empty default
     * implementation in case nothing specific needs to happen.
     */
    virtual void setup() {}

    /**
     * \brief Module help string goes here.
     * 
     * This method can either choose to print the help string itself, 
     * or pass back a readonly reference that can be used by the caller,
     * or both.
     * 
     * Method is mandatory.
     */
    virtual const String& help() = 0;

    /**
     * \brief Command processing and execution goes here.
     * 
     * Method is mandatory.
     */
    virtual void run(String rawCommand) = 0;

    /**
     * \brief Default settings can be persisted here in an arbitrary format.
     */
    // virtual void saveDefaults();

    //! Default settings are loaded here.
    // virtual void loadDefaults();
};

#endif // __SHELLMODULE_H__
