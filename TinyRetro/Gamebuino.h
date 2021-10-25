
/*
 * Compatability class to ease porting games that were previously ported to Gamebuino.
 */

#include <stdint.h>
#include "Control.h"

struct Gamebuino {
  void begin() {}
  bool update() {
    return true;
  }
  void titleScreen(uint8_t * bytes) {
  }

  struct {
    bool pressed(uint8_t btn) {
      return control::isPressed(btn);
    }
  } buttons;

  struct {
    void update() {}
  } display;


  struct {
    void playNote(int x, int y, int z) {}
  } sound;
};
