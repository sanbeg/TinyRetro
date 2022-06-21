//        >>>>>  T-I-N-Y  M-O-R-P-I-O-N for ATTINY85  GPL v3 <<<<<
//                    Programmer: Daniel C 2020-2021
//             Contact EMAIL: electro_l.i.b@tinyjoypad.com
//                    https://WWW.TINYJOYPAD.COM
//          https://sites.google.com/view/arduino-collection

//  tiny-Morpion is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//for TINY JOYPAD rev2 (attiny85)
//the code work at 16MHZ internal
//Program the chip with an arduino uno in "Arduino as ISP" mode.

#include <avr/pgmspace.h>

#define TINYJOYPAD_LEFT  (analogRead(A0)>=750)&&(analogRead(A0)<950)
#define TINYJOYPAD_RIGHT (analogRead(A0)>500)&&(analogRead(A0)<750)
#define TINYJOYPAD_DOWN (analogRead(A3)>=750)&&(analogRead(A3)<950)
#define TINYJOYPAD_UP  (analogRead(A3)>500)&&(analogRead(A3)<750)
#define BUTTON_DOWN (digitalRead(1)==0)
#define BUTTON_UP (digitalRead(1)==1)


const uint8_t  CPU_RNDALT_TMORPION [] PROGMEM= {
  2,6,8,0,
  0,8,6,2,
  6,2,0,8,
  8,0,2,6,
};

const uint8_t  LINE_CHECK_TMORPION [] PROGMEM= {
0b00011000,0b01000010,
0b10000001,0b00100100,
0b11100000,0b00000111,
0b00101001,0b10010100, 
};

const uint8_t  CPU_CHECK_MIDDLE_TMORPION [] PROGMEM= {
0b10000001,0b01011010,
0b00100100,0b01011010,
0b01010000,0b10000000,
0b01001000,0b00100000,
0b00001010,0b00000001,
0b00010010,0b00000100,
0b01000001,0b00100000,
0b01000100,0b10000000,
0b10001000,0b00100000,
0b00001100,0b00000001,
0b00100010,0b00000001,
0b10000010,0b00000100,
0b00110000,0b10000000,
0b00010001,0b00000100,
0b10000000,0b00000001,
0b00100000,0b00000100,
0b00000100,0b00100000,
0b00000001,0b10000000,
};

const uint8_t  police_TMORPION [] PROGMEM= {
4,1,
0xF8,0x88,0xF8,0x00,0x00,0xF8,0x00,0x00,0xE8,0xA8,0xB8,0x00,0x88,0xA8,0xF8,0x00,0x38,0x20,0xF8,0x00,
0xB8,0xA8,0xE8,0x00,0xF8,0xA8,0xE8,0x00,0x08,0xE8,0x18,0x00,0xF8,0xA8,0xF8,0x00,0xB8,0xA8,0xF8,0x00,
};

const uint8_t  PLATEAU_UP_TMORPION [] PROGMEM= {
0xFE,0x01,0xFC,0xFE,0xFE,0xFE,0x06,0xD6,0xC6,0xFE,0x06,0x7E,0x7E,0xF6,0xEE,0x1E,0xE6,0xFE,0xFE,0xFE,
0x76,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
0xFC,0x01,0xFE,0x00,0x00,0xFE,0x01,0xFC,0xFE,0xFE,0xFE,0x06,0x76,0x76,0xFE,0x06,0xD6,0xC6,0xFE,0x06,
0x7E,0x06,0xFE,0xFE,0xFE,0x76,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
0xFE,0xFE,0xFE,0xFE,0xFE,0xFC,0x01,0xFE,0x03,0x04,0x09,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,
0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,
0x2B,0x1B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,
0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x19,0x0C,0x07,0x00,0x00,0x03,0x04,0x09,0x1B,0x2B,0x1B,0x2B,
0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,
0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x1B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,
0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x2B,0x3B,0x19,0x0C,0x07,};

