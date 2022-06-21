//      >>>>>  T-I-N-Y  B-E-R-T for ATTINY85  GPL v3 <<<<
//                   Programmer: Daniel C 2019-2021
//              Contact EMAIL: electro_l.i.b@tinyjoypad.com
//                     https://www.tinyjoypad.com   
//           https://sites.google.com/view/arduino-collection

//  Tiny Bert is free software: you can redistribute it and/or modify
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

#include "../video/ssd1306xled.h"

namespace {

//prototype
void TinyOLED_init(void);
void TinyOLED_Begin(void);
void TinyOLED_End(void);
void TinyOLED_Send(uint8_t byte_);
void TinyOLED_send_command(uint8_t command_);
void TinyOLED_Data_Start(uint8_t Y_);
                   
#define OledSDA_LOW  0b11111110
#define OledSDA_HIGH 0b00000001
#define OledSCL_LOW 0b11111011
#define OledSCL_HIGH 0b00000100
#define OledADRESS 0x78

void TinyOLED_init(void){
DDRB = DDRB | OledSDA_HIGH;
DDRB = DDRB | OledSCL_HIGH;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//According to the graphics library of your choice, insert the code here to initialize the oled ssd1306 screen//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void TinyOLED_Begin(void){
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////According to the graphic library of your choice, insert the code here to start the communication////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void TinyOLED_End(void){
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////According to the graphics library of your choice, insert the code here to stop communication//////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SSD1306.ssd1306_send_data_stop();
}

void TinyOLED_Send(uint8_t byte_){
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////According to the graphics library of your choice, insert the code here to send the data//////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    SSD1306.ssd1306_send_byte(byte_);
}

void TinyOLED_send_command(uint8_t command_){
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////According to the graphics library of your choice, insert the code here to send a command////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void TinyOLED_Data_Start(uint8_t Y_){
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////According to the graphics library of your choice, insert the code here to initiate the sending of data/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  SSD1306.ssd1306_send_command(0xb0 + Y_); // page0 - page1
  SSD1306.ssd1306_send_command(0x00);   // low column start address
  SSD1306.ssd1306_send_command(0x10);   // high column start address

    SSD1306.ssd1306_send_data_start();
}


} // anon namespace