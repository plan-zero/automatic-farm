#when a new hardware is added, please update these strings as well
set(SUPPORTED_MCU "atmega16 atmega328p")
set(SUPPORTED_FREQ "1000000UL 2000000UL 4000000UL 8000000UL 16000000UL")

#check input arguments that are passed down to the submodules
#check AVR_MCU
if(AVR_MCU)
    string(FIND ${SUPPORTED_MCU} ${AVR_MCU} RES)
    if(${RES} MATCHES "-1")
           message(FATAL_ERROR "AVR_MCU=${AVR_MCU} is not a valid configuration. The supported MCUs are: ${SUPPORTED_MCU}")
    endif()
else()
    message(WARNING "The AVR_MCU is not set, this is set to default: atmega16")
    set(AVR_MCU atmega16)
endif()

if(MCU_FREQ)
    string(FIND ${SUPPORTED_FREQ} ${MCU_FREQ} RES)
    if(${RES} MATCHES "-1")
        message(FATAL_ERROR "MCU_FREQ=${AVR_MCU} is not a valid configuration. The supported MCU FREQ are: ${SUPPORTED_FREQ}")
    endif()
else()
    message(WARNING "The MCU_FREQ is not set, this is set to default: 8000000UL")
    set(MCU_FREQ 8000000UL)
endif()