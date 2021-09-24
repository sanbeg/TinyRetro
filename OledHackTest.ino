
#include "ssd1306xled.h"

#include "Control.h"
#include "Demo.h"

const SSD1306Device oled;

void clear() {
  oled.ssd1306_setpos(0, 0);

  for (int y = 0; y < 8; ++y) {
    //oled.ssd1306_setpos(0, y);
    for (int x = 0; x < 128; ++x) {
      oled.ssd1306_send_byte(0);
    }
  }
}


Demo demo(oled);

void setup() {
  // put your setup code here, to run once:

  control::setup();
  oled.ssd1306_init();
  //oled.ssd1306_fillscreen(0x0);

  clear();

  demo.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  demo.loop();
}
