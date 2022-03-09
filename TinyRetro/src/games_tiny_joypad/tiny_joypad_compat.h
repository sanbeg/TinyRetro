#include "../io/Control.h"

#define TINYJOYPAD_LEFT  isPressed(control::BTN_L)
#define TINYJOYPAD_RIGHT isPressed(control::BTN_R)
#define TINYJOYPAD_DOWN isPressed(control::BTN_D)
#define TINYJOYPAD_UP  isPressed(control::BTN_U)
#define BUTTON_DOWN isPressed(control::BTN_A)
#define BUTTON_UP !isPressed(control::BTN_A)
