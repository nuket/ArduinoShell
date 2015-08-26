/**
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

#include <EEPROM.h>
#include <string.h>

#define EEPROM_ROW_LENGTH 16
#define OUTPUT_ROW_LENGTH 80

struct EepromRow
{
    uint8_t data[EEPROM_ROW_LENGTH];
};

bool is_ascii(char c)
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
void printEepromContents(uint32_t startAddress, uint32_t length)
{
    uint16_t  rows = length / EEPROM_ROW_LENGTH;
    uint8_t   mod  = length % EEPROM_ROW_LENGTH; // How many bytes in the last row?

    static const char HEXVAL[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    EepromRow eepromRow;

    // Generate a hexdump string for the row.
    char outputRow[OUTPUT_ROW_LENGTH] = {0};

    for (int row = 0; row < rows; ++row)
    {
        int address = row * EEPROM_ROW_LENGTH;
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
            uint8_t msb =  eepromRow.data[column] & 0x0F;
            uint8_t lsb = (eepromRow.data[column] & 0xF0) >> 4;

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

void setup() 
{
    Serial.begin(115200);

    Serial.println("EEPROM Shell");
    Serial.println("(c) 2015 Max Vilimpoc (https://github.com/nuket/eeprom-shell), MIT licensed.");
    Serial.println("");

    Serial.print("Size of EEPROM: ");
    Serial.println(EEPROM.length());
    Serial.println("");

    printEepromContents(0, EEPROM.length());
}

void loop() 
{
    // Read serial command.
    
    
    // Commands to write strings to the EEPROM, dump the EEPROM.

}
