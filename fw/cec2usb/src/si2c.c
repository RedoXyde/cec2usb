#include <util/delay.h>
#include <avr/interrupt.h>  // sei/cli

#include "common.h"
#include "si2c.h"

#define SI2C_uDelay 4       // us delay for SI2C Timings

#define SI2C_PullDown(n)  SI2C_Dir |= _BV(n)
#define SI2C_Release(n)   SI2C_Dir &= ~_BV(n)
#define SI2C_isHigh(n)    ((SI2C_Pin & _BV(n))!=0)

#define SI2CDelay _delay_us(SI2C_uDelay)

void SI2C_wBit(uint8_t b) 
{
  if(b)
    SI2C_Release(SI2C_SDA);
  else
    SI2C_PullDown(SI2C_SDA);
  SI2CDelay;
  SI2C_Release(SI2C_SCL);         // SCL High, data goes live
  while(!SI2C_isHigh(SI2C_SCL));  // Stretching
  SI2CDelay;
  SI2C_PullDown(SI2C_SCL);        // SCL Low, end of bit
  SI2C_Release(SI2C_SDA);
}

uint8_t SI2C_rBit(void) 
{
  uint8_t r;
  SI2C_Release(SI2C_SDA);         // Release SDA, so slave can set it
  SI2CDelay;
  SI2C_Release(SI2C_SCL);         // Realease SCL, data can be sampled
  while(!SI2C_isHigh(SI2C_SCL));  // Stretching
  r = SI2C_isHigh(SI2C_SDA);
  SI2CDelay;
  SI2C_PullDown(SI2C_SCL);        // SCL Low, end of bit
  return r;
}

void SI2C_Init(void) 
{
  // Input, no pullups
  SI2C_Release(SI2C_SDA); 
  SI2C_Release(SI2C_SCL);
  SI2C_Port &= ~(_BV(SI2C_SDA) | _BV(SI2C_SCL));
}

void SI2C_End(void) 
{
  // Output, low
  SI2C_PullDown(SI2C_SDA); 
  SI2C_PullDown(SI2C_SCL);
}

void SI2C_Start(void) 
{
  SI2C_PullDown(SI2C_SDA);  // SDA Low
  SI2CDelay;
  SI2C_PullDown(SI2C_SCL);  // Then, SCL Low
}

void SI2C_Restart(void) 
{
  SI2C_Release(SI2C_SDA);
  SI2CDelay;
  // Stretching
  SI2C_Release(SI2C_SCL);
  while(!SI2C_isHigh(SI2C_SCL));
  SI2CDelay;
  SI2C_Start();
}

void SI2C_Stop(void) 
{
  _delay_ms(1);
  SI2C_PullDown(SI2C_SDA);
  SI2CDelay;
  // Stretching
  SI2C_Release(SI2C_SCL);
  while(!SI2C_isHigh(SI2C_SCL));
  SI2CDelay;
  SI2C_Release(SI2C_SDA);
  _delay_ms(5);
}

uint8_t SI2C_Write(uint8_t c) {
  uint8_t i;
  cli();
  for (i=0; i<8; i++) {
    SI2C_wBit((c & 0x80)!=0);
    c<<=1;
  }
  i = !SI2C_rBit();
  sei();
  //~ _delay_ms(1);
  return i;  
}

uint8_t SI2C_Read(uint8_t *c, uint8_t e) {
  uint8_t i;
  *c = 0;
  cli();
  for (i=0; i<8; i++)
      *c = (*c<<1) | SI2C_rBit();
  SI2C_wBit(!(e&0x1));              // Send Ack or Nack
  sei();
  //~ _delay_ms(1);
  return 1;  
}