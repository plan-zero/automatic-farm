#!/bin/bash
#setting the toolchain to PATH variable

BASEDIR=$(pwd -L)
TOOLCHAIN_NAME="avr8-gnu-toolchain-linux_x86_64"

if [ -d "$BASEDIR/$TOOLCHAIN_NAME" ]; then
    echo "Toolchain found, setting the PATH env variable"
    echo "GNU binaries dir: $BASEDIR/$TOOLCHAIN_NAME/bin"

    if [[ $PATH != *"$BASEDIR/$TOOLCHAIN_NAME/bin"* ]]; then
        echo "Setting PATH env variable: $PATH:$BASEDIR/$TOOLCHAIN_NAME/bin"
        echo export PATH="$PATH:$BASEDIR/$TOOLCHAIN_NAME/bin" >> ~/.bashrc
        echo "Make PATH permanent..."
        echo "$PATH:$BASEDIR/$TOOLCHAIN_NAME/bin" > /etc/environment
    else
        echo "Toolchain's path is already in PATH env variable"
    fi

else
    echo "Toolchain is not installed...Please visit https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers and download the AVR 8-bit Toolchain 3.6.2 - Linux 64-bit"
fi