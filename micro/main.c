#include <xc.h>
#include <stdint.h>
#include <spi.h>
#include <enc28j60.h>
#include <lg_arr.h>
#include <timing.h>
#include <ws2812.h>

#pragma config WDTE = OFF

#define LEDS 120
#define LED_PIN LATCbits.LATC4

inline uint8_t uabs(uint8_t x, uint8_t offset) {
  return x < offset ? offset-x : x-offset;
}

inline uint8_t triangle(uint16_t height, uint16_t half_period, uint16_t x) {
  return uabs(x%(2*half_period),half_period) * height / half_period;
}

LG_ARR_DECL(colors, 3*LEDS, 3)

void init_leds() {
  TRISCbits.TRISC4 = 0;
  LATCbits.LATC4 = 0;
}

void send_leds() {
  LG_ARR_LOOP(colors, colors, 1, {
      SEND_BYTE(LED_PIN, colors[i]);
  })
  latch_color();
}

uint8_t MAC_ADDR[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

void init_ethernet() {
  // Set rx buffer limits
  enc_wcr(ENC_ERXSTL, 0);
  enc_wcr(ENC_ERXSTH, 0);
  enc_wcr(ENC_ERXNDL, 0xFF);
  enc_wcr(ENC_ERXNDH, 0x1F);

  // Receive only unicast packets
  enc_wcr(ENC_ERXFCON, ENC_ERXFCON_UCEN);

  // Enable interrupts when a packet is received
  INTCONbits.INTE = 1;
  TRISAbits.TRISA2 = 1;
  enc_bfs(ENC_EIE, ENC_EIE_PKIE | ENC_EIE_INTIE);

  // Poll the oscillator start-up timer
  while(!(enc_rcr(ENC_ESTAT) & ENC_ESTAT_CLKRDY));

  // Enable the MAC reception of frames
  enc_bfs(ENC_MACON1, ENC_MACON1_MARXEN);
  // Enable frame length checking
  enc_bfs(ENC_MACON3, ENC_MACON3_FRMLNEN);
  // Half-duplex mode
  enc_bfc(ENC_MACON3, ENC_MACON3_FULDPX);

  // Set MAMXFL (1518 bytes)
  enc_wcr(ENC_MAMXFLL, 0xEE);
  enc_wcr(ENC_MAMXFLH, 0x05);

  // (Probably not required) Set non-back-to-back inter-packet gap
  enc_wcr(ENC_MAIPGL, 0x12);
  enc_wcr(ENC_MAIPGH, 0x0C);

  // Set MAADR registers
  enc_wcr(ENC_MAADR1, MAC_ADDR[0]);
  enc_wcr(ENC_MAADR2, MAC_ADDR[1]);
  enc_wcr(ENC_MAADR3, MAC_ADDR[2]);
  enc_wcr(ENC_MAADR4, MAC_ADDR[3]);
  enc_wcr(ENC_MAADR5, MAC_ADDR[4]);
  enc_wcr(ENC_MAADR6, MAC_ADDR[5]);

  // Enable reception of frames
  enc_bfs(ENC_ECON1, ENC_ECON1_RXEN);
}

#define STATUS_AND_HEADER_LENGTH (2 + 4 + 6 + 6 + 2)

void process_packet() {
  static uint16_t packet = 0;

  // Set read pointer
  enc_wcr(ENC_ERDPTL, packet);
  enc_wcr(ENC_ERDPTH, packet >> 8);

  // Read the next packet pointer, the receive status vector and the packet header
  uint8_t status_and_header[STATUS_AND_HEADER_LENGTH];
  enc_rbm(status_and_header, STATUS_AND_HEADER_LENGTH);

  // Extract the length
  uint16_t length = *((uint16_t *)(status_and_header + 16));

  // Extract the next packet pointer
  packet = *((uint16_t *)status_and_header);

  // Copy packet payload
  LG_ARR_PART(colors, colors_part, colors_part_size, {
      if(length > 0) {
        enc_rbm(colors_part, colors_part_size);
      }
      length = colors_part_size > length ? 0 : length - colors_part_size;
  });

  // Free buffer space
  enc_wcr(ENC_ERXRDPTL, packet);
  enc_wcr(ENC_ERXRDPTH, packet >> 8);
}

volatile uint8_t receiving_colors;
void receive_colors() {
  receiving_colors = 1;
  INTCONbits.GIE = 1;
  while(receiving_colors) {}
  INTCONbits.GIE = 0;
}


int main() {
  // 16 Mhz clock
  OSCCONbits.SCS = 0b10;
  OSCCONbits.IRCF = 0b1111;

  init_spi();
  init_ethernet();
  init_leds();

  while (1) {
    receive_colors();
    send_leds();
  }
}

void interrupt ISR() {
  if(INTCONbits.INTF) {
    process_packet();
    enc_bfc(ENC_ESTAT, ENC_ESTAT_INT);
    receiving_colors = 0;
    INTCONbits.INTF = 0;
  }
}
