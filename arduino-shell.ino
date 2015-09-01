/**
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

#define ENABLE_UNIT_TESTS 0

#include <EEPROM.h>
#include <stdlib.h>

#if ENABLE_UNIT_TESTS
#include <assert.h>
#endif

#define EEPROM_ROW_LENGTH 16
#define OUTPUT_ROW_LENGTH 80

// -----------------------------------------------------------------------
// CRC functions (generated)
// -----------------------------------------------------------------------

/**
 * \file crc.h
 * Functions and types for CRC checks.
 *
 * Generated on Tue Sep  1 23:48:57 2015,
 * by pycrc v0.8.3, https://pycrc.org
 * using the configuration:
 *    Width        = 16
 *    Poly         = 0x8005
 *    XorIn        = 0x0000
 *    ReflectIn    = True
 *    XorOut       = 0x0000
 *    ReflectOut   = True
 *    Algorithm    = table-driven
 *****************************************************************************/
// #ifndef __CRC_H__
// #define __CRC_H__

#include <stdlib.h>

// #ifdef __cplusplus
// extern "C" {
// #endif


/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algoritm-specific code, is desired.
 *****************************************************************************/
// #define CRC_ALGO_TABLE_DRIVEN 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 16 bits.
 *****************************************************************************/
typedef unsigned int crc_t;


/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param data         The data word to be reflected.
 * \param data_len     The width of \a data expressed in number of bits.
 * \return             The reflected data.
 *****************************************************************************/
crc_t crc_reflect(crc_t data, size_t data_len);


