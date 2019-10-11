#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "hid.h"

int main(int argc, char **argv) {
    char hBuff[12];
    int n,r;
    if (rawhid_open(1, 0x1337, 0x0402, 0xFF42, 0x0200)<= 0) {
    pdbg("CEC2Usb device not found...Aborting\n");
  } else {
    r = 0x42;
    int i;
    while(r == 0x42) 
    {
      n = rawhid_recv(0, hBuff, hBuffLen, usbTimeout);
      if (n <= 0 || n >= hBuffLen) {
        pdbg("\nError reading, device went offline\n");
        r = EXIT_SUCCESS;
      } 
      else if(n>0) 
      {
        for(i=0;i<5;i++) 
        {
          pdbg("0x%02X ",hBuff[i]);
        }
        pdbg("\n");
        
      }
    }
    rawhid_close(0);
  }
    return r;
}
