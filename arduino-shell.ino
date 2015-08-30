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
        char output[80] = {'\0'};
        snprintf(output, 80, "Address (0x%04x) is out of EEPROM bounds [0, %04x].", startAddress, EEPROM.length());
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
}

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
    }
}
