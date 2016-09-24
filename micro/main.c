#include <xc.h>
#include <stdint.h>

#define LEDS 20
#define LED_DATA_PIN LATCbits.LATC2

#define _XTAL_FREQ 16000000

#pragma config WDTE = OFF

uint16_t time = 0;

inline void latch_color() {
  __delay_us(7);
}

inline uint8_t uabs(uint8_t x, uint8_t offset) {
  return x < offset ? offset-x : x-offset;
}

inline uint8_t triangle(uint16_t height, uint16_t half_period, uint16_t x) {
  return uabs(x%(2*half_period),half_period) * height / half_period;
}

#define ZERO LED_DATA_PIN = 1; LED_DATA_PIN = 0; _delay(1)
#define ONE LED_DATA_PIN = 1; _delay(2); LED_DATA_PIN = 0;


uint8_t colors_a[3*LEDS / 5];
uint8_t colors_b[3*LEDS / 5];
uint8_t colors_c[3*LEDS / 5];
uint8_t colors_d[3*LEDS / 5];
uint8_t colors_e[3*LEDS / 5 + (3*LEDS % 5)];

inline void compute_color(uint8_t pos, uint8_t* colors) {
  uint16_t intensity_r = triangle(255, 60, pos+time);
  uint16_t intensity_g = triangle(255, 60, pos+time+10);
  uint16_t intensity_b = triangle(255, 60, pos+time+20);
  colors[0] = intensity_r * intensity_r >> 8;
  colors[1] = intensity_g * intensity_g >> 8;
  colors[2] = intensity_b * intensity_b >> 8;
}

#define COMPUTE_COLORS(colors) for(uint8_t i=0; i<sizeof(colors); i+=3) { compute_color(pos, colors+i); ++pos; }
void compute_colors() {
  uint8_t pos=0;
  COMPUTE_COLORS(colors_a);
  COMPUTE_COLORS(colors_b);
  COMPUTE_COLORS(colors_c);
  COMPUTE_COLORS(colors_d);
  COMPUTE_COLORS(colors_e);
}


#define SEND_BIT(byte,mask) if(byte & mask) { ONE; } else { ZERO; }
#define SEND_BYTE(byte) SEND_BIT(byte,0b10000000) SEND_BIT(byte,0b01000000) SEND_BIT(byte,0b00100000) SEND_BIT(byte,0b00010000) SEND_BIT(byte,0b00001000) SEND_BIT(byte,0b00000100) SEND_BIT(byte,0b00000010) SEND_BIT(byte,0b00000001)
#define SEND_COLOR(red, blue, green) SEND_BYTE(green); SEND_BYTE(red); SEND_BYTE(blue)
#define SEND_COLORS(colors) for(uint8_t i=0; i<sizeof(colors); i+=3) { SEND_COLOR((colors)[i+0], (colors)[i+1], (colors)[i+2]); }
void send_colors() {
  SEND_COLORS(colors_a);
  SEND_COLORS(colors_b);
  SEND_COLORS(colors_c);
  SEND_COLORS(colors_d);
  SEND_COLORS(colors_e);
}

int main() {
  OSCCONbits.SCS = 0b10;
  OSCCONbits.IRCF = 0b1111;

  TRISCbits.TRISC2 = 0;


  while (1) {
    compute_colors();
    send_colors();
    latch_color();

    ++time;
  }
}
