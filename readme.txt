This project is working great with RPI 3 and atmega16/328p microcontrollers, feel free to adapt the code to your development board.

The code is developed and maintained using the cmake 3.10.2 and AVR_8_bit_GNU_Toolchain_3.6.1_1750.

This firmware contains a new bootloader implementation that is working alongside the NRF24L01 Radio module to download the application code to the target. 

Steps to build the software:

cd software
mkdir build
cmake -DAVR_MCU=<MCU> -DF_CPU=<FREQ> ..
make
make install

Notes: 
	- <MCU>: atmega16 | atmega328p
	- <FREQ>: 1000000UL | 2000000UL | 4000000UL | 8000000UL | 16000000UL
	
The executables (only .hex files) are installed in software/assets/{AVR_MCU} . There we have the following:
	- app: the user application executable
	- firmware: the firmware executable and eeprom data (firmware.hex & firmware_eep.hex)
	- programmer: the programmer executable used as a gateway between target and the host programmer (raspberry pi / x86)

==============================================================================================================================
Implementation Details:
Because the bootloader section is limited (the maximum allowed size for atmega16/328p is 2KB) I had to move the NRF24 library code below the bootloader section.

Also I wanted to give access to both, application and bootloader, to the NRF24 library, so I created a table of pointers to the NRF24 APIs which is loaded a the beginning of NRF24 code.

Find below the defined FLASH layer for atmega16 (the atmega328p flash layout is alike):

ADDR	FLASH
         _______________________
0x4000  |                       |
        | Bootloader            |
        | 2KB                   |
0x3800  |_______________________|
        |                       |
        | NRF24 Library	        |
        | 3.93KB (4032B)        |
        |                       |
        |                       |
        |                       |
0x2840  |_______________________|
0x2800  | NRF24 APIs HOOKs 64B  |
        |_______________________|
        |                       |
        |                       |
        |                       |
        |  USER APPLICATION     |
        |  10KB                 |
        |                       |
        |                       |
        |                       |
        |                       |
        |_______________________|

For the bootloader section, the AVR microcontroller must have the Boot Reset vector enabled (the BOOTRST bit is zero in fusebits) and the size have to be set to 1024 Words (2KB, BOOTSZ = 00).

The linker must include the following flag: -Wl,-section-start=.text=0x3800 (this is moving the compiled code to the 0x3800 address). Because the AVR is word addressable, this actually is 0x1C00, but the linker will do the math for you so you can use the byte unit for addressing. For atmega328p this should be set to -Wl,-section-start=.text=0x7000 - consult your microcontroller datasheet to find the BLS starting address.

To put the NRF24 library at a specific address, I used the following section: -Wl,--section-start=.nrf24=0x2840. Then I added the code to this section, therefore I defined the following macro: "#define NRF24_MEMORY __attribute__((section(".nrf24")))", so this should be added at the beginning of each function declaration that is added to .nrf24 section: e.g "NRF24_MEMORY radio_error_code nrfRadio_PowerDown();". 

If you want to place the section somewhere else, just change the 0x2840 address with another available flash location (0-16KB if atmega16 is used).

The hooks actually are a simple array of function pointers and these are pointing to the NRF24 APIs. The purpose of using these pointers is to share these APIs between bootloader and user application. In this case the section is defined as: -Wl,--section-start=.radio_fptrs=0x2800

Each function address is 2 bytes long, so in this case we have enough space for 32 APIs (the section is 64 Bytes). If you want to change this section, also change the NRF_LIB_HOOKS_BASE_ADDR define, which should be the same as the starting address of your section. In this project both are set to 0x2800.

The hooks are defined in radio_fptr.h file e.g: "radio_error_code (* const __flash *fptr_nrfRadio_Main)(void) = (radio_error_code(* const __flash *)(void))NRF_LIB_HOOKS_BASE_ADDR + OFFSET" where:

NRF_LIB_HOOKS_BASE_ADDR - is the starting address of .radio_fptrs section
OFFSET - is the address offset, which is a multiple of two - the address is two bytes long

The pointers array is defined as "ptrs" and it can be checked in the bottom of nrf24Radio.c file:

fptr_t ptrs[] __attribute__((used, section(".radio_fptrs"))) = {
	(fptr_t)nrfRadio_Main,          //OFFSET is zero, the address is NRF_LIB_HOOKS_BASE_ADDR + 0
	(fptr_t)nrfRadio_TransmitMode,  //OFFSET is 2, the address is NRF_LIB_HOOKS_BASE_ADDR + 2
	(fptr_t)nrfRadio_Init,          //OFFSET is 4, the address is NRF_LIB_HOOKS_BASE_ADDR + 4
...

Also I used the following flag in linker: -Wl,--undefined=ptrs. Without this the linker will not add the .radio_fptrs section unless the garbage collect is disabled in linker optimization (which is not a good idea, some code that is not used will be added to flash).

Another option to resolve this problem is to reference the ptrs variable, so in this case the compiler will mark the variable as used - even I put the "used" attribute, the linker will still miss this due the compiler.
NOTE: Before you use the hooks, make sure that your microcontroller has loaded the NRF24 library in FLASH memory, otherwise you'll jump to an empty FLASH and the CPU will execute NOP instructions until memory overflow or until it will hit some valid code, this can lead to undefined behavior.