const uint8_t  PLATEAU_TMORPION [] PROGMEM= {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xE0,0x60,
0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xB0,0x70,0x30,0x30,0x30,
0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x70,0xB0,0x30,0x30,0x30,0x30,0x30,0x30,
0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x60,0xE0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0x78,
0x7C,0x5E,0x4F,0x47,0x43,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
0x40,0xE0,0x5C,0x43,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
0x40,0x43,0x5C,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x41,
0x43,0x47,0x4F,0x5E,0x7C,0x78,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0x78,
0x3C,0x1E,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x38,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x38,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1E,
0x3C,0x78,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0x78,
0x3C,0x1E,0x0F,0x07,0x03,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x82,0x72,0x0E,0x03,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x03,0x0E,0x72,0x82,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x03,0x07,0x0F,0x1E,0x3C,0x78,0xF0,0xE0,
0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xE0,0xF0,0xF8,
0xFC,0x9E,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x1C,0x03,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1C,0xE0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x9E,0xFC,0xF8,0xF0,0xE0,0x40,0x00,0x00,0x00,
0x00,0x00,0x00,0x3F,0x42,0xFC,0x85,0xF9,0xEB,0xAB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,
0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,
0xEB,0xEB,0xEB,0xEB,0xE3,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,
0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,
0xEB,0xEB,0xEB,0xEB,0xE3,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,
0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xEB,0xAB,
0xEB,0xF9,0x85,0xFC,0x42,0x3F,0x00,0x00,
};

const uint8_t  UP_POS_TMORPION [] PROGMEM= {36,56,75};

const uint8_t  MIDDLE_POS_TMORPION [] PROGMEM= {25,52,78};

const uint8_t  DOWN_POS_TMORPION [] PROGMEM= {11,47,82};

const uint8_t  UP_TMORPION [] PROGMEM= {
0x00,0xC0,0xE0,0x60,0x30,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0x30,0x30,0x00,0x00,0x45,
0x4E,0x4F,0x5F,0x5F,0x5D,0x59,0x58,0x58,0x58,0x58,0x5D,0x4F,0x4F,0x47,0x47,0x00,0x00,0x30,0x30,0x30,
0x30,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x46,0x4F,0x4F,0x4F,0x5D,
0x59,0x58,0x58,0x58,0x59,0x5D,0x4F,0x4F,0x4F,0x46,0x00,0x00,0x00,0x30,0x30,0xB0,0xB0,0xB0,0xB0,0xB0,
0xB0,0xB0,0xB0,0xB0,0xB0,0x30,0x60,0xE0,0xC0,0x00,0x00,0x47,0x47,0x4F,0x4F,0x5D,0x58,0x58,0x58,0x58,
0x59,0x5D,0x5F,0x5F,0x4F,0x4E,0x45,0x00,0xC0,0xC0,0xE0,0x60,0x30,0xB0,0xB0,0xB0,0x30,0x30,0x30,0x30,
0x30,0xB0,0xB0,0xB0,0xB0,0xB0,0x53,0x59,0x58,0x48,0x4C,0x4C,0x45,0x47,0x47,0x47,0x47,0x4F,0x5F,0x5D,
0x59,0x59,0x40,0x60,0x70,0x30,0xB0,0xB0,0xB0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xB0,0xB0,0xB0,0x30,
0x70,0x00,0x50,0x58,0x58,0x5D,0x4D,0x4F,0x47,0x47,0x47,0x47,0x47,0x4F,0x4D,0x5D,0x58,0x58,0x50,0x00,
0xB0,0xB0,0xB0,0xB0,0xB0,0x30,0x30,0x30,0x30,0x30,0xB0,0xB0,0xB0,0x30,0x60,0xE0,0xC0,0xC0,0x60,0x40,
0x59,0x59,0x5D,0x5F,0x4F,0x47,0x47,0x47,0x47,0x45,0x4C,0x4C,0x48,0x58,0x59,0x53,0xC0,0xC0,0xE0,0x60,
0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x53,0x49,0x54,0x4A,0x55,0x4A,
0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x45,0x60,0x70,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,
0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0x70,0x00,0x48,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,
0x4A,0x55,0x4A,0x55,0x4A,0x55,0x48,0x00,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,0xB0,0x30,
0xB0,0x30,0x60,0xE0,0xC0,0xC0,0x60,0x45,0x4A,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,0x4A,0x55,
0x4A,0x54,0x49,0x53,
};

