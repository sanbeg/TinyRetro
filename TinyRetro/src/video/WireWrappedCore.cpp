#include "WireWrap.h"
#include <avr/io.h>
#include <Wire.h>

/*
 * This code is still very expermimental, since most core I2C implementations
 * buffer each transaction, and silently discard any data which doesn't fit in the buffer.
 */
//#define CORE_WIRE

namespace wirerap {
#ifdef CORE_WIRE
  uint8_t Core::address;
  uint8_t Core::cnt;
  uint8_t Core::reg;

void Core::init() {
    Wire.begin();
  }

bool Core::start(uint8_t addr) {
    address = addr;
    cnt = 1;
    reg = 0;
    Wire.beginTransmission(addr);
    return true;
  }

void Core::stop() {
    //address = 0;
    //cnt = 0;
    //reg = 0;
    Wire.endTransmission();
  }

  // this probably buffers, needs to occationally flush
void Core::write(uint8_t data) {
    if (cnt == 1) {
      reg = data;
    }
    if (++cnt >= 16) {
      Wire.endTransmission();
      cnt = 2;
      Wire.beginTransmission(address);
      Wire.write(reg);
    }
    Wire.write(data);
  }


#endif
};
