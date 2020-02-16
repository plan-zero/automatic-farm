#include "e2p_layout.h"
#include "stdint.h"
#include "string.h"

#define E2P_EXTRACT_BYTE(string, pos) (uint8_t)string[pos]

union e2p_data
{
    struct{
        uint8_t rx_address[RADIO_RX_LENGTH];
        uint8_t e2p_checksum[E2P_CKS_LENGTH];
        uint8_t flash_checksum[FLASH_CKS_LENGTH];
        uint8_t nrf_version[NRF_VERS_LENGTH];
        uint8_t app_version[APP_VERS_LENGTH];
        uint8_t e2p_version[E2P_VERS_LENGTH];
        uint8_t reserved1[RESERVED1_LENGTH];
        uint8_t download_flag[DOWNLOAD_FLAG_LENGTH];
        uint8_t programmer_address[PROGRAMMER_ADDR_LENGTH];
        uint8_t reserved2[RESERVED2_LENGTH];
        uint8_t programmer_key[PROGRAMMER_KEY_LENGTH];
    }e2p_fields;
    uint8_t e2p_raw[E2P_SIZE];
};

union e2p_data e2p_layout __attribute__((section(".eeprom"))) = 
{
    .e2p_fields.rx_address = 
        {
         E2P_RX_ADDRESS_BYTE_0,
         E2P_RX_ADDRESS_BYTE_1, 
         E2P_RX_ADDRESS_BYTE_2,
         E2P_RX_ADDRESS_BYTE_3,
         E2P_RX_ADDRESS_BYTE_4,
        },
    .e2p_fields.download_flag = E2P_DOWNLOAD_FLAG_VALUE
};