const uint8_t  MIDDLE_TMORPION [] PROGMEM= {
0x00,0x00,0x00,0x13,0x3D,0x7C,0x7C,0x7E,0xFE,0xE6,0xE7,0xE3,0xE3,0xE3,0xE3,0xE3,0xE3,0x63,0x77,0x7F,
0x7E,0x3E,0x3E,0x1C,0x08,0x00,0x00,0x38,0x3C,0x7C,0x7E,0x7E,0xFE,0xE6,0xE7,0xE3,0xC3,0xC3,0xC3,0xC3,
0xC3,0xE3,0xE7,0xE6,0xFE,0x7E,0x7E,0x7C,0x3C,0x38,0x00,0x00,0x00,0x08,0x1C,0x3E,0x3E,0x7E,0x7F,0x77,
0x63,0xE3,0xE3,0xE3,0xE3,0xE3,0xE3,0xE7,0xE6,0xFE,0x7E,0x7C,0x7C,0x3D,0x13,0x00,0x00,0x00,0x9E,0xCF,
0xC7,0xE3,0x61,0x60,0x71,0x33,0x33,0x37,0x3F,0x1E,0x1E,0x1E,0x3C,0x3C,0x7E,0x7E,0xF6,0xF6,0xE3,0xE3,
0xE3,0xC3,0x01,0xC1,0x87,0xC0,0xC0,0xE1,0xE3,0xE3,0x73,0x77,0x36,0x3E,0x3E,0x1C,0x1C,0x1C,0x3E,0x3E,
0x36,0x77,0x73,0xE3,0xE3,0xE1,0xC0,0xC0,0x87,0x00,0xC1,0x01,0xC3,0xE3,0xE3,0xE3,0xF6,0xF6,0x7E,0x7E,
0x3C,0x3C,0x1E,0x1E,0x1E,0x3F,0x37,0x33,0x33,0x71,0x60,0x61,0xE3,0xC7,0xCF,0x9E,0x9E,0x4F,0xA7,0x53,
0xA9,0x54,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,
0x0A,0xC1,0x87,0x50,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,
0xAA,0x55,0xAA,0x55,0xAA,0x50,0x87,0x00,0xC1,0x0A,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,
0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x54,0xA9,0x53,0xA7,0x4F,0x9E,
};

const uint8_t  DOWN_TMORPION [] PROGMEM= {
0x00,0x00,0x00,0x00,0x78,0x3C,0x9E,0xCE,0xC6,0xE2,0xE2,0xE2,0xF2,0x72,0x72,0x72,0x3A,0x3A,0x3A,0x3A,
0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x7A,0xFA,0xF2,0xF2,0xF2,0xE2,0xE2,0xC2,0x82,0x00,0x00,0x00,0x00,0x00,
0x06,0x0F,0x1F,0x1F,0x1F,0x3F,0x3F,0x3F,0x7F,0x7C,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x38,0x38,0x3C,
0x3C,0x3C,0x3E,0x1F,0x1F,0x0F,0x0F,0x07,0x07,0x03,0x61,0x00,0x00,0x00,0x8E,0xC2,0xE2,0xE2,0xF2,0xF2,
0xF2,0xFA,0x7A,0x7A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,0x7A,0x7A,0xFA,0xF2,0xF2,
0xF2,0xE2,0xE2,0xC2,0x8E,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x0F,0x1F,0x1F,0x3F,0x3F,0x3F,0x7E,0x7C,
0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x7C,0x7E,0x3F,0x3F,0x3F,0x1F,0x1F,0x0F,0x07,
0x07,0x00,0x00,0x00,0x00,0x82,0xC2,0xE2,0xE2,0xF2,0xF2,0xF2,0xFA,0x7A,0x3A,0x3A,0x3A,0x3A,0x3A,0x3A,
0x3A,0x3A,0x3A,0x3A,0x72,0x72,0x72,0xF2,0xE2,0xE2,0xE2,0xC6,0xCE,0x9E,0x3C,0x78,0x00,0x00,0x00,0x00,
0x00,0x61,0x03,0x07,0x07,0x0F,0x0F,0x1F,0x1F,0x3E,0x3C,0x3C,0x3C,0x38,0x38,0x78,0x78,0x78,0x78,0x78,
0x78,0x78,0x7C,0x7F,0x3F,0x3F,0x3F,0x1F,0x1F,0x1F,0x0F,0x06,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,
0x78,0x3C,0x1E,0x0E,0x06,0x1A,0x3A,0x3A,0x7A,0x7A,0xFA,0xF2,0xF2,0xE2,0xE2,0xE2,0xC2,0xC2,0xC2,0xE2,
0xE2,0xE2,0xE2,0x72,0x72,0x72,0x3A,0x3A,0x3A,0x1A,0x1A,0x1A,0x47,0x63,0x71,0x70,0x78,0x78,0x38,0x3C,
0x3C,0x1C,0x1C,0x1E,0x0E,0x0E,0x0F,0x07,0x07,0x07,0x03,0x07,0x07,0x0F,0x0F,0x1F,0x1F,0x3E,0x3E,0x7E,
0x7C,0x7C,0x78,0x78,0x70,0x00,0xE0,0x1C,0x00,0x00,0x0E,0x02,0x02,0x1A,0x1A,0x3A,0x3A,0x3A,0x7A,0x72,
0xF2,0xF2,0xE2,0xE2,0xE2,0xC2,0xE2,0xE2,0xE2,0xF2,0xF2,0x72,0x7A,0x3A,0x3A,0x3A,0x1A,0x1A,0x02,0x02,
0x0E,0x00,0x00,0x00,0x00,0x00,0x70,0x70,0x78,0x78,0x7C,0x3C,0x3C,0x3E,0x1E,0x1E,0x0F,0x0F,0x07,0x07,
0x07,0x03,0x07,0x07,0x07,0x0F,0x0F,0x1E,0x1E,0x3E,0x3C,0x3C,0x7C,0x78,0x78,0x70,0x70,0x00,0x00,0x00,
0x1A,0x1A,0x1A,0x3A,0x3A,0x3A,0x72,0x72,0x72,0xE2,0xE2,0xE2,0xE2,0xC2,0xC2,0xC2,0xE2,0xE2,0xE2,0xF2,
0xF2,0xFA,0x7A,0x7A,0x3A,0x3A,0x1A,0x06,0x0E,0x1E,0x3C,0x78,0xF0,0xE0,0xC0,0x80,0x1C,0xE0,0x00,0x70,
0x78,0x78,0x7C,0x7C,0x7E,0x3E,0x3E,0x1F,0x1F,0x0F,0x0F,0x07,0x07,0x03,0x07,0x07,0x07,0x0F,0x0E,0x0E,
0x1E,0x1C,0x1C,0x3C,0x3C,0x38,0x78,0x78,0x70,0x71,0x63,0x47,0x80,0xC0,0xE0,0xF0,0x78,0x3C,0x9E,0x4E,
0xA6,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,
0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x12,0x27,0x53,0x29,0x54,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,
0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,
0x2A,0x05,0x60,0x1C,0x00,0x72,0x0E,0x42,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,
0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x42,0x0E,0x72,0x00,0x00,
0x00,0x50,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,
0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x50,0x00,0x00,0x12,0xAA,0x52,0xAA,
0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,0x52,0xAA,
0x52,0xAA,0x52,0xA6,0x4E,0x9E,0x3C,0x78,0xF0,0xE0,0xC0,0x80,0x1C,0x60,0x05,0x2A,0x55,0x2A,0x55,0x2A,
0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,0x55,0x2A,
0x55,0x2A,0x55,0x2A,0x54,0x29,0x53,0x27,
};

