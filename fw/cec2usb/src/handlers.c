#include <string.h>

#include "common.h"
#include "cec.h"
#include "cec_protocol.h"
#include "usb_keyboard.h"

typedef struct {
  uint8_t cec;
  uint8_t kbd;
} cec_key_map_t;

#define CEC_KEYS_KBD_MAPPINGS_NB  32
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
  { .cec = 0x46, .kbd = KEY_SPACE       },  // LG, Pause
  { .cec = 0x49, .kbd = KEY_F           },
  { .cec = 0x4c, .kbd = KEY_TAB         },
  { .cec = 0x4b, .kbd = KEY_PRINTSCREEN },
  { .cec = 0x71, .kbd = KEY_C           },  // LG, Blue
  { .cec = 0x72, .kbd = KEY_TAB         },  // LG, Red
  { .cec = 0x73, .kbd = KEY_I           },  // LG, Green
  { .cec = 0x74, .kbd = KEY_W           },  // LG, Yellow
};

uint16_t _cec_active_src = 0x000;

void cecGiveDeckStatus(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[3];
  d[0] = data[0]>>4;
  d[1] = CEC_OPC_DECK_STATUS;
  d[2] = 0x20; // CEC_DECK_INFO_OTHER_STATUS_LG
  CEC_tx(d,5,CEC_TX_MAX_TRIES);
  dbg_s("< cecGiveDeckStatus: 0x"); dbg_n(d[2]); dbg_c('\n');
}

#define SL_COMMAND_INIT                 0x01
#define SL_COMMAND_ACK_INIT             0x02
#define SL_COMMAND_POWER_ON             0x03
#define SL_COMMAND_CONNECT_REQUEST      0x04
#define SL_COMMAND_SET_DEVICE_MODE      0x05
#define SL_COMMAND_REQUEST_RECONNECT    0x0b
#define SL_COMMAND_REQUEST_POWER_STATUS 0xa0

#define SL_TYPE_HDDRECORDER_DISC        0x01
#define SL_TYPE_VCR                     0x02
#define SL_TYPE_DVDPLAYER               0x03
#define SL_TYPE_HDDRECORDER_DISC2       0x04
#define SL_TYPE_HDDRECORDER             0x05


void cecVendorCommand(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[5],olen=0;
  d[0] = data[0]>>4;
  d[1] = CEC_OPC_VENDOR_COMMAND;
  dbg_s("< cecVendorCommand: 0x"); dbg_n(data[2]); dbg_c(' ');
  switch(data[2])
  {
    case SL_COMMAND_INIT:
      dbg_s("Init");
      d[2] = SL_COMMAND_ACK_INIT;
      d[3] = SL_TYPE_HDDRECORDER_DISC2;
      olen = 4;
      break;
    //case 0x02:      // Ack Init
    //case 0x03:      // Power On    
    case 0x04:      // Connect request
      dbg_s("Connect request");
      d[2] = SL_COMMAND_SET_DEVICE_MODE;
      d[3] = CEC_OPC_DEVICE_TYPE_RECORDER;
      olen = 4;
      break;
    //case 0x05:      // Set Device Mode
    case 0x0B:      // Request reconnect
      dbg_s("Reconnect request");
      d[1] = CEC_OPC_REPORT_POWER_STATUS;
      d[2] = CEC_OPC_POWER_STATUS_STANDBY2ON;
      olen = 3;
      break;
    //case 0xa0:      // Request Power status
    default:
      dbg_s("Unknown vendor command");
  };
  dbg_c('\n');
  if(olen)
  {
    dbg_s("> cecVendorCommand: 0x"); dbg_n(d[2]); dbg_c('\n');
    CEC_tx(d,olen,CEC_TX_MAX_TRIES);
  }
}

