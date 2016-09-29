#include <xc.h>
#include <timing.h>

#ifndef _WS2812_H_
#define _WS2812_H_

#define ZERO(PIN) PIN = 1; PIN = 0; _delay(1)
#define ONE(PIN) PIN = 1; _delay(2); PIN = 0;

#define SEND_BIT(PIN,BYTE,MASK) if(BYTE & MASK) { ONE(PIN); } else { ZERO(PIN); }
#define SEND_BYTE(PIN,BYTE) \
  SEND_BIT(PIN,BYTE,0b10000000) \
  SEND_BIT(PIN,BYTE,0b01000000) \
  SEND_BIT(PIN,BYTE,0b00100000) \
  SEND_BIT(PIN,BYTE,0b00010000) \
  SEND_BIT(PIN,BYTE,0b00001000) \
  SEND_BIT(PIN,BYTE,0b00000100) \
  SEND_BIT(PIN,BYTE,0b00000010) \
  SEND_BIT(PIN,BYTE,0b00000001)

#endif

inline void latch_color();
