echo off
set AVRDUDE_PATH=.\..\..\tools\avrdude
set OUT_PATH=.\..\out
%AVRDUDE_PATH%\avrdude.exe -c usbasp -p m16 -P usb -B 1 -U flash:w:%OUT_PATH%\nrf24_firmware.hex
%AVRDUDE_PATH%\avrdude.exe -c usbasp -p m16 -P usb -B 1 -U eeprom:w:%OUT_PATH%\nrf24_firmware_e2p.hex:i