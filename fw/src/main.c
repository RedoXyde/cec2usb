#include <string.h>
#include "common.h"
#include "main.h"
#include "usb.h"

#include "cec.h"

/**
 * Atmega32u2:
 *  - 32kB flash
 *  - 1kB EEPROM
 *  - 1kB RAM
 */

/**
 * Kodi: Logical Address 0x1, Physical Address 0x1000 (1.0.0.0)
 * // Discover
 *    [0x00 > 0x01 a: 0x83]                       // rxu   Give Physical Address
 *    [0x01 > 0x0F n: 0x84, 0x10 0x00 0x01]       // txb   Report Physical Address    Physical Address (0x1000)
 *    [0x00 > 0x01 a: 0x46]                       // rxu   Give OSD Name
 *    [0x01 > 0x00 a: 0x47, 0x4B 0x6F 0x64 0x69]  // txu   Set OSD Name               'K','o','d','i'
 * // Selecting Kodi
 *    [0x00 > 0x0F n: 0x80, 0x10 0x00 0x10 0x00]  // rxb   Routing Change             Original Address (0x1000), New Address(0x1000)
 *    [0x01 > 0x00 a: 0x04]                       // txu   Image View On
 *    [0x00 > 0x01 a: 0x8F]                       // rxu   Give Device Power Status
 *    [0x01 > 0x0F n: 0x82, 0x10 0x00]            // txb   Set Active Source          Physical Address (0x1000)
 *    [0x01 > 0x00 a: 0x90, 0x00]                 // txu   Report Power Status        ON
 *    [0x00 > 0x0F n: 0x86, 0x10 0x00]            // rxb   Set Stream Path            Physical Address (0x1000)
 *    [0x01 > 0x0F n: 0x82, 0x10 0x00]            // txb   Set Active Source          Physical Address (0x1000)
 *    [0x00 > 0x01 a: 0x8F]                       // rxu   Give Device Power Status
 *    [0x01 > 0x00 a: 0x90, 0x00]                 // txu   Report Power Status        ON
 * // Keys for selecting a movie and playing
 *    [0x00 > 0x01 a: 0x44, 0x00]                 // rxu   Pressing key               Select
 *    [0x00 > 0x01 a: 0x45]                       // rxu   Releasing key
 *    [0x00 > 0x01 a: 0x44, 0x02]                 // rxu   Pressing key               Down
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x02]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x02]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x00]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x02]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x00]
 *    [0x00 > 0x01 a: 0x45]
 * // Kodi going to powersave => Powering Off the TV
 *    [0x01 > 0x00 a: 0x36]                       // Standby
 *    [0x00 > 0x0F n: 0x36]
 */

#define DEV_SPY 0
void cecSpy(const uint8_t st, const uint8_t* data, const uint8_t len);

#define CEC_ADDRS     { CEC_ADDR_PLAYBACK_1, CEC_ADDR_PLAYBACK_2, CEC_ADDR_PLAYBACK_3 }
#define CEC_TYPE      CEC_OPC_DEVICE_TYPE_PLAYBACK
#define CEC_PHYSADDR  0x2000
#define CEC_NAME      "ArkHome"

uint16_t _cec_active_src = 0x000;

void cecGivePhysAddr(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[5];
  d[0] = CEC_ADDR_BROADCAST;
  d[1] = CEC_OPC_REPORT_PHYSICAL_ADDRESS;
  d[2] = CEC_PHYSADDR>>8;
  d[3] = CEC_PHYSADDR&0xFF;
  d[4] = CEC_TYPE;
  dbg_s("> GivePhysAddr: physAddr: 0x"); dbg_n16(CEC_PHYSADDR); 
  dbg_s(", type: 0x"); dbg_n(CEC_TYPE);  dbg_c('\n');
  CEC_tx(d,5,CEC_TX_MAX_TRIES);
}

void cecReportPhysAddr(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint16_t addr = (((uint16_t)data[2])<<8)|data[3];
  uint8_t  type  = data[4];
  dbg_s("< ReportPhysAddr from 0x"); dbg_n(data[0]>>4); 
  dbg_s(": physAddr: 0x"); dbg_n16(addr);
  dbg_s(", type: 0x"); dbg_n(type);
}

void cecGiveOSDName(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[16];
  d[0] = data[0]>>4;
  d[1] = CEC_OPC_SET_OSD_NAME;
  char *name = CEC_NAME;
  uint8_t nlen = strlen(name);
  if(nlen > 14) nlen = 14;
  memcpy(d+2,name,nlen);
  dbg_s("> GiveOSDName: "); dbg_p(name); dbg_c('\n');
  CEC_tx(d,2+nlen,CEC_TX_MAX_TRIES);
}

void cecGivePowerStatus(const uint8_t st, const uint8_t* data, const uint8_t len)
{ 
  uint8_t d[3];
  d[0] = data[0]>>4;
  d[1] = CEC_OPC_REPORT_POWER_STATUS;
  d[2] = CEC_OPC_POWER_STATUS_ON;
  dbg_s("> GivePowerStatus: 0x"); dbg_n(CEC_OPC_POWER_STATUS_ON); dbg_c('\n');
  CEC_tx(d,3,CEC_TX_MAX_TRIES);
}

void cecReportPowerStatus(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("< ReportPowerStatus from 0x"); dbg_n(data[0]>>4); 
  dbg_s(": 0x"); dbg_n(data[2]); dbg_c('\n');
}

