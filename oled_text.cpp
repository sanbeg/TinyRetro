#include "oled_text.h"
#include "font6x8AJ2.h"

namespace oled_text {


void ssd1306_char_f6x8(SSD1306Device oled, uint8_t x, uint8_t y, const char ch[]) {
  uint8_t c, i, j = 0;
  while (ch[j] != '\0')
  {
    c = ch[j] - 32;
    if (c > 0) c = c - 12;
    if (c > 15) c = c - 6;
    if (c > 40) c = c - 9;
    if (x > 126)
    {
      x = 0;
      y++;
    }
    oled.ssd1306_setpos(x, y);
    oled.ssd1306_send_data_start();
    for (i = 0; i < 6; i++)
    {
      oled.ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font6x8[c * 6 + i]));
    }
    oled.ssd1306_send_data_stop();
    x += 6;
    j++;
  }
}

void doNumber (SSD1306Device oled, int x, int y, int value) {
  char temp[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  itoa(value, temp, 10);
  ssd1306_char_f6x8(oled, x, y, temp);
}

}
