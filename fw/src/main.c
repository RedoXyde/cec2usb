#include "common.h"
#include "main.h"
#include "usb.h"

#include "cec.h"

/** Atmega32u2:
 *  - 32kB flash
 *  - 1kB EEPROM
 *  - 1kB RAM
 */

void cecDataPrint(uint8_t* data, uint8_t len)
{
  dbg_s("Rx [0x");   dbg_n(data[0]>>4); 
  dbg_s(" > 0x"); dbg_n(data[0]&0xF);
  if(len > 1)
  {
    dbg_s(": 0x");  dbg_n(data[1]);
    if(len > 2)
    {
      dbg_s(", ");
      int i;
      for(i=2;i<len;i++)
      {
        dbg_s("0x"); dbg_n(data[i]); dbg_c(' ');
      }
    }
  }
  dbg_c(']');
}

#define DEV_SPY 1
#define DEV_TX 0
#define DEV_REPLY 1
#define DEV_TX_ADDR 0x0B
#define DEV_RX_ADDR 0x0C
int main(void) 
{
  /* Run at 8MHz */
  CPU_PRESCALE(1);
  /* Init USB **********************************************************/
  usb_init();																		  /* Init USB */
  //while (!usb_configured());											  /* Wait until connected */
  //_delay_ms(2500);													/* Wait a littl' bit more */

  #if DEV_SPY
    CEC_Init(0xF, CEC_LISTEN_ONLY|CEC_PROMISCUIOUS);
  #elif DEV_TX
    CEC_Init(DEV_TX_ADDR, CEC_DEFAULT|CEC_PROMISCUIOUS);
    #define DEV_ADDR DEV_TX_ADDR
  #else
    CEC_Init(DEV_RX_ADDR, CEC_DEFAULT|CEC_PROMISCUIOUS);
    #define DEV_ADDR DEV_RX_ADDR
  #endif

  uint8_t cec_data[16];
  uint8_t cnt=0x00;

  #if !DEV_SPY
  cec_data[0] = DEV_TX ? DEV_RX_ADDR : DEV_TX_ADDR;
  for(cnt=1;cnt<16;cnt++)
    cec_data[cnt] = cnt;
  CEC_tx(cec_data,cnt);
  _delay_ms(100);
  #endif

  //_delay_ms(10000);
  while(1) 
  {
    uint8_t m=0; 
    while((cnt = CEC_rx(cec_data)) != 0)
    {
      m++;
      cecDataPrint(cec_data,cnt);
      dbg_c(' ');
      #if !DEV_SPY && DEV_REPLY
        if((cec_data[0] & 0xF) == DEV_ADDR)
        {
          cec_data[0] = cec_data[0]>>4;
          cec_data[1] = cec_data[1]+1;
          CEC_tx(cec_data,cnt);
        }
      #endif
    }
    if(m)
      dbg_c('\n');
    //TLed;
    //_delay_ms(250);
  }
}
