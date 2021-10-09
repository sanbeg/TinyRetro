
template <int P>
class Beeper
{
  public:
    boolean mute = 0;         // Mute the speaker

    void setup() {
      pinMode(P, OUTPUT);
    }

    void beep(int bCount, int bDelay) {
      if (mute) return;
      for (int i = 0; i <= bCount; i++) {
        digitalWrite(P, HIGH);
        for (int i2 = 0; i2 < bDelay; i2++) {
          __asm__("nop\n\t");
        }
        digitalWrite(P, LOW);
        for (int i2 = 0; i2 < bDelay; i2++) {
          __asm__("nop\n\t");
        }
      }
    }

};