void cecRoutingChange(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint16_t from,to;
  from = (((uint16_t)data[2])<<8)|data[3];
  to = (((uint16_t)data[4])<<8)|data[5];

  dbg_s("< RoutingChange from 0x"); dbg_n16(from); 
  dbg_s(" to 0x"); dbg_n16(to); dbg_c('\n');
  if(to != CEC_PHYSADDR)
    return;

  uint8_t d[4];
  // Image View On
  d[0] = CEC_ADDR_TV;
  d[1] = CEC_OPC_IMAGE_VIEW_ON;
  CEC_tx(d,2,CEC_TX_MAX_TRIES);
  dbg_s("> ImageViewOn\n");
  // Set active source
  d[0] = CEC_ADDR_BROADCAST;
  d[1] = CEC_OPC_ACTIVE_SOURCE;
  d[2] = CEC_PHYSADDR>>8;
  d[3] = CEC_PHYSADDR&0xFF;
  dbg_s("> ActiveSource: 0x"); dbg_n16(CEC_PHYSADDR); dbg_c('\n');
  _cec_active_src = CEC_PHYSADDR;
  CEC_tx(d,4,CEC_TX_MAX_TRIES);
}

void cecSetStreamPath(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint16_t addr = (((uint16_t)data[2])<<8)|data[3];
  dbg_s("< SetStreamPath, physAddr 0x"); dbg_n16(addr); dbg_c('\n');
}

void cecRequestActiveSource(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("< RequestActiveSource\n");
  if(_cec_active_src != CEC_PHYSADDR)
    return;
  uint8_t d[4];
  d[0] = CEC_ADDR_BROADCAST;
  d[1] = CEC_OPC_ACTIVE_SOURCE;
  d[2] = CEC_PHYSADDR>>8;
  d[3] = CEC_PHYSADDR&0xFF;
  dbg_s("> ActiveSource: 0x"); dbg_n16(CEC_PHYSADDR); dbg_c('\n');
  CEC_tx(d,4,CEC_TX_MAX_TRIES);
}

void cecActiveSource(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint16_t addr = (((uint16_t)data[2])<<8)|data[3];
  _cec_active_src = addr;
  dbg_s("< ActiveSource, physAddr 0x"); dbg_n16(addr); dbg_c('\n');
}

void cecStandBy(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("< StandBy\n");
}

void cecSetMenuLanguage(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  char lang[4];
  uint8_t i=0;
  for(;i<3;i++)
    lang[i] = data[2+i];
  lang[3] = '\0';
  dbg_s("< SetMenuLanguage "); dbg_p(lang); dbg_c('\n');
}

void cecKeyDown(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t k = data[2];
  dbg_s("< KeyDown 0x"); dbg_n(k); dbg_c('\n');
}

void cecKeyUp(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("< KeyUp\n");
}

int main(void)
{

  CPU_PRESCALE(1);            // Run at 8MHz
  usb_init();
  while (!usb_configured());  // Loop until connected
  _delay_ms(1500);

  CEC_Init();
  #if DEV_SPY
    CEC_setMode(CEC_LISTEN_ONLY|CEC_PROMISCUIOUS|CEC_ALLOW_ALL_OPCODES);
    CEC_setDefaultHandler(&cecSpy);
  #else
    CEC_setMode(CEC_DEFAULT); // CEC_ALLOW_ALL_OPCODES);
    uint8_t addrs[3] = CEC_ADDRS, addr;
    uint8_t i=0;
    for(;i<3;i++)
    {
      dbg_s("Try address 0x"); dbg_n(addrs[i]); dbg_s(": ");
      if((addr = CEC_registerLogicalAddr(CEC_ADDR_PLAYBACK_1,0)) >= 0)
      {
        dbg_s("Ok\n");
        break;
      }
    }
    // FIXME Handle all addresses failed

    // Registering OPCODES
        // Unicast
    CEC_registerOpcode(CEC_OPC_GIVE_PHYSICAL_ADDRESS    , &cecGivePhysAddr);
    CEC_registerOpcode(CEC_OPC_GIVE_OSD_NAME            , &cecGiveOSDName);
    CEC_registerOpcode(CEC_OPC_GIVE_DEVICE_POWER_STATUS , &cecGivePowerStatus);
    CEC_registerOpcode(CEC_OPC_REPORT_POWER_STATUS      , &cecReportPowerStatus);
        // Broadcast
    CEC_registerOpcode(CEC_OPC_REQUEST_ACTIVE_SOURCE    , &cecRequestActiveSource);
    CEC_registerOpcode(CEC_OPC_ROUTING_CHANGE           , &cecRoutingChange);

    CEC_registerOpcode(CEC_OPC_REPORT_PHYSICAL_ADDRESS  , &cecReportPhysAddr);
    CEC_registerOpcode(CEC_OPC_ACTIVE_SOURCE            , &cecActiveSource);
    CEC_registerOpcode(CEC_OPC_SET_STREAM_PATH          , &cecSetStreamPath);

    CEC_registerOpcode(CEC_OPC_SET_MENU_LANGUAGE        , &cecSetMenuLanguage);
    CEC_registerOpcode(CEC_OPC_STANDBY                  , &cecStandBy);
        // Fallback to default handler for broadcasted messages
    CEC_setDefaultHandler(&cecSpy); 
      // Keys
    CEC_registerOpcode(CEC_OPC_USER_CONTROL_PRESSED     , &cecKeyDown);
    CEC_registerOpcode(CEC_OPC_USER_CONTROL_RELEASED    , &cecKeyUp);

    dbg_s("Ready, addr 0x"); dbg_n(addr); dbg_c('\n');
  #endif
  
  while(1)
    CEC_processQueue();
}

void cecSpy(const uint8_t st, const uint8_t* data, const uint8_t len)
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
