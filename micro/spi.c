#include <spi.h>
#include <timing.h>

void init_spi() {
  // Put SDO on RC2
  APFCON0bits.SDOSEL = 0;

  // Configure pins

  //nCS
  TRISCbits.TRISC3 = 0;
  ANSELCbits.ANSC3 = 0;
  PORTCbits.RC3 = 1;
  //SCK
  TRISCbits.TRISC0 = 0;
  ANSELCbits.ANSC0 = 0;
  PORTCbits.RC0 = 0;
  //SDI
  TRISCbits.TRISC1 = 1;
  ANSELCbits.ANSC1 = 0;
  //SDO
  TRISCbits.TRISC2 = 0;
  ANSELCbits.ANSC2= 0;
  PORTCbits.RC2 = 0;


  // SPI, clock=F_osc/64
  SSP1CON1bits.SSPM = 0b0010;

  // Mode 0,0
  SSP1CON1bits.CKP=0;
  SSP1STATbits.CKE=1;

  // Input sampled at middle
  SSP1STATbits.SMP=0;

  // Why not
  SSP1CON1bits.WCOL=0;
  SSP1CON1bits.SSPOV=0;

  // Enable
  SSP1CON1bits.SSPEN = 1;
};

inline void set_cs(uint8_t inactive) {
  LATCbits.LATC3 = inactive;
}

void send_spi(uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_len) {
  set_cs(0);
  send_spi_raw(tx, tx_len, rx, rx_len);
  set_cs(1);
}

void send_spi_raw(uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_len) {
  uint8_t len = rx_len > tx_len ? rx_len : tx_len;
  for(uint8_t i=0; i<len; ++i) {
    SSP1BUF = i < tx_len ? tx[i] : 0;
    while(!SSP1STATbits.BF);
    uint8_t tmp = SSP1BUF;
    if(i < rx_len) rx[i] = tmp;
  }
}
