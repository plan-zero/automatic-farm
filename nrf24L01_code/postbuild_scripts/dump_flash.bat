echo off
set AVRDUDE_PATH=.\..\..\tools\avrdude
set OUT_PATH=.\..\out
%AVRDUDE_PATH%\avrdude.exe -c usbasp -p m16 -P usb -B 1 -U flash:r:%OUT_PATH%\nrf24_dump.bin:i