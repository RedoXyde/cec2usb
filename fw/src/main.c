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
#define DEV_TX_ADDR 0x0B
#define DEV_RX_ADDR 0x0C
#define DEV_FB_ADDR 0x0C

#include <string.h> // memcpy

void cecCb(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("cecCb 0x"); dbg_n(data[1]); dbg_c(st==0x40 ? 'n' :'a'); dbg_c('\n'); 
  uint8_t tx_data[16];
  memcpy(tx_data,data,len);
  tx_data[0] = tx_data[0]>>4; // Send back packet
  tx_data[1] = tx_data[1]+1;  // Mess with the data
  CEC_tx(tx_data,len,CEC_TX_MAX_TRIES);
}

void cecDef(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("[0x");   dbg_n(data[0]>>4); 
  dbg_s(" > 0x"); dbg_n(data[0]&0xF);
  dbg_c(' ');     dbg_c(st == 0x40 ? 'n' : 'a');
  if(len > 1)
  {
    dbg_s(": 0x");  dbg_n(data[1]);
    if(len > 2)
    {
      dbg_s(", ");
      uint8_t i;
      for(i=2;i<len;i++)
      {
        dbg_s("0x"); dbg_n(data[i]);
        if(i+1<len)
          dbg_c(' ');
      }
    }
  }
  dbg_s("]\n");
}

int main(void) 
{
  /* Run at 8MHz */
  CPU_PRESCALE(1);
  /* Init USB **********************************************************/
  usb_init();																		  /* Init USB */
  //while (!usb_configured());											  /* Wait until connected */
  _delay_ms(1500);													/* Wait a littl' bit more */

  int8_t addr=DEV_FB_ADDR;
  CEC_Init();
  #if DEV_SPY
    CEC_setMode(CEC_LISTEN_ONLY|CEC_PROMISCUIOUS|CEC_ALLOW_ALL_OPCODES);
    CEC_setDefaultHandler(&cecDef);
  #else
    CEC_setMode(CEC_DEFAULT); // CEC_ALLOW_ALL_OPCODES);
    dbg_s("Try TX Addr: ");
    if((addr = CEC_registerLogicalAddr(DEV_TX_ADDR,0)) >= 0)
      dbg_s("Ok\n");
    else
    {
      dbg_s("Failed\nTry RX Addr: ");
      if((addr = CEC_registerLogicalAddr(DEV_RX_ADDR,0)) >= 0)
        dbg_s("Ok\n");
      else
      {
        dbg_s("Failed\nForce fallback address\n");
        addr = CEC_registerLogicalAddr(DEV_FB_ADDR,1);
      } 
    }
      
    uint8_t i;
    for(i=0;i<10;i++)
      CEC_registerOpcode(i,&cecCb);
  #endif

  _delay_ms(100);

  uint8_t cec_data[16];
  uint8_t cnt=0x00;

  #if !DEV_SPY
  cec_data[0] = addr == DEV_TX_ADDR ? DEV_RX_ADDR : DEV_TX_ADDR;
  for(cnt=1;cnt<16;cnt++)
    cec_data[cnt] = cnt;
  CEC_tx(cec_data,cnt,CEC_TX_MAX_TRIES);
  _delay_ms(100);
  #endif

  uint8_t op = 0x00;
  while(1) 
  {
    CEC_processQueue();
    //TLed;           // Baaaaad for CEC Timings
    //dbg_c('.');
    #if !DEV_SPY
    if(addr == DEV_TX_ADDR)
    {
      
      _delay_ms(1000);
      TLed;
      cec_data[0] = DEV_RX_ADDR;
      cec_data[1] = op;
      if(++op>16) op = 0x00;
      for(cnt=2;cnt<16;cnt++)
        cec_data[cnt] = cnt;
      CEC_tx(cec_data,cnt,CEC_TX_MAX_TRIES);
    }
    #endif
  }
}
