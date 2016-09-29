#include <xc.h>
#include <spi.h>

#ifndef _ENC28J60_H_
#define _ENC28J69_H_

#define ENC_OP(OPCODE, ARG) ((OPCODE<<5) | (0b00011111 & ARG))
#define ENC_OP_RCR(ADDR) ENC_OP(0b000, ADDR)
#define ENC_OP_WCR(ADDR) ENC_OP(0b010, ADDR)
#define ENC_OP_BFS(ADDR) ENC_OP(0b100, ADDR)
#define ENC_OP_BFC(ADDR) ENC_OP(0b101, ADDR)
#define ENC_OP_WBM 0b01111010
#define ENC_OP_RBM 0b00111010
#define ENC_OP_SRC 0b11111111

typedef struct {
unsigned addr : 5;
unsigned banked : 1;
unsigned bank : 2;
unsigned is_m : 1;
} enc_register;

#define BANKED_REG(NAME, ADDR, BANK, SENDS_DUMMY) const enc_register ENC_ ## NAME = {ADDR, 1, BANK, SENDS_DUMMY};
#define UNBANKED_REG(NAME, ADDR) const enc_register ENC_ ## NAME = {ADDR, 0, 0, 0};

#define BANK0 0
#define BANK1 1
#define BANK2 2
#define BANK3 3

BANKED_REG(ERDPTL, 0x00, BANK0, 0)
BANKED_REG(EHT0, 0x00, BANK1, 0)
BANKED_REG(MACON1, 0x00, BANK2, 1)
BANKED_REG(MAADR5, 0x00, BANK3, 1)

BANKED_REG(ERDPTH, 0x01, BANK0, 0)
BANKED_REG(EHT1, 0x01, BANK1, 0)
BANKED_REG(MAADR6, 0x01, BANK3, 1)

BANKED_REG(EWRPTL, 0x02, BANK0, 0)
BANKED_REG(EHT2, 0x02, BANK1, 0)
BANKED_REG(MACON3, 0x02, BANK2, 1)
BANKED_REG(MAADR3, 0x02, BANK3, 1)

BANKED_REG(EWRPTH, 0x03, BANK0, 0)
BANKED_REG(EHT3, 0x03, BANK1, 0)
BANKED_REG(MACON4, 0x03, BANK2, 1)
BANKED_REG(MAADR4, 0x03, BANK3, 1)

BANKED_REG(ETXSTL, 0x04, BANK0, 0)
BANKED_REG(EHT4, 0x04, BANK1, 0)
BANKED_REG(MABBIPG, 0x04, BANK2, 1)
BANKED_REG(MAADR1, 0x04, BANK3, 1)

BANKED_REG(ETXSTH, 0x05, BANK0, 0)
BANKED_REG(EHT5, 0x05, BANK1, 0)
BANKED_REG(MAADR2, 0x05, BANK3, 1)

BANKED_REG(ETXNDL, 0x06, BANK0, 0)
BANKED_REG(EHT6, 0x06, BANK1, 0)
BANKED_REG(MAIPGL, 0x06, BANK2, 1)
BANKED_REG(EBSTSD, 0x06, BANK3, 0)

BANKED_REG(ETXNDH, 0x07, BANK0, 0)
BANKED_REG(EHT7, 0x07, BANK1, 0)
BANKED_REG(MAIPGH, 0x07, BANK2, 1)
BANKED_REG(EBSTCON, 0x07, BANK3, 0)

BANKED_REG(ERXSTL, 0x08, BANK0, 0)
BANKED_REG(EPMM0, 0x08, BANK1, 0)
BANKED_REG(MACLCON1, 0x08, BANK2, 1)
BANKED_REG(EBSTCSL, 0x08, BANK3, 0)

BANKED_REG(ERXSTH, 0x09, BANK0, 0)
BANKED_REG(EPMM1, 0x09, BANK1, 0)
BANKED_REG(MACLCON2, 0x09, BANK2, 1)
BANKED_REG(EBSTCSH, 0x09, BANK3, 0)

