#ifndef _MAIN_h
#define _MAIN_h

#include <avr/io.h>
#include <util/delay.h>

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#endif