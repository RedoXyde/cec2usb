#include <string.h>
#include "common.h"
#include "main.h"
#include "usb.h"
#include "usb_keyboard.h"
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

/**
 * Grundig TV remote
 *   Key              CEC code      Kodi Keyboard
 *    1               0x21          KEYPAD_1
 *    2               0x22          KEYPAD_2
 *    3               0x23          KEYPAD_3
 *    4               0x24          KEYPAD_4
 *    5               0x25          KEYPAD_5
 *    6               0x26          KEYPAD_6
 *    7               0x27          KEYPAD_7
 *    8               0x28          KEYPAD_8
 *    9               0x29          KEYPAD_9
 *    0               0x20          KEYPAD_0
 *    Repeat          0x09          KEY_W
 *    EPG             0x53          KEY_C
 *    P+              0x30          KEY_PAGE_UP
 *    P-              0x31          KEY_PAGE_DOWN
 *    Up              0x01          KEY_UP
 *    Down            0x02          KEY_DOWN
 *    Left            0x03          KEY_LEFT
 *    Right           0x05          KEY_RIGHT
 *    Select          0x00          KEY_ENTER
 *    Back            0x0D          KEY_BACKSPACE
 *    Info            0x35          KEY_I
 *    Red/Play/Pause  0x44          KEY_SPACE
 *    Green/Rewind    0x48          KEY_R
 *    Yellow/Stop     0x45          KEY_X
 *    Blue/Forward    0x49          KEY_F
 *    Txt             0x4c          KEY_TAB
 *    Rec             0x4b          KEY_PRINTSCREEN
 */

typedef struct {
  uint8_t cec;
  uint8_t kbd;
} cec_key_map_t;

#define CEC_KEYS_KBD_MAPPINGS_NB  27
const cec_key_map_t _cec_keys_mappings[CEC_KEYS_KBD_MAPPINGS_NB] = 
{
  { .cec = 0x21, .kbd = KEYPAD_1        },
  { .cec = 0x22, .kbd = KEYPAD_2        },
  { .cec = 0x23, .kbd = KEYPAD_3        },
  { .cec = 0x24, .kbd = KEYPAD_4        },
  { .cec = 0x25, .kbd = KEYPAD_5        },
  { .cec = 0x26, .kbd = KEYPAD_6        },
  { .cec = 0x27, .kbd = KEYPAD_7        },
  { .cec = 0x28, .kbd = KEYPAD_8        },
  { .cec = 0x29, .kbd = KEYPAD_9        },
  { .cec = 0x20, .kbd = KEYPAD_0        },
  { .cec = 0x09, .kbd = KEY_W           },
  { .cec = 0x53, .kbd = KEY_C           },
  { .cec = 0x30, .kbd = KEY_PAGE_UP     },
  { .cec = 0x31, .kbd = KEY_PAGE_DOWN   },
  { .cec = 0x01, .kbd = KEY_UP          },
  { .cec = 0x02, .kbd = KEY_DOWN        },
  { .cec = 0x03, .kbd = KEY_LEFT        },
  { .cec = 0x04, .kbd = KEY_RIGHT       },
  { .cec = 0x00, .kbd = KEY_ENTER       },
  { .cec = 0x0D, .kbd = KEY_BACKSPACE   },
  { .cec = 0x35, .kbd = KEY_I           },
  { .cec = 0x44, .kbd = KEY_SPACE       },
  { .cec = 0x48, .kbd = KEY_R           },
  { .cec = 0x45, .kbd = KEY_X           },
  { .cec = 0x49, .kbd = KEY_F           },
  { .cec = 0x4c, .kbd = KEY_TAB         },
  { .cec = 0x4b, .kbd = KEY_PRINTSCREEN },
};

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
  dbg_s("< KeyDown 0x"); dbg_n(k); 
  uint8_t i;
  for(i=0;i<CEC_KEYS_KBD_MAPPINGS_NB;i++)
  {
    const cec_key_map_t* m = &_cec_keys_mappings[i];
    if(k == m->cec)
    {
      usb_keyboard_press(0,m->kbd,0);
      dbg_s(" Mapped \n");
      return;
    }
  }
  dbg_c('\n');
}

void cecKeyUp(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("< KeyUp\n");
  usb_keyboard_release();
}


// Define the port at which the signal will be sent. The port needs to
// be known at compilation time, the pin (0-7) can be chosen at run time.
#define WS2811_DDR  DDRB
#define WS2811_PORT PORTB
#define WS2811_IO   PB1

// send RGB in R,G,B order instead of the standard WS2811 G,R,B order.
// Most ws2811 LED strips take their colors in GRB order, while some LED strings
// take them in RGB. Default is GRB, define this symbol for RGB.
//#define STRAIGHT_RGB

#include "ws2811/ws2811.h"
#include <avr/interrupt.h>

#define WS2811_NB_LEDS 99

uint8_t _leds[WS2811_NB_LEDS*3];

int main(void)
{
  DDRD |= _BV(7);
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
  
  WS2811_DDR |= _BV(WS2811_IO);

  uint16_t _leds_usb_idx=0;
  while(1)
  {
    CEC_processQueue();

    uint8_t usb[32],i;
    uint8_t n = usb_rawhid_recv(usb,10);
    if(n && _leds_usb_idx < WS2811_NB_LEDS*3)
    {
      /*dbg_n(n); dbg_c('/');
      for(i=0;i<n;i++)
        dbg_n(usb[i]);
      dbg_c('\n');
      */

      i = _leds_usb_idx == 0 ? 1 : 0;
      for(;i<n;i++)
      {
        _leds[_leds_usb_idx] = usb[i];
        if(++_leds_usb_idx >= WS2811_NB_LEDS*3)
        {
          _leds_usb_idx = 0;
          if(!CEC_isIdle())
            break;
          cli();
          ws2811_send(&_leds,WS2811_NB_LEDS,WS2811_IO);
          sei();
          break;
        }
      }      
    }
  }
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