/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 *****************************************************************************/
#define crc_init()      (0x0000)


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(crc_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
#define crc_finalize(crc)      (crc ^ 0x0000)


// #ifdef __cplusplus
// }           /* closing brace for extern "C" */
// #endif

// #endif      /* __CRC_H__ */

/**
 * \file crc.c
 * Functions and types for CRC checks.
 *
 * Generated on Tue Sep  1 23:48:53 2015,
 * by pycrc v0.8.3, https://pycrc.org
 * using the configuration:
 *    Width        = 16
 *    Poly         = 0x8005
 *    XorIn        = 0x0000
 *    ReflectIn    = True
 *    XorOut       = 0x0000
 *    ReflectOut   = True
 *    Algorithm    = table-driven
 *****************************************************************************/
// #include "crc.h"     /* include the header file generated with pycrc */
// #include <stdlib.h>

/**
 * Static table used for the table_driven implementation.
 *****************************************************************************/
static const crc_t crc_table[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
};

/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param data         The data word to be reflected.
 * \param data_len     The width of \a data expressed in number of bits.
 * \return             The reflected data.
 *****************************************************************************/
crc_t crc_reflect(crc_t data, size_t data_len)
{
    unsigned int i;
    crc_t ret;

    ret = data & 0x01;
    for (i = 1; i < data_len; i++) {
        data >>= 1;
        ret = (ret << 1) | (data & 0x01);
    }
    return ret;
}


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(crc_t crc, const void *data, size_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int tbl_idx;

    while (data_len--) {
        tbl_idx = (crc ^ *d) & 0xff;
        crc = (crc_table[tbl_idx] ^ (crc >> 8)) & 0xffff;

        d++;
    }
    return crc & 0xffff;
}

// -----------------------------------------------------------------------
// Arduino Shell code.
// -----------------------------------------------------------------------

struct EepromRow
{
    uint8_t data[EEPROM_ROW_LENGTH];
};

static bool is_ascii(char c)
{
    // if ('a' <= c && c <= 'z') return true;
    // if ('A' <= c && c <= 'Z') return true;
    if (0x20 <= c && c <= 0x7e) return true;

    return false;
}

/**
 * Prints the contents of EEPROM from startAddress to startAddress + length.
 * 
 * Works best with multiples of EEPROM_ROW_LENGTH (16) byte-sized blocks.
 * 
 * Will not print less than EEPROM_ROW_LENGTH bytes, and will round down
 * to the next-lower block size.
 */
static void ec_print_eeprom_contents(uint32_t startAddress, uint32_t length)
{
    // Quick check that range is valid.
    if (startAddress >= EEPROM.length())
    {
        char output[OUTPUT_ROW_LENGTH] = {'\0'};
        snprintf(output, OUTPUT_ROW_LENGTH, "Address (0x%04x) is out of EEPROM bounds [0, %04x].", startAddress, EEPROM.length());
        Serial.println(output);
        return;
    }

    // Round to nearest row.
    startAddress &= 0xFFF0;
    
    uint16_t  rows = length / EEPROM_ROW_LENGTH;
    uint8_t   mod  = length % EEPROM_ROW_LENGTH; // How many bytes in the last row?

    static const char HEXVAL[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    EepromRow eepromRow;

    // Generate a hexdump string for the row.
    char outputRow[OUTPUT_ROW_LENGTH] = {0};

    for (int row = 0; row < rows; ++row)
    {
        int address = startAddress + row * EEPROM_ROW_LENGTH;
        EEPROM.get(address, eepromRow);

        memset(outputRow, 0x20, (OUTPUT_ROW_LENGTH - 1));
        
        outputRow[0] = HEXVAL[(address & 0xF000) >> 12];
        outputRow[1] = HEXVAL[(address & 0x0F00) >>  8];
        outputRow[2] = HEXVAL[(address & 0x00F0) >>  4];
        outputRow[3] = HEXVAL[(address & 0x000F)];

        outputRow[5] = '|';

        const uint8_t HEX_START = 7;

        for (int column = 0; column < EEPROM_ROW_LENGTH; ++column)
        {
            uint8_t msb = (eepromRow.data[column] & 0xF0) >> 4;
            uint8_t lsb =  eepromRow.data[column] & 0x0F;

            uint8_t linePos = HEX_START + (column * 3);

            outputRow[linePos]     = HEXVAL[msb];
            outputRow[linePos + 1] = HEXVAL[lsb];
        }

        outputRow[HEX_START + (EEPROM_ROW_LENGTH * 3)] = '|';

        const uint8_t ASCII_START = HEX_START + (EEPROM_ROW_LENGTH * 3) + 2;

        for (int column = 0; column < EEPROM_ROW_LENGTH; ++column)
        {
            outputRow[ASCII_START + column] = is_ascii(eepromRow.data[column]) ? eepromRow.data[column] : '.'; 
        }

        Serial.println(outputRow);
    }
}

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

CommandAndParams::CommandAndParams(String rawCommand) :
    commandUsable(false),
    paramCount(0)
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

//        Serial.println(spaceA);
//        Serial.println(spaceB);

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
    Serial.print("command:  >>");
    Serial.print(command);
    Serial.println("<<");
    
    for (int i = 0; i < paramCount; i++)
    {
        Serial.print("param[");
        Serial.print(i);
        Serial.print("]: >>");
        Serial.print(params[i]);
        Serial.println("<<");
    }
}

#if ENABLE_UNIT_TESTS
static void test_command_and_params_class()
{
    Serial.println("CommandAndParams unit test starting.");
    
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
    
    Serial.println("CommandAndParams unit test ending OK.");
}
#endif

/**
 * Handles an incoming string command, and does whatever it damn well
 * pleases.
 * 
 * You could use this to implement a chained command-processing scheme
 * with other modules, by registering this command handler in a list
 * of handlers.
 * 
 * (Someday, maybe, the Arduino environment will support projects.)
 * 
 * rawCommand passed by value, so we can work on a copy of it.
 */
void ec_handle_command(String rawCommand)
{
    // Echo the command.
    Serial.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand);
    cp.print();

    if (cp.command.equals("p"))
    {
        if (cp.paramCount == 0)
        {
            // Example: p
            //
            // Print entire EEPROM.
            ec_print_eeprom_contents(0, EEPROM.length());
        }
        else
        if (cp.paramCount == 1)
        {
            // Example: p 0x10
            //
            // Print row containing address.
            long int address = strtol(cp.params[0].c_str(), NULL, 0);
            ec_print_eeprom_contents(address & 0xFFF0, EEPROM_ROW_LENGTH);
        }
        else
        if (cp.paramCount == 2)
        {
            // Example: p 0x00 10
            //
            // Print the specified number of rows, starting from the row containing address.
            long int address = strtol(cp.params[0].c_str(), NULL, 0);
            long int rows    = strtol(cp.params[1].c_str(), NULL, 0);

            // Find number of rows between address and EEPROM.length();
            if (0 <= address && address < EEPROM.length())
            {
                int rowsLeft = (EEPROM.length() - address) >> 4;
                rows = ((rows < rowsLeft) ? rows : rowsLeft);

                ec_print_eeprom_contents(address, rows * EEPROM_ROW_LENGTH);
            }
        }
    }
    else
    if (cp.command.equals("wb") && cp.paramCount == 2)
    {
        // Example: wb 0x0010 0x41 
        // Writes the specified byte to the specified location.

        // Convert to long.
        long int address = strtol(cp.params[0].c_str(), NULL, 0);
        // Serial.println(address);

        if (!(0 <= address < EEPROM.length())) return;

        // Write the byte to the address.
        long int data = strtol(cp.params[1].c_str(), NULL, 0) & 0xFF;
        // Serial.println(data);

        EEPROM.write(address, data);
        ec_print_eeprom_contents(address & 0xFFF0, EEPROM_ROW_LENGTH);
    }
    else
    if (cp.command.equals("clear-eeprom") == 0)
    {
    }
}