BANKED_REG(ERXNDL, 0x0A, BANK0, 0)
BANKED_REG(EPMM2, 0x0A, BANK1, 0)
BANKED_REG(MAMXFLL, 0x0A, BANK2, 1)
BANKED_REG(MISTAT, 0x0A, BANK3, 1)

BANKED_REG(ERXNDH, 0x0B, BANK0, 0)
BANKED_REG(EPMM3, 0x0B, BANK1, 0)
BANKED_REG(MAMXFLH, 0x0B, BANK2, 1)

BANKED_REG(ERXRDPTL, 0x0C, BANK0, 0)
BANKED_REG(epmm4, 0x0C, BANK1, 0)

BANKED_REG(ERXRDPTH, 0x0D, BANK0, 0)
BANKED_REG(EPMM5, 0x0D, BANK1, 0)

BANKED_REG(ERXWRPTL, 0x0E, BANK0, 0)
BANKED_REG(EPMM6, 0x0E, BANK1, 0)

BANKED_REG(ERXWRPTH, 0x0F, BANK0, 0)
BANKED_REG(EPMM7, 0x0F, BANK1, 0)

BANKED_REG(EDMASTL, 0x10, BANK0, 0)
BANKED_REG(EPMCSL, 0x10, BANK1, 0)

BANKED_REG(EDMASTH, 0x11, BANK0, 0)
BANKED_REG(EPMCSH, 0x11, BANK1, 0)

BANKED_REG(EDMANDL, 0x12, BANK0, 0)
BANKED_REG(MICMD, 0x12, BANK2, 1)
BANKED_REG(EREVID, 0x12, BANK3, 0)

BANKED_REG(EDMANDH, 0x13, BANK0, 0)

BANKED_REG(EDMADSTL, 0x14, BANK0, 0)
BANKED_REG(EPMOL, 0x14, BANK1, 0)
BANKED_REG(MIREGADR, 0x14, BANK2, 1)

BANKED_REG(EDMADSTH, 0x15, BANK0, 0)
BANKED_REG(EPMOH, 0x15, BANK1, 0)
BANKED_REG(ECOCON, 0x15, BANK3, 0)

BANKED_REG(EDMACSL, 0x16, BANK0, 0)
BANKED_REG(MIWRL, 0x16, BANK2, 1)

BANKED_REG(EDMACSH, 0x17, BANK0, 0)
BANKED_REG(MIWRH, 0x17, BANK2, 1)
BANKED_REG(EFLOCON, 0x17, BANK3, 0)

BANKED_REG(ERXFCON, 0x18, BANK1, 0)
BANKED_REG(MIRDL, 0x18, BANK2, 1)
BANKED_REG(EPAUSL, 0x18, BANK3, 0)

BANKED_REG(EPKTCNT, 0x19, BANK1, 0)
BANKED_REG(MIRDH, 0x19, BANK2, 1)
BANKED_REG(EPAUSH, 0x19, BANK3, 0)

UNBANKED_REG(EIE, 0x1B)
UNBANKED_REG(EIR, 0x1C)
UNBANKED_REG(ESTAT, 0x1D)
UNBANKED_REG(ECON2, 0x1E)
UNBANKED_REG(ECON1, 0x1F)

#define ENC_ECON1_RXEN (1 << 2)

#define ENC_EIE_INTIE (1 << 7)
#define ENC_EIE_PKIE (1 << 6)

#define ENC_ESTAT_INT (1 << 7)
#define ENC_ESTAT_CLKRDY (1 << 0)

#define ENC_MACON1_MARXEN (1 << 0)

#define ENC_MACON3_FULDPX (1 << 0)
#define ENC_MACON3_FRMLNEN (1 << 1)

#define ENC_ERXFCON_UCEN (1 << 7)

void enc_bank_sel(enc_register reg);
void enc_bfs(enc_register reg, uint8_t val);
void enc_bfc(enc_register reg, uint8_t val);
uint8_t enc_rcr(enc_register reg);
void enc_wcr(enc_register reg, uint8_t val);
void enc_rbm(uint8_t *rx, uint8_t len);
void enc_wbm(uint8_t *tx, uint8_t len);
void enc_src();

#endif