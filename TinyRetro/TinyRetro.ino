
#include "src/video/ssd1306xled.h"

#include "Control.h"

#define play_game(g) namespace g{void loop();void setup();} namespace game{using namespace g;}

//play_game(demo);
play_game(arkanoid);
//play_game(bike);
//play_game(bomber);
//play_game(frogger);
//play_game(frogger_ata);
//play_game(gilbert);
//play_game(invaders);
//play_game(space_attack);
//play_game(ttris);

SSD1306Device oled;

void clear() {
  oled.ssd1306_setpos(0, 0);

  for (int y = 0; y < 8; ++y) {
    //oled.ssd1306_setpos(0, y);
    for (int x = 0; x < 128; ++x) {
      oled.ssd1306_send_byte(0);
    }
  }
}

void setup() {
  // put your setup code here, to run once:

  control::setup();
  oled.ssd1306_init();
  //oled.ssd1306_fillscreen(0x0);

  clear();

  game::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  game::loop();
}