// -----------------------------------------------------------------------
// ShellModule
//
// Defines a set of functionality, and corresponding EEPROM configuration
// management.
// -----------------------------------------------------------------------

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

// -----------------------------------------------------------------------
// DigitalPinShell module
//
// Manipulate the digital pins, and save the config to EEPROM.
// -----------------------------------------------------------------------

class DigitalPinShell : public ShellModule
{
public:
    static const uint32_t MODULE_ID = 0x4450534d; // 'DPSM' - used to identify the module config block in EEPROM.
    static const uint8_t  MAX_PINS  = 64;

    //! Specifies where in EEPROM the defaults are stored.
    const uint32_t configBase;
    
    void setup();
    
    void help() override;
    void run(String rawCommand) override;

    void saveDefaults() override;
    // void loadDefaults() override;

private:
    struct
    {
        uint32_t id;     // MODULE_ID needs to be stored there, or the config block SHOULD NOT BE USED.
        uint32_t crc;    // Currently using a 16-bit CRC, can shrink this field for other uses if needed.

        uint64_t io;     // INPUT = 0, OUTPUT = 1
        uint64_t pullup; // INPUT_PULLUP means set this = 1, when necessary.
        uint64_t value;  // HIGH or LOW.
    } config;

    void configPinMode (const uint8_t pin, const int mode);
    void configPinValue(const uint8_t pin, const int value);
public:
    DigitalPinShell(const uint32_t configBase);
};

DigitalPinShell::DigitalPinShell(const uint32_t configBase) :
    configBase(configBase)
{
}

void DigitalPinShell::setup()
{
    // Check the config block, and if the .id and .crc are valid,
    // load the defaults and apply them.
    EEPROM.get(configBase, config);

    // Calculate the CRC of the config block, minus the .crc field
    // i.e. clear .crc first, so that it is always 0 when doing the
    // calculation.
    const uint32_t storedCrc = config.crc;
    config.crc = 0;
    
    crc_t crc = crc_init();
    crc = crc_update(crc, (const uint8_t *) &config, sizeof(config));
    crc = crc_finalize(crc);

    // Serial.println(crc, HEX);

    if (MODULE_ID == config.id && crc == storedCrc)
    {
        
    }
}

void DigitalPinShell::help()
{
}

