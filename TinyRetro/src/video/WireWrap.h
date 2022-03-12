
#include <stdint.h>

#include "TinyI2CMaster.h"

namespace wirerap {

  // USI implementation; stable on attiny, but slightly larger than bitbang
  class Tiny {
  private:
    static TinyI2CMaster i2c;

  public:
    static void init() {
      i2c.init();
    }

    static bool start(uint8_t addr, int count=0) {
      return i2c.start(addr, count);
    }

    static void stop() {
      i2c.stop();
    }

    static void write(uint8_t data) {
      i2c.write(data);
    }

  };

// experimental wrapper around core Wire lib, not ready for prime time.
class Core {
private:
  static uint8_t address;
  static uint8_t cnt;
  static uint8_t reg;
public:
  static void init();
  static bool start(uint8_t addr);
  static void stop();
  // this probably buffers, needs to occationally flush
  static void write(uint8_t data);
};


// bitbang implementation, can run in either open drain or forced modes.
class BitBang {
private:
  static void xfer_start(void);
  static void xfer_stop(void);

public:
  static void init();
  static void send_byte(uint8_t byte);
  static void write(uint8_t data);
  static bool start(uint8_t addr);
  static void stop();
};

class WireWrap : public BitBang {};


} // namespace
