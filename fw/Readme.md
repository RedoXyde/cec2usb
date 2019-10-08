# CEC2Usb

# Programming

- Flash the Bootloader using ICSP
  avrdude -c usbtiny -p m32u2 -U flash:w:./BootloaderDFU.hex
- Flash the fuses using ICSP
  avrdude -c usbtiny -p m32u2 -U lfuse:w:0xde:m -U hfuse:w:0xd9:m -U efuse:w:0xf4:m
- Reboot the board in DFU: HWB must be low, nRESET must be triggered
- Flash the App
  dfu-programmer atmega32u2 erase
  dfu-programmer atmega32u2 flash ./CEC2Usb.hex
