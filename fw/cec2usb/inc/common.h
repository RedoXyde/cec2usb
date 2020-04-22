#pragma once
#include <stdint.h>

#define DBG 1
//#define V01

#if DBG
	#include "print.h"
	#define dbg_s(s) print(s)
	#define dbg_p(s) print_s(s)
	#define dbg_c(c) pchar(c)
	#define dbg_n(n) phex(n)
	#define dbg_sn(n) pshex(n)
	#define dbg_n16(n) phex16(n)
	#define dbg_sn16(n) pshex16(n)
#else
	#define dbg_s(s) 
	#define dbg_p(s)
	#define dbg_c(c) 
	#define dbg_n(n)
	#define dbg_sn(n) 
	#define dbg_n16(n) 
	#define dbg_sn16(n)
#endif

#ifdef V01
	#define LED PD7
	#define ILed DDRD  |= _BV(LED)
	#define TLed PORTD ^= _BV(LED)
	#define SLed PORTD |= _BV(LED)
	#define CLed PORTD &= ~_BV(LED)
#else
	#define LED PB7
	#define ILed DDRB  |= _BV(LED)
	#define TLed PORTB ^= _BV(LED)
	#define SLed PORTB |= _BV(LED)
	#define CLed PORTB &= ~_BV(LED)
#endif

#define CEC_OWN_ADDRS     { CEC_ADDR_RECORDER_1, CEC_ADDR_PLAYBACK_2, CEC_ADDR_PLAYBACK_3 }
#define CEC_OWN_TYPE      CEC_OPC_DEVICE_TYPE_RECORDER
#define CEC_OWN_PHYSADDR  0x1000
#define CEC_OWN_NAME      "ArkHome"
