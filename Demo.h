#include <stdint.h>
#include "ssd1306xled.h"


class Demo {
  private:
    const SSD1306Device oled;
    int dotx = 96;
    int doty = 4;

  public:
    void setup();
    void loop();
    Demo(SSD1306Device oled) : oled(oled){}
};
