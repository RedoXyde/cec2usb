#include "edid.h"
#include "si2c.h"

#define EDID_ADDR         0x50
#define EDID_ADDR_WRITE   (EDID_ADDR<<1)
#define EDID_ADDR_READ    (EDID_ADDR_WRITE|0x01)

uint16_t EDID_ReadPhysicalAddress(void)
{
  uint8_t b[18] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  };
  uint8_t n, i;

  SI2C_Init();
  
  //dbg_c('A');
  // Initial check, check EDID header
  n = EDID_Read(0x00,7,b);
  if(n != 7)
    return EDID_ADDR_INVALID;
  for(i=0;i<7;i++)
    if(b[i] != b[i+7])
      return EDID_ADDR_INVALID;
  //dbg_c('B');
  
  uint8_t saddr = 128;
  n = EDID_Read(saddr,4,b);
  if( n != 4 || 
      b[0] != 0x02 || // Check for CEA EDID extension
      b[2] == 4       // Check for Data Block Collection 
    )
    return EDID_ADDR_INVALID;
  //dbg_c('C');
  
  uint8_t eaddr = saddr+b[2];
  saddr += 4;
  do 
  {  
    n = EDID_Read(saddr,1,b);
    if(!n) 
      return EDID_ADDR_INVALID;
    uint8_t t = ((b[0]>>5)&0x7),  
            l =  (b[0]&0x1F);
    ++saddr;

    /*dbg_s(" D");
    
    if(t == 0b001)
      dbg_s("a");
    else if(t == 0b010)
      dbg_s("v");
    else if(t == 0b011)
      dbg_s("V");
    else if(t == 0b100)
      dbg_s("s");
    else
      dbg_s("r");
    */
    //dbg_n(b[0]);
    if(t == 0b011)
    {
      //dbg_c('E');
      n = EDID_Read(saddr,l,b);
      if(n!=l)
        return EDID_ADDR_INVALID;
      //for(i=0;i<n;i++)
      //{
      //  dbg_s(" 0x"); dbg_n(b[i]);
      //}
      // "HDMI Licensing, LLC" specific data block, with HDMI1.4 infos
      if( b[2] == 0x00 && b[1] == 0x0C && b[0] == 0x03)
      {
        uint16_t addr = ((uint16_t)(b[3])<<8)|b[4];
        return addr;
      }
    }
    saddr +=  l;
  } while(saddr < eaddr);

  return EDID_ADDR_INVALID;
}

// Read max l bytes from EEPROM, starting at address a, put them in *b, return nb read

// I2C>[0xA0 0x09 [ 0xA1 r:2]
// I2C START BIT
// WRITE: 0xA0 ACK 
// WRITE: 0x09 ACK 
// I2C START BIT
// WRITE: 0xA1 ACK 
// READ: 0x55  ACK 0x48 
// NACK
// I2C STOP BIT

uint8_t EDID_Read(const uint8_t a, const uint8_t l, uint8_t *b) 
{
	uint8_t i=0;
	if(SI2C_Start())
  {
    if(SI2C_Write(EDID_ADDR_WRITE)) // Write I2C address
    {
      if(SI2C_Write(a))               // Write EEPROM Address
      {
        SI2C_Restart();
        if(SI2C_Write(EDID_ADDR_READ)) // Write I2C address
        {
          for(i=0;i<l;i++)
            SI2C_Read(b+i,i+1 < l ? SI2C_SAck : SI2C_SNAck);
        }
      }
    }
    SI2C_Stop();
  }
  return i;
}
