
#include <avr/io.h>


template <int P=PB0>
class Beeper
{
  private:
  static const uint8_t mask = 1<<P;
  public:
    bool mute = 0;         // Mute the speaker

    void setup() {
      //pinMode(10, OUTPUT);
      DDRB |= mask;
    }

    void beep(int bCount, int bDelay) {
      if (mute) return;
      for (int i = 0; i <= bCount; i++) {
        //digitalWrite(10, HIGH);
        PORTB |= mask;
        for (int i2 = 0; i2 < bDelay; i2++) {
          __asm__("nop\n\t");
        }
        //digitalWrite(10, LOW);
        PORTB &= ~mask;
        for (int i2 = 0; i2 < bDelay; i2++) {
          __asm__("nop\n\t");
        }
      }
    }

};
