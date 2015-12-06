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

#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>

class Task
{
public:
    void start() { runnable = true;  }
    void stop()  { runnable = false; }

    virtual void run() { main(); }
    virtual void main() = 0;
protected:
    bool     runnable        = false;
    uint32_t lastRun         = 0;     //!< When did last run occur, in ms?
    
    uint32_t timesRun        = 0;     //!< Total number of executions.
    uint32_t executionTimeUs = 0;     //!< Total microseconds used for execution.
};

class PeriodicTask : public Task
{
public:
    PeriodicTask(uint32_t period);

    void run();
private:
    uint32_t runAt;
    uint32_t period;
    bool     expectOverflow;
};

#endif // __TASK_H_
