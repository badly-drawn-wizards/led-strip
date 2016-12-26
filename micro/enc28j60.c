#include <enc28j60.h>

void enc_bfs_helper(enc_register reg, uint8_t val) {
  uint8_t tx[2];
  tx[0] = ENC_OP_BFS(reg.addr);
  tx[1] = val;
  send_spi(tx, 2, 0, 0);
}

void enc_bfc_helper(enc_register reg, uint8_t val) {
  uint8_t tx[2];
  tx[0] = ENC_OP_BFC(reg.addr);
  tx[1] = val;
  send_spi(tx, 2, 0, 0);
}

void enc_bank_sel(enc_register reg) {
  static uint8_t curr_bank = 0;
  if(reg.banked && curr_bank != reg.bank) {
    enc_bfs_helper(ENC_ECON1, reg.bank & 0b11);
    enc_bfc_helper(ENC_ECON1, ~reg.bank & 0b11);
    curr_bank = reg.bank;
  }
}

uint8_t enc_rcr(enc_register reg) {
  enc_bank_sel(reg);
  uint8_t tx_init = ENC_OP_RCR(reg.addr);
  uint8_t rx[3];
  uint8_t length = 2 + reg.is_m;
  send_spi(&tx_init, 1, rx, length);
  return rx[length - 1];
}

void enc_wcr(enc_register reg, uint8_t val) {
  enc_bank_sel(reg);
  uint8_t tx[2];
  tx[0] = ENC_OP_WCR(reg.addr);
  tx[1] = val;
  send_spi(tx, 2, 0, 0);
}

void enc_bfs(enc_register reg, uint8_t val) {
  enc_bank_sel(reg);
  if(reg.is_m) {
    enc_wcr(reg, enc_rcr(reg) | val);
  } else {
    enc_bfs_helper(reg, val);
  }
}

void enc_bfc(enc_register reg, uint8_t val) {
  enc_bank_sel(reg);
  if(reg.is_m) {
    enc_wcr(reg, enc_rcr(reg) & ~val);
  } else {
    enc_bfc_helper(reg, val);
  }
}

void enc_rbm(uint8_t *rx, uint8_t len) {
  uint8_t tx_init = ENC_OP_RBM;
  set_cs(0);
  send_spi_raw(&tx_init, 1, 0, 0);
  send_spi_raw(0, 0, rx, len);
  set_cs(1);
}

void enc_wbm(uint8_t *tx, uint8_t len) {
  set_cs(0);
  uint8_t tx_init = ENC_OP_WBM;
  send_spi_raw(&tx_init, 1, 0, 0);
  send_spi_raw(tx, len, 0, 0);
  set_cs(1);
}

void enc_src() {
  uint8_t tx_init = ENC_OP_SRC;
  send_spi(&tx_init, 1, 0, 0);
  // Errata B7.1
  __delay_ms(1);
}

void enc_wphy(uint8_t phreg, uint16_t val) {
  enc_wcr(ENC_MIREGADR, phreg);
  uint8_t *val_bytes = (uint8_t *)&val;
  enc_wcr(ENC_MIWRL, val_bytes[0]);
  enc_wcr(ENC_MIWRH, val_bytes[1]);
  __delay_us(11);
  while(enc_rcr(ENC_MISTAT) & ENC_MISTAT_BUSY);
}

uint16_t enc_rphy(uint8_t phreg) {
  enc_wcr(ENC_MIREGADR, phreg);
  enc_bfs(ENC_MICMD, ENC_MICMD_MIIRD);
  __delay_us(11);
  while(enc_rcr(ENC_MISTAT) & ENC_MISTAT_BUSY);
  uint16_t res;
  uint8_t *res_bytes = (uint8_t *)&res;
  res_bytes[0] = enc_rcr(ENC_MIRDL);
  res_bytes[1] = enc_rcr(ENC_MIRDH);
  return res;
}
