#ifndef CONTROL_
#define CONTROL_


namespace control {

enum ButtonPins {
  // TODO - use PORTA macros?
  BTN_D = 7,
  BTN_L = 5,
  BTN_U = 3,
  BTN_R = 2,
  BTN_A = 1,
  BTN_B = 0,
};

void setup();
bool isPressed(int);
bool consumePress(int);
const char * debug(char *);
};

#endif
