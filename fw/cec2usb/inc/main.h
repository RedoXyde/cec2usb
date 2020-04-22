#ifndef _MAIN_h
#define _MAIN_h

#include <avr/io.h>
#include <util/delay.h>
#include "cec.h"

// Define the port at which the signal will be sent. The port needs to
// be known at compilation time, the pin (0-7) can be chosen at run time.
#define WS2811_DDR  DDRB
#define WS2811_PORT PORTB
#ifdef V01
  #define WS2811_IO   PB1
#else
  #define WS2811_IO   PB2
#endif

// send RGB in R,G,B order instead of the standard WS2811 G,R,B order.
// Most ws2811 LED strips take their colors in GRB order, while some LED strings
// take them in RGB. Default is GRB, define this symbol for RGB.
//#define STRAIGHT_RGB

#define WS2811_NB_LEDS 99
#include "ws2811/ws2811.h"

#define DEV_SPY 0

#endif