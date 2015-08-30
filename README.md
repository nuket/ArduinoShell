# arduino-shell
A command shell to interact with built-in Arduino features via the Serial Monitor.

Right now: Offers commands to print EEPROM contents in a hexdump-like form, and to write 
bytes into EEPROM. Other commands will be added, as needed.

## Startup

## Command Examples

### wb <address> <byte>

Write the <byte> to the specified <address>.

```
wb 0x51 0x45
command:  >>wb<<
param[0]: >>0x51<<
param[1]: >>0x45<<
0050 | FF 45 FF FF FF FF FF FF FF FF FF FF FF FF FF FF | .E..............      
```

### p <address> <rows>

Print <rows> rows, starting from <address>.

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
