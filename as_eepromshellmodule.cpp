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

#include "as_commandandparams.h"
#include "as_eepromshellmodule.h"

#include <EEPROM.h>

#if ENABLE_UNIT_TESTS
#include <assert.h>
#endif

#define EEPROM_ROW_LENGTH 16
#define OUTPUT_ROW_LENGTH 80

EepromShellModule::EepromShellModule(Stream& serialOut) :
    serialOut(serialOut)
{
}

void EepromShellModule::setup()
{
    serialOut.print("Size of EEPROM: ");
    serialOut.print(EEPROM.length());
    serialOut.println(" bytes.");
}

const String& EepromShellModule::help()
{
    serialOut.println(__func__);
}

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
void EepromShellModule::printContents(uint32_t startAddress, uint32_t length)
{
    // Quick check that range is valid.
    if (startAddress >= EEPROM.length())
    {
        char output[OUTPUT_ROW_LENGTH] = {'\0'};
        snprintf(output, OUTPUT_ROW_LENGTH, "Address (0x%04x) is out of EEPROM bounds [0, %04x].", startAddress, EEPROM.length());
        serialOut.println(output);
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

        serialOut.println(outputRow);
    }
}

void EepromShellModule::run(String rawCommand)
{
    // Echo the command.
    serialOut.println(rawCommand);

    // Parse it.
    CommandAndParams cp(rawCommand);
    // cp.print();

    if (cp.command.equals("p"))
    {
        if (cp.paramCount == 0)
        {
            // Example: p
            //
            // Print entire EEPROM.
            printContents(0, EEPROM.length());
        }
        else
        if (cp.paramCount == 1)
        {
            // Example: p 0x10
            //
            // Print row containing address.
            long int address = strtol(cp.params[0].c_str(), NULL, 0);
            printContents(address & 0xFFF0, EEPROM_ROW_LENGTH);
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

                printContents(address, rows * EEPROM_ROW_LENGTH);
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

        EEPROM.put(address, data);
        printContents(address & 0xFFF0, EEPROM_ROW_LENGTH);
    }
    else
    if (cp.command.equals("clear-eeprom") == 0)
    {
    }
}

