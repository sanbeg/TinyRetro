
/*
    Sleep code is based on this blog post by Matthew Little:
    http://www.re-innovation.co.uk/web12/index.php/en/blog-75/306-sleep-modes-on-attiny85
*/

#include "attiny_sleep.h"
#include <avr/sleep.h>

// Routines to set and clear bits (used in the sleep code)
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void attiny_sleep(void)
{
  cbi(ADCSRA, ADEN);                   // switch analog to digital converter off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // system actually sleeps here
  sleep_disable();                     // system continues execution here when watchdog timed out
  sbi(ADCSRA, ADEN);                   // switch analog to digital converter on
};
