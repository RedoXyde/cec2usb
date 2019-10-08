#include "common.h"
#include "main.h"
#include "usb.h"

#include "cec.h"

/** Atmega32u2:
 *  - 32kB flash
 *  - 1kB EEPROM
 *  - 1kB RAM
 */

#define DEV_SPY 0
#define DEV_REPLY 1

#define DEV_TX_ADDR 0x0B
#define DEV_RX_ADDR 0x0C
#define DEV_FB_ADDR 0x0C

#include <string.h> // memcpy

void cecCb(const uint8_t* data, const uint8_t len)
{
  dbg_s("cecCb 0x"); dbg_n(data[1]); dbg_c('\n');
  uint8_t tx_data[16];
  memcpy(tx_data,data,len);
  tx_data[0] = tx_data[0]>>4; // Send back packet
  tx_data[1] = tx_data[1]+1;  // Mess with the data
  CEC_tx(tx_data,len);
}

int main(void) 
{
  /* Run at 8MHz */
  CPU_PRESCALE(1);
  /* Init USB **********************************************************/
  usb_init();																		  /* Init USB */
  //while (!usb_configured());											  /* Wait until connected */
  _delay_ms(1500);													/* Wait a littl' bit more */

  uint8_t addr=DEV_FB_ADDR;
  CEC_Init();
  #if DEV_SPY
    CEC_setMode(CEC_LISTEN_ONLY|CEC_PROMISCUIOUS|CEC_ALLOW_ALL_OPCODES);
  #else
    CEC_setMode(CEC_DEFAULT); // CEC_ALLOW_ALL_OPCODES);
    if(CEC_registerLogicalAddr(DEV_TX_ADDR,0) == 0)
      dbg_s("TX Addr Ok");
    else
    {
      dbg_s("TX Addr Failed. Try RX Addr\n");
      if(CEC_registerLogicalAddr(DEV_RX_ADDR,0) == 0)
        dbg_s("RX Addr Ok\n");
      else
      {
        dbg_s("RX Addr Failed. Force fallback address\n");
        CEC_registerLogicalAddr(DEV_FB_ADDR,1);
      } 
    }
      
    uint8_t i;
    for(i=0;i<10;i++)
      CEC_registerOpcode(i,&cecCb);
    //CEC_setDefaultHandler(&cecDef);
  #endif

  uint8_t cec_data[16];
  uint8_t cnt=0x00;

  #if !DEV_SPY
  cec_data[0] = addr == DEV_TX_ADDR ? DEV_RX_ADDR : DEV_TX_ADDR;
  for(cnt=1;cnt<16;cnt++)
    cec_data[cnt] = cnt;
  CEC_tx(cec_data,cnt);
  _delay_ms(100);
  #endif

  while(1) 
  {
    CEC_processQueue();
    //TLed;           // Baaaaad for CEC Timings
    //dbg_c('.');
  }
}
