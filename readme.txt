This project is working great with RPI 3 and atmega16/32 microcontrolers, feel free to adapt the code to your development board.

NRF24L01 code
The code is developed and maintained using the Atmel Studio tool (7.0.122). The toolchain used to build the project is AVR GNU 8 BIT (gcc version 5.4.0, AVR_8_bit_GNU_Toolchain_3.6.1_1750).
The NRF24 firmware is a new bootloader which is using the NRF24L01 Radio to download the application to the target. 
Because the bootloader section is limitated (the maximum allowed size for atmega16 is 2KB) I had to move the NRF24 library code above the bootloader section.
Also I wanted to give access to both, application and bootloader, to the NRF24 library, so I created a table of pointers to the NRF24 APIs which is loaded a the begining of NRF24 code.
Above are defined the FLASH sections:

ADDR	FLASH
		 _______________________
0x4000	| 						|
		| Bootloader			|
		| 2KB					|
0x3800	|_______________________|
		|						|
		| NRF24 Library			|
		| 3.93KB (4032B)		|
		|						|
		|						|
		|						|
0x2840	|_______________________|
0x2800	| NRF24 APIs HOOKs 64B	|
		|_______________________|
		|						|
		|						|
		|						|
		|  USER APPLICATION		|
		|	10KB				|
		|						|
		|						|
		|						|
		|						|
		|_______________________|

For the bootloader section, the AVR microcontroler must have the Boot Reset vector enabled (the BOOTRST bit is zero in fusebits) and the size have to be set to 1024 Words (2KB, BOOTSZ = 00).
The linker must include the following flag: -Wl,-section-start=.text=0x3800 (this is moving the compiled code to the 0x3800 address, this actually is 0x1C00 because the AVR is word addressable, but the linker will do the math for you if the byte unit is used.
To put the NRF24 library at a specific address, I used the following section: -Wl,--section-start=.nrf24=0x2840, then to add the code to this section, I defined the following macro: "#define NRF24_MEMORY __attribute__((section(".nrf24")))", so this should be added at the begining of each function declaration that you want to add to the .nrf24 section: e.g "NRF24_MEMORY radio_error_code nrfRadio_PowerDown();". If you want to place the section somewhere else, just change the 0x2840 address with another available flash location (0-16KB if atmega16 is used).
The hooks actually are a simple array of pointers to the NRF24 APIs the the purpose is to share these APIs between bootloader and user application. The section is defined as: -Wl,--section-start=.radio_fptrs=0x280
Each function address is 2 bytes long, so in this case we have enough space for 32 APIs (the section is 64 Bytes). If you want to change this section, also change the NRF_LIB_HOOKS_BASE_ADDR define, which should be the same as the starting address of your section. In this project both are set to 0x2800.
The hooks are defined in radio_fptr.h file e.g: "radio_error_code (* const __flash *fptr_nrfRadio_Main)(void) = (radio_error_code(* const __flash *)(void))NRF_LIB_HOOKS_BASE_ADDR + OFFSET" where:
NRF_LIB_HOOKS_BASE_ADDR - is the starting address of .radio_fptrs section
OFFSET - is the pointer array offset which is a multiple of two - the function's address is two bytes long
The pointers array is defined as and it can be checked in the buttom of nrf24Radio.c :
fptr_t ptrs[] __attribute__((used, section(".radio_fptrs"))) = {
	(fptr_t)nrfRadio_Main,			//OFFSET is zero, the address is NRF_LIB_HOOKS_BASE_ADDR + 0
	(fptr_t)nrfRadio_TransmitMode,	//OFFSET is 2, the address is NRF_LIB_HOOKS_BASE_ADDR + 2
	(fptr_t)nrfRadio_Init,			//OFFSET is 4, the address is NRF_LIB_HOOKS_BASE_ADDR + 4
...

Also I used the following flag in linker: -Wl,--undefined=ptrs. Without this the linker will not add the .radio_fptrs section unless the garbage collect is disabled in linker optimization (which is not a good idea). Another option is to reference the ptrs variable, so in this case the compiler will mark the variable as used - even I put the "used" attribute, the linker will still miss this because due the compiler.
NOTE: Before you use the hooks, make sure that your microcontroler has loaded the NRF24 library in FLASH memory, otherwise you'll jump to an empty FLASH and the CPU will execute NOP instruction until memory overflow or until it will hit some valid code.

The Atmel solution contains the following projects:
application:
	This is used as a stub for the high level and the goal is to test the NRF24 library. This will not be a part of firmware.
bootloader:
	This is the firmware project, the NRF24 library is added to this project and it compiles both, the NRF24 code (including the hooks) and the bootloader code.
radio:
	This is the NRF24 code, this is compiled as a static library and the solution can be added to any project.
uart
	This is a support library for UART and the goal is to provide a minimal interface for debugging over serial. This is compiled as a static library and it can be added to any project.