void cecVendorCommandWithID(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[10],olen=0;
  d[0] = data[0]>>4;
  d[1] = CEC_OPC_VENDOR_COMMAND_WITH_ID;
  dbg_s("< cecVendorCommandWithID: len:"); dbg_n(len); dbg_c(' ');
  
  // Samsung Vendor ID
  // IDs and reply found in libcec source, thanks to them
  if(len >= 5 && data[2] == 0x00 && data[3] == 0x00 && data[4] == 0xf0)
  {
    if(len >= 6 && data[5] == 0x23)
    {
      // Keep [2:4] as is, reply with Samsung Vendor ID
      d[2] = data[2]; d[3] = data[3]; d[4] = data[4];
      d[6] = 0x24;
      d[7] = 0x00;
      d[8] = 0x80;
      olen = 9;
    }
    else
      dbg_s("Unknown Samsung command");
  }
  else
    dbg_s("Unknown vendor ID");
  dbg_c('\n');
  if(olen)
  {
    dbg_s("> cecVendorCommandWithID: 0x"); dbg_n(d[2]); dbg_c('\n');
    CEC_tx(d,olen,CEC_TX_MAX_TRIES);
  }
}

void cecGivePhysAddr(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[5];
  uint16_t addr = CEC_PhysicalAddr();
  d[0] = CEC_ADDR_BROADCAST;
  d[1] = CEC_OPC_REPORT_PHYSICAL_ADDRESS;
  d[2] = addr>>8;
  d[3] = addr&0xFF;
  d[4] = CEC_OWN_TYPE;
  dbg_s("> GivePhysAddr: physAddr: 0x"); dbg_n16(addr); 
  dbg_s(", type: 0x"); dbg_n(CEC_OWN_TYPE);  dbg_c('\n');
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
  char *name = CEC_OWN_NAME;
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

void cecDeviceVendorID(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  return; // LG TV is too chatty =/
  dbg_s("< cecDeviceVendorID from 0x"); dbg_n(data[0]>>4);
  dbg_s(": 0x"); dbg_n(data[2]); 
  dbg_s(", 0x"); dbg_n(data[3]);
  dbg_s(", 0x"); dbg_n(data[4]); 
  dbg_c('\n');
}

void cecGiveDeviceVendorID(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint8_t d[5];
  d[0] = CEC_ADDR_BROADCAST;
  d[1] = CEC_OPC_DEVICE_VENDOR_ID;
  d[2] = 0x00;
  d[3] = 0xE0;
  d[4] = 0x91;
  dbg_s("> cecGiveDeviceVendorID\n");
  CEC_tx(d,5,CEC_TX_MAX_TRIES);
}

void cecRoutingChange(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  uint16_t from,to;
  uint16_t addr = CEC_PhysicalAddr();

  from = (((uint16_t)data[2])<<8)|data[3];
  to = (((uint16_t)data[4])<<8)|data[5];

  dbg_s("< RoutingChange from 0x"); dbg_n16(from); 
  dbg_s(" to 0x"); dbg_n16(to); dbg_c('\n');
  if(to != addr)
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
  d[2] = addr>>8;
  d[3] = addr&0xFF;
  dbg_s("> ActiveSource: 0x"); dbg_n16(addr); dbg_c('\n');
  _cec_active_src = addr;
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
  if(_cec_active_src != CEC_PhysicalAddr())
    return;
  uint8_t d[4];
  uint16_t addr = CEC_PhysicalAddr();
  d[0] = CEC_ADDR_BROADCAST;
  d[1] = CEC_OPC_ACTIVE_SOURCE;
  d[2] = addr>>8;
  d[3] = addr&0xFF;
  dbg_s("> ActiveSource: 0x"); dbg_n16(addr); dbg_c('\n');
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

uint8_t keyDown = 0x00;

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
      dbg_s(" Mapped \n");
      if(keyDown == m->kbd)
        return;
      else if(keyDown)
        usb_keyboard_release();

      keyDown = m->kbd;
      usb_keyboard_press(0,m->kbd,0);
      return;
    }
  }
  dbg_c('\n');
}

void cecKeyUp(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  dbg_s("< KeyUp\n");
  keyDown = 0x00;
  usb_keyboard_release();
}

void cecSpy(const uint8_t st, const uint8_t* data, const uint8_t len)
{
  #ifndef CEC_SPY
  if(len<2) return; // Don't print PING
  #endif
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
