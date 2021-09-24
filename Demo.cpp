
#include "Demo.h"
#include "Control.h"
#include "oled_text.h"

void Demo::setup() {
    using oled_text::ssd1306_char_f6x8;

  ssd1306_char_f6x8(oled, 0, 1, "Hello World.");
  ssd1306_char_f6x8(oled, 0, 2, "hacked i2c lib");
}

void Demo::loop() {
  using oled_text::ssd1306_char_f6x8;
  char btn_text[7];
  control::debug(btn_text);
  ssd1306_char_f6x8(oled, 0, 5, btn_text);

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
