#!/bin/bash

#check if avrdude is installed
out=$(command -V avrdude)

if [[ $out == *"avrdude is"* ]]; then
    echo "Avrdude is installed!"

    ##install avr_flash
    if grep -q "avr_flash" ~/.bashrc; then
        echo "avr_flash is already installed!"
    else
        echo "Install avr_flash!"
        ##generate code
        echo 'avr_flash() {' >> ~/.bashrc
        echo '    avrdude -c usbasp -p $1 -P usb -U flash:w:$2' >> ~/.bashrc
        echo '}' >> ~/.bashrc
    fi

    ##install avr_eeprom
    if grep -q "avr_eeprom" ~/.bashrc; then
        echo "avr_eeprom is already installed!"
    else
        echo "Install avr_eeprom!"
        ##generate code
        echo 'avr_eeprom() {' >> ~/.bashrc
        echo '    avrdude -c usbasp -p $1 -P usb -U eeprom:w:$2' >> ~/.bashrc
        echo '}' >> ~/.bashrc
    fi

    ##install avr_ota_push
    if grep -q "avr_ota_push" ~/.bashrc; then
        echo "avr_ota_push is already installed!"
    else
        echo "Install avr_ota_push!"
        #check python3
        out=$(command -V python3)
        if [[ $out == *"python3 is"* ]]; then
            ##generate code
            flash_tool_path=$(pwd)/flash_tool/flash-tool.py
            echo 'avr_ota_push() {' >> ~/.bashrc
            echo "usb_dev=\$(dmesg | grep \"FTDI USB Serial Device converter now attached\" | grep -Eo 'ttyUSB[0-9]+')" >> ~/.bashrc
            echo 'if [ -z "$3" ]; then' >> ~/.bashrc
            echo "    python3 $flash_tool_path -T \$2 -H \$1 -P /dev/\$usb_dev -B 250000 " >> ~/.bashrc
            echo 'else' >> ~/.bashrc
            echo "    python3 $flash_tool_path -T \$2 -H \$1 -K \$3 -P /dev/\$usb_dev -B 250000 " >> ~/.bashrc
            echo 'fi' >> ~/.bashrc
            echo '}' >> ~/.bashrc
        else
            echo "python3 not found, please run apt-get install python3!"
        fi

    fi

    ##check if the functions are already installed
else
    echo "Avrdude not found, please run apt-get install avrdude!"
fi