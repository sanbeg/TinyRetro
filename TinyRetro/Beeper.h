
template <int P=PB0>
class Beeper
{
  private:
  const uint8_t mask = 1<<P;
  public:
    boolean mute = 0;         // Mute the speaker

    void setup() {
      //pinMode(P, OUTPUT);
      DDRB |= mask;
    }

    void beep(int bCount, int bDelay) {
      if (mute) return;
      for (int i = 0; i <= bCount; i++) {
        //digitalWrite(P, HIGH);
        PORTB |= mask;
        for (int i2 = 0; i2 < bDelay; i2++) {
          __asm__("nop\n\t");
        }
        //digitalWrite(P, LOW);
        PORTB &= ~mask;
        for (int i2 = 0; i2 < bDelay; i2++) {
          __asm__("nop\n\t");
        }
      }
    }

};
