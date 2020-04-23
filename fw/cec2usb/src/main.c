#include <string.h>
#include <avr/interrupt.h>

#include "common.h"
#include "main.h"
#include "usb.h"
#include "handlers.h"
#include "edid.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

/**
 * Atmega32u2:
 *  - 32kB flash
 *  - 1kB EEPROM
 *  - 1kB RAM
 */

int main(void)
{
  CPU_PRESCALE(1);            // Run at 8MHz
  ILed;
  SLed;

  usb_init();
  while (!usb_configured());  // Loop until connected
  _delay_ms(2000);
  CLed;
  dbg_s("CEC2Usb start\n");

  EDID_Init();
  CEC_Init();
  #if DEV_SPY
    CEC_setMode(CEC_LISTEN_ONLY|CEC_PROMISCUIOUS|CEC_ALLOW_ALL_OPCODES);
    CEC_setDefaultHandler(&cecSpy);
  #else
    CEC_setMode(CEC_DEFAULT); // CEC_ALLOW_ALL_OPCODES);
    //CEC_setPhysicalAddr(0x1000); // Force Physical Address to 1.0.0.0

    // Registering OPCODES
        // Unicast
    CEC_registerOpcode(CEC_OPC_GIVE_PHYSICAL_ADDRESS    , &cecGivePhysAddr);        // 0x83
    CEC_registerOpcode(CEC_OPC_GIVE_OSD_NAME            , &cecGiveOSDName);         // 0x46
    CEC_registerOpcode(CEC_OPC_GIVE_DEVICE_POWER_STATUS , &cecGivePowerStatus);     // 0x8f
    CEC_registerOpcode(CEC_OPC_REPORT_POWER_STATUS      , &cecReportPowerStatus);   // 0x90
    CEC_registerOpcode(CEC_OPC_GIVE_DEVICE_VENDOR_ID    , &cecGiveDeviceVendorID);  // 0x8c
    CEC_registerOpcode(CEC_OPC_VENDOR_COMMAND           , &cecVendorCommand);       // 0x89
    CEC_registerOpcode(CEC_OPC_GIVE_DECK_STATUS         , &cecGiveDeckStatus);      // 0x1a

    CEC_registerOpcode(CEC_OPC_VENDOR_COMMAND_WITH_ID   , &cecVendorCommandWithID); // 0xa0

        // Broadcast
    CEC_registerOpcode(CEC_OPC_REQUEST_ACTIVE_SOURCE    , &cecRequestActiveSource); // 0x85
    CEC_registerOpcode(CEC_OPC_ROUTING_CHANGE           , &cecRoutingChange);       // 0x80

    CEC_registerOpcode(CEC_OPC_REPORT_PHYSICAL_ADDRESS  , &cecReportPhysAddr);      // 0x84
    CEC_registerOpcode(CEC_OPC_ACTIVE_SOURCE            , &cecActiveSource);        // 0x82
    CEC_registerOpcode(CEC_OPC_SET_STREAM_PATH          , &cecSetStreamPath);       // 0x86

    CEC_registerOpcode(CEC_OPC_SET_MENU_LANGUAGE        , &cecSetMenuLanguage);     // 0x32
    CEC_registerOpcode(CEC_OPC_STANDBY                  , &cecStandBy);             // 0x36

    CEC_registerOpcode(CEC_OPC_DEVICE_VENDOR_ID         , &cecDeviceVendorID);      // 0x87
      // Keys
    CEC_registerOpcode(CEC_OPC_USER_CONTROL_PRESSED     , &cecKeyDown);             // 0x44
    CEC_registerOpcode(CEC_OPC_USER_CONTROL_RELEASED    , &cecKeyUp);               // 0x45
        // Fallback to default handler for broadcasted messages
    CEC_setDefaultHandler(&cecSpy); 
  #endif

  // Init Leds
  uint8_t _leds[WS2811_NB_LEDS*3];
  WS2811_DDR |= _BV(WS2811_IO);
  uint16_t _leds_usb_idx;
  for(_leds_usb_idx=0;_leds_usb_idx<WS2811_NB_LEDS*3-3;_leds_usb_idx+=3)
    _leds[_leds_usb_idx]   = 0x00;  // G
    _leds[_leds_usb_idx+1] = 0x00;  // R
    _leds[_leds_usb_idx+2] = 0x00;  // B

  cli();
  ws2811_send(&_leds,WS2811_NB_LEDS,WS2811_IO);
  sei();
  _leds_usb_idx=0;

  usb_rawhid_set_feature_report_buffer((uint8_t*)&_leds,WS2811_NB_LEDS*3);

  while(1)
  {
    #ifndef CEC_SPY
    if(CEC_Addr() == CEC_ADDR_UNREGISTERED)
    {
      //dbg_s("Try to register on CEC Bus\n");
      if(CEC_PhysicalAddr() == CEC_PHYSADDR_INVALID)
      {
        uint16_t a = EDID_ADDR_INVALID;
        while(1)
        {
          a = EDID_ReadPhysicalAddress(); 
          if(a != EDID_ADDR_INVALID)
            break;
          dbg_c('.');
          _delay_ms(1000);
        }
        CEC_setPhysicalAddr(a);
      }
      uint16_t a = CEC_PhysicalAddr();
      dbg_s("Physical Address: ");
      dbg_n((a>>12)&0xF);dbg_c('.'); dbg_n((a>>8)&0xF);dbg_c('.');
      dbg_n((a>>4)&0xF);dbg_c('.');  dbg_n(a&0xF);
        dbg_s("\n");
      uint8_t addrs[3] = CEC_OWN_ADDRS;
      int8_t addr=0;
      uint8_t i=0;
      for(;i<3;i++)
      {
        //dbg_s("  Try Logcial address 0x"); dbg_n(addrs[i]); dbg_s(": ");
        if((addr = CEC_registerLogicalAddr(addrs[i],0)) >= 0)
        {
          //dbg_s("Ok\n");
          break;
        }
      }
      // FIXME Handle all addresses failed
      dbg_s("Logical Address: 0x"); dbg_n(addr); dbg_c('\n');
    }
    #endif

    CEC_processQueue();

    if(usb_rawhid_feature_report_available() > -1 && CEC_isIdle())
    {
      //dbg_s("Got Feature report\n");
      cli();
      ws2811_send(&_leds,WS2811_NB_LEDS,WS2811_IO);
      sei();
      /*
      uint16_t i;
      for(i=0;i<WS2811_NB_LEDS*3;i++)
        dbg_n(_leds[i]);
      dbg_c('\n');
      */
      usb_rawhid_enable_feature_report();
    }
    //_delay_ms(1);
  }
}
