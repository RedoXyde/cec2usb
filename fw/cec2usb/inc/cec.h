/*
MIT License

Copyright (c) 2019 RedoX

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _CEC_H_
#define _CEC_H_

#include <stdint.h>
#include "cec_protocol.h"

#define CEC_IN  PD2 // INT2n, AIN1, RXD1
#define CEC_OUT PD3 // INT3n, TXD1
#define HDP     PD4 // INT5

#define CEC_TX_MAX_TRIES     6

#define CEC_DEFAULT           0x00
#define CEC_LISTEN_ONLY       0x01
#define CEC_PROMISCUIOUS      0x02
#define CEC_ALLOW_ALL_OPCODES 0x04

#define CEC_HWADDR_LEN 4

void CEC_Init(void);
uint8_t CEC_isIdle(void);
void CEC_setMode(const uint8_t m);
void CEC_setHWAddr(const uint8_t addr[CEC_HWADDR_LEN]);
int8_t CEC_registerLogicalAddr(const uint8_t addr, const uint8_t skipPoll);

typedef void (*cec_cb)(const uint8_t st, const uint8_t* data, const uint8_t len);

void CEC_registerOpcode(const uint8_t opcode, cec_cb hdlr);
void CEC_setDefaultHandler(cec_cb hdlr);

void CEC_tx(const uint8_t* data, const uint8_t len, const uint8_t tries);
void CEC_processQueue(void);

#endif
