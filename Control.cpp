#include "Control.h"

#include <Arduino.h>

namespace control {

enum ButtonPins {
  BTN_L = 0,
  BTN_U = 1,
  BTN_R = 2,
  BTN_A = 3,
  BTN_B = 5,
  BTN_D = 7,
};

static volatile uint8_t pins = 0xff;

ISR(PCINT0_vect) {
  control::pins = PINA;
}


void setup() {
  pinMode(BTN_L, INPUT_PULLUP);
  pinMode(BTN_U, INPUT_PULLUP);
  pinMode(BTN_R, INPUT_PULLUP);
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);

  GIMSK |= 1 << PCIE0;
  /* we could compute this mask from all of the buttons, but we're listening on all of
     port A, except the i2c pins.  The enum really just allows us to swap button pins
     amongst each other.
  */
  PCMSK0 |= 0b10101111;
}

const char * debug(char * btn_text) {
  if (pins & (1 << BTN_L)) btn_text[0] = ' '; else btn_text[0] = 'L';
  if (pins & (1 << BTN_U)) btn_text[1] = ' '; else btn_text[1] = 'U';
  if (pins & (1 << BTN_R)) btn_text[2] = ' '; else btn_text[2] = 'R';
  if (pins & (1 << BTN_A)) btn_text[3] = ' '; else btn_text[3] = 'A';
  if (pins & (1 << BTN_B)) btn_text[4] = ' '; else btn_text[4] = 'B';
  if (pins & (1 << BTN_D)) btn_text[5] = ' '; else btn_text[5] = 'D';
  btn_text[6] = 0;

  return btn_text;
}
}
