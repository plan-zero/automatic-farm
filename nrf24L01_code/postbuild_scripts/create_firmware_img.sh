echo off

OUT_PATH="./../../out"

echo Extracting images...


BOOTLOADER_START=0x3800
BOOTLOADER_END=0x3FFF
echo Remove prevoius bootloader image...
rm $OUT_PATH/nrf24_bootloader.hex 2> /dev/null
echo Extracting bootloader image...
while IFS="" read -r p || [ -n "$p" ]
do
  #printf '%s\n' "$p"
  address=${p:3:4}
  
if [[ 0x$address -ge $BOOTLOADER_START ]]
then
	if [[ 0x$address -le $BOOTLOADER_END ]]
	then
		echo $p >> $OUT_PATH/nrf24_bootloader.hex
	fi
fi
done < bootloader.hex


NRF_START=0x2800
NRF_END=0x37FF
echo Remove prevoius radio image...
rm $OUT_PATH/nrf24_radio.hex 2> /dev/null
echo Extracting radio image...
while IFS="" read -r p || [ -n "$p" ]
do
  #printf '%s\n' "$p"
  address=${p:3:4}
  
if [[ 0x$address -ge $NRF_START ]]
then
	if [[ 0x$address -le $NRF_END ]]
	then
		echo $p >> $OUT_PATH/nrf24_radio.hex
	fi
fi
done < bootloader.hex

echo Create firmware image...
cat $OUT_PATH/nrf24_radio.hex $OUT_PATH/nrf24_bootloader.hex > $OUT_PATH/nrf24_firmware.hex

echo create E2P file...
cp ./../m16.hex $OUT_PATH/nrf24_firmware_e2p.hex

read -p "Press any key to exit..."
exit 0