#include <Arduino.h>

struct CommandAndParams
{
    String command;
    bool   commandUsable;
    
    enum { MAX_PARAMS = 8 };
    String params[MAX_PARAMS];

    // Number of parameters parsed.
    uint8_t paramCount;

    CommandAndParams(String rawCommand);
    void print();
};

