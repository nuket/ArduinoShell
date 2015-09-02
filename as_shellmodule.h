// -----------------------------------------------------------------------
// ShellModule
//
// Defines a set of functionality, and corresponding EEPROM configuration
// management.
// -----------------------------------------------------------------------

#include <Arduino.h>

class ShellModule
{
public:
    //! Module initialization code goes here.
    virtual void setup();

    //! Help string explaining commands goes here.
    virtual void help();

    //! Command processing and execution goes here.
    virtual void run(String rawCommand);

    //! Default settings can be persisted here in an arbitrary format.
    virtual void saveDefaults();

    //! Default settings are loaded here.
    // virtual void loadDefaults();
};