void DigitalPinShell::run(String rawCommand)
{
    // Echo the command.
    Serial.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand);
    cp.print();

    if (!cp.command.equals("pin")) return;

    // Examples: pin 8 in
    //           pin 8 out
    //           pin 8 in_pullup
    //           pin 8 high
    //           pin 8 low
    //           pin save
    if (cp.paramCount == 1)
    {
        if (String("save").equals(cp.params[0]))
        {
            saveDefaults();
        }
    }
    else
    if (cp.paramCount == 2)
    {
        // Set up I/O pins, turn them on or off (if set to output)
        if (String("all").equals(cp.params[0]))
        {
            if (String("out").equals(cp.params[1]))
            {
                for (int i = 0; i < MAX_PINS; i++)
                {
                    
                }
            }
        }
        else
        {
            long int pin = strtol(cp.params[0].c_str(), NULL, 0);
            // Serial.println(pin);

            if (String("in").equals(cp.params[1].c_str()))
            {
                configPinMode(pin, INPUT);
            }
            else
            if (String("out").equals(cp.params[1].c_str()))
            {
                configPinMode(pin, OUTPUT);
            }
            else
            if (String("in_pullup").equals(cp.params[1].c_str()))
            {
                configPinMode(pin, INPUT_PULLUP);
            }
            else
            if (String("high").equals(cp.params[1].c_str()))
            {
                configPinValue(pin, HIGH);
            }
            else
            if (String("low").equals(cp.params[1].c_str()))
            {
                configPinValue(pin, LOW);
            }
        }
    }

    // Save the pin configuration to EEPROM?
    // save();
}

void DigitalPinShell::saveDefaults()
{
    EEPROM.put(configBase, config);
}

//void DigitalPinShell::loadDefaults()
//{   
//}

void DigitalPinShell::configPinMode(const uint8_t pin, const int mode)
{
    uint64_t configBit = 1;

    // Quick exit.
    if (pin >= 64) return;

    // Figure out which bits need twiddling.
    configBit <<= pin;
    
    switch(mode)
    {
        case INPUT:
            config.io     &= ~configBit;
            config.pullup &= ~configBit;
            break;
        case OUTPUT:
            config.io     |=  configBit;
            config.pullup &= ~configBit;
            break;
        case INPUT_PULLUP:
            config.io     &= ~configBit;
            config.pullup |=  configBit;
            break;
    }

    Serial.print((uint32_t)  (config.io & 0xFFFFFFFF), HEX);
    Serial.print((uint32_t) ((config.io & 0xFFFFFFFF00000000) >> 32), HEX);

    pinMode(pin, mode);
}

void DigitalPinShell::configPinValue(const uint8_t pin, const int mode)
{
    uint64_t configBit = 1;

    // Quick exit.
    if (pin >= 64) return;
    if (!(HIGH == mode || LOW == mode)) return;

    // Figure out which bits represent current pin config.
    configBit <<= pin;
    
    // Pin has to be in OUTPUT mode to set HIGH / LOW.
    if (config.io & configBit)
    {
        if (mode == HIGH)
        {
            config.value |= configBit;
        }
        else
        {
            config.value &= ~configBit;
        }

        digitalWrite(pin, mode);
    }
}

// -----------------------------------------------------------------------
// AnalogPin module
// -----------------------------------------------------------------------

/**
 * Handles an incoming string command, and does whatever it damn well
 * pleases.
 */ 
void pin_handle_command(String rawCommand)
{
    // Echo the command.
    Serial.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand);
    cp.print();

    if (cp.command.equals("pin"))
    {
        // Examples: pin 8 out
        //           pin 8 in
        //           pin 8 off
        //           pin 8 on
        //
        // Set up I/O pins, turn them on or off (if set to output)

        // Save the pin configuration to EEPROM?
    }
}

void pin_config_save()
{
}

void pin_config_load()
{
}

// -----------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------

DigitalPinShell digitalPinShell(0x0100);

// -----------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------

void setup() 
{
    Serial.begin(115200);

    Serial.println("Arduino Shell");
    Serial.println("(c) 2015 Max Vilimpoc (https://github.com/nuket/arduino-shell), MIT licensed.");
    Serial.println("");

    Serial.print("Size of EEPROM: ");
    Serial.println(EEPROM.length());
    Serial.println("");

    ec_print_eeprom_contents(0, EEPROM.length());

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
        
        ec_handle_command(command);
        digitalPinShell.run(command);
    }
}
