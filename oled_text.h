
#include<stdint.h>
#include "src/video/ssd1306xled.h"

namespace oled_text {

void ssd1306_char_f6x8(SSD1306Device oled, uint8_t x, uint8_t y, const char ch[]);
void doNumber (SSD1306Device oled, uint8_t x, uint8_t y, int value);


}