const uint8_t  menu_TMORPION [] PROGMEM= {
20,3,
0x00,0x9F,0x15,0x91,0x00,0x9F,0x85,0x9F,0x00,0x97,0x95,0x9D,0x00,0x93,0x94,0x0F,0x00,0x00,0x00,0x00,
0x40,0xCF,0x42,0xCF,0x00,0xCF,0x42,0xCF,0x00,0xCF,0x46,0xCB,0x00,0xCF,0x08,0xC7,0x00,0xC0,0x40,0x40,
0x04,0x07,0x05,0x07,0x00,0x07,0x03,0x05,0x00,0x07,0x01,0x07,0x00,0x03,0x04,0x03,0x00,0x07,0x05,0x04,
};

const uint8_t  CURSEUR_TMORPION [] PROGMEM= {
25,2,
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0xFF,0xFE,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,};

const uint8_t  intro_TMORPION [] PROGMEM= {
41,5,
0xFE,0x03,0x01,0x01,0x01,0x01,0x11,0x21,0x41,0x81,0x41,0x21,0x11,0x01,0xF1,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0xF1,0x01,0x11,0x21,0x41,0x81,0x41,0x21,0x11,0x01,0x01,0x01,0x03,0xFE,
0xFC,0xFF,0x00,0x00,0x00,0x00,0x00,0x14,0x12,0x11,0x10,0x11,0x12,0x14,0x10,0xFF,0x10,0x10,0x90,0x10,
0x10,0x10,0x10,0x10,0x90,0x10,0x10,0xFF,0x10,0x14,0x12,0x11,0x10,0x11,0x12,0x14,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x20,
0x11,0x0A,0x04,0x0A,0x11,0x20,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x40,0x40,0xE1,0x51,0x49,0x49,0x49,0x51,0xE1,0x41,0xFF,0x41,0x41,
0xE1,0x51,0x49,0x49,0x49,0x51,0xE1,0x41,0x41,0xFF,0x41,0xE1,0x51,0x49,0x49,0x49,0x51,0xE1,0x40,0x40,
0x00,0x00,0xFF,0xFF,0x0F,0x18,0x30,0x30,0x30,0x30,0x30,0x31,0x32,0x32,0x32,0x31,0x30,0x30,0x31,0x30,
0x30,0x30,0x31,0x32,0x32,0x32,0x31,0x30,0x30,0x30,0x31,0x30,0x30,0x31,0x32,0x32,0x32,0x31,0x30,0x30,
0x30,0x30,0x38,0x2F,0x1F,};
