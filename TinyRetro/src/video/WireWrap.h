
#include "TinyI2CMaster.h"
#include <stdint.h>

namespace wirerap {

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


  class WireWrap : public Tiny {

  };


}
