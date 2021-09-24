


#include "font6x8AJ2.h"

#include "ssd1306xled.h"

#include "Control.h"

const SSD1306Device oled;

void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[]) {
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

void clear() {
  oled.ssd1306_setpos(0, 0);

  for (int y = 0; y < 8; ++y) {
    //oled.ssd1306_setpos(0, y);
    for (int x = 0; x < 128; ++x) {
      oled.ssd1306_send_byte(0);
    }
  }
}

void show_title() {

  ssd1306_char_f6x8(0, 1, "Hello World.");
  ssd1306_char_f6x8(0, 2, "hacked i2c lib");
}

void setup() {
  // put your setup code here, to run once:

  control::setup();
  oled.ssd1306_init();
  //oled.ssd1306_fillscreen(0x0);

  clear();

  show_title();
}

int16_t dotx = 96;
int doty = 4;

void loop() {
  // put your main code here, to run repeatedly:

  char btn_text[7];
  control::debug(btn_text);
  ssd1306_char_f6x8(0, 5, btn_text);

  oled.ssd1306_setpos(dotx, doty);
  oled.ssd1306_send_data_start();
  oled.ssd1306_send_byte(0);
  oled.ssd1306_send_byte(0);
  oled.ssd1306_send_data_stop();

  if (control::consumePress(control::BTN_U)) {
    -- doty;
  }
  
  if (control::consumePress(control::BTN_D)) {
    ++ doty;
  }

  if (control::consumePress(control::BTN_L)) {
    dotx -= 6;
  }
  
  if (control::consumePress(control::BTN_R)) {
    dotx += 6;
  }

  if (dotx < 0) dotx = 0;
  if (dotx > 126) dotx = 126; // right edge should be <= 127
  if (doty < 0) doty = 0;
  if (doty > 7) doty = 7;

  oled.ssd1306_setpos(dotx, doty);
  oled.ssd1306_send_data_start();
  oled.ssd1306_send_byte(0b00011000);
  oled.ssd1306_send_byte(0b00011000);
  oled.ssd1306_send_data_stop();

  //delay(128);


}
