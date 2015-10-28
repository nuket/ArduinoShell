# -----------------------------------------------------------------------
# Useful script for invoking clang against Arduino source code to take
# advantage of clang's analysis tools.
# 
# Currently only works on OS X with an Xcode installation, but probably
# would work on Linux with some tweaking.
#
# Already picked up some errors:
# ./clang-check.sh ConfigBlock.cpp
#
# In file included from ConfigBlock.cpp:27:
# /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/EEPROM/EEPROM.h:42:69: warning: cast to 'unsigned char *' from smaller integer
#       type 'int' [-Wint-to-pointer-cast]
#     uint8_t operator*() const            { return eeprom_read_byte( (uint8_t*) index ); }
#                                                                     ^
# /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/EEPROM/EEPROM.h:47:70: warning: cast to 'unsigned char *' from smaller integer
#       type 'int' [-Wint-to-pointer-cast]
#     EERef &operator=( uint8_t in )       { return eeprom_write_byte( (uint8_t*) index, in ), *this;  }
#                                                                      ^
# ConfigBlock.cpp:69:1: warning: control reaches end of non-void function [-Wreturn-type]
# }
# ^
# -----------------------------------------------------------------------

HARD=/Applications/Arduino.app/Contents/Java/hardware

clang -I"${HARD}/tools/avr/avr/include" -I"${HARD}/arduino/avr/cores/arduino" -I"${HARD}/arduino/avr/variants/standard" -I"${HARD}/arduino/avr/libraries/EEPROM" \
-Os \
-D__AVR_ATmega328P__ \
-DARDUINO=10605 \
-DF_CPU=16000000L \
-std=c++11 \
-Wno-attributes \
-fno-asm \
$@

# -fsyntax-only -Os \

