#include <stdint.h>
#include "ssd1306xled.h"


class Demo {
  private:
    const SSD1306Device oled;
    int dotx = 96;
    int doty = 4;

    void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[]);

  public:
    void setup();
    void loop();
    Demo(SSD1306Device oled) : oled(oled){}
};
