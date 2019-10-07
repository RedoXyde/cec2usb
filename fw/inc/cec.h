#ifndef _CEC_H_
#define _CEC_H_

#include <stdint.h>

#define CEC_IN  PD2 // INT2n, AIN1, RXD1
#define CEC_OUT PD3 // INT3n, TXD1
#define HDP     PD4 // INT5

#define CEC_ADDR_BROADCAST 0xF

#define CEC_DEFAULT       0x00
#define CEC_LISTEN_ONLY   0x01
#define CEC_PROMISCUIOUS  0x02

void CEC_Init(uint8_t addr, uint8_t m);
void CEC_tx(uint8_t* data, uint8_t len);
uint8_t CEC_rx(uint8_t* data);

#endif
