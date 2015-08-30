# Arduino Shell
A command shell to interact with built-in Arduino features via the Serial Monitor.

Right now: Offers commands to print EEPROM contents in a hexdump-like form, and to write 
bytes into EEPROM. Other commands will be added, as needed.

## Startup

At startup, the Arduino Shell will print the full contents of EEPROM.

## Command Examples

### `wb address byte`

Write the `byte` to the specified EEPROM `address`.

```
wb 0x51 0x45
command:  >>wb<<
param[0]: >>0x51<<
param[1]: >>0x45<<
0050 | FF 45 FF FF FF FF FF FF FF FF FF FF FF FF FF FF | .E..............      
```

### `p address rows`

Print `rows` rows, starting from EEPROM `address`.

```
p 0x00 10
command:  >>p<<
param[0]: >>0x00<<
param[1]: >>10<<
0000 | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | ................      
0010 | 42 41 43 FF FF FF FF FF FF FF FF FF FF FF FF FF | BAC.............      
0020 | 47 51 FF FF FF FF FF FF FF FF FF FF FF FF FF FF | GQ..............      
0030 | 57 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | W...............      
0040 | FF FF FF FF FF FF FF FF 44 FF FF FF FF FF FF FF | ........D.......      
0050 | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | ................      
0060 | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | ................      
0070 | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | ................      
0080 | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | ................      
0090 | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF | ................      
```

# License

```
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
```
