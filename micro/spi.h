#include <xc.h>
#include <stdint.h>

#ifndef _SPI_H_
#define _SPI_H_

void init_spi();
inline void set_cs(uint8_t inactive);
void send_spi(uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_len);
void send_spi_raw(uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_len);

#endif
