#ifndef _COM_h
#define _COM_h

#define DBG 1

#if DBG
	#include "print.h"
	#define dbg_s(s) print(s)
	#define dbg_c(c) pchar(c)
	#define dbg_n(n) phex(n)
	#define dbg_sn(n) pshex(n)
	#define dbg_n16(n) phex16(n)
	#define dbg_sn16(n) pshex16(n)
#else
	#define dbg_s(s) 
	#define dbg_c(c) 
	#define dbg_n(n)
	#define dbg_sn(n) 
	#define dbg_n16(n) 
	#define dbg_sn16(n)
#endif


#define LED PD7
#define TLed PORTD ^= _BV(LED)
#define SLed PORTD |= _BV(LED)
#define CLed PORTD &= ~_BV(LED)
#endif