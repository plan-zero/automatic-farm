# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(PROGRAMMER)

include_directories(./../libraries/modules/nrf24/inc)
include_directories(./../libraries/drivers/uart/inc)

set(TARGET_NAME programmer)
set(NRF24HEX nrf24.hex)
set(TARGET_EXT_NAME _ext.hex)

avr_add_executable(${TARGET_NAME} ./src/main.c)
target_link_libraries(${TARGET_NAME}.elf nrf24_api uart)

#set postbuild command to concatenate programmer with nrf24
add_custom_command(TARGET programmer POST_BUILD
    COMMAND test ! -e ${CMAKE_CURRENT_BINARY_DIR}/../firmware/${NRF24HEX} || 
    head -n-1 ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex > ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}${TARGET_EXT_NAME}
    COMMAND test ! -e ${CMAKE_CURRENT_BINARY_DIR}/../firmware/${NRF24HEX} || 
    cat ${CMAKE_CURRENT_BINARY_DIR}/../firmware/${NRF24HEX} >> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}${TARGET_EXT_NAME}
)


#set command to clean the programmer + nrf24 output
get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
set_property(DIRECTORY APPEND
    PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${TARGET_NAME}${TARGET_EXT_NAME}" "${TARGET_NAME}.map"
)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex
          ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}${TARGET_EXT_NAME}
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/${AVR_MCU}/${TARGET_NAME}

)
