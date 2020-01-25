/*
 * E2P_Layout.h
 *
 * Created: 12/2/2019 9:16:29 PM
 *  Author: Adi
 */ 


#ifndef E2P_LAYOUT_H_
#define E2P_LAYOUT_H_

#define RADIO_RX_ADDRESS			((const uint8_t*) 0)
#define RADIO_RX_LENGTH				((uint8_t) 5)

#define E2P_CKS_ADDRESS				((const uint8_t*) (RADIO_RX_ADDRESS + RADIO_RX_LENGTH))
#define E2P_CKS_LENGTH				((uint8_t) 4)

#define FLASH_CKS_ADDRESS			((const uint8_t*) (E2P_CKS_ADDRESS + E2P_CKS_LENGTH))
#define FLASH_CKS_LENGTH			((uint8_t) 4)

#define NRF_VERS_ADDRESS			((const uint8_t*) (FLASH_CKS_ADDRESS + FLASH_CKS_LENGTH))
#define NRF_VERS_LENGTH				((uint8_t) 2)

#define APP_VERS_ADDRESS			((const uint8_t*) (NRF_VERS_ADDRESS + NRF_VERS_LENGTH))
#define APP_VERS_LENGTH				((uint8_t) 2)

#define E2P_VERS_ADDRESS			((const uint8_t*) (APP_VERS_ADDRESS + APP_VERS_LENGTH))
#define E2P_VERS_LENGTH				((uint8_t) 2)

#define RESERVED1_ADDRESS			((const uint8_t*) (E2P_VERS_ADDRESS + E2P_VERS_LENGTH))
#define RESERVED1_LENGTH			((uint8_t) 2)

#define DOWNLOAD_FLAG_ADDRESS		((const uint8_t*) (RESERVED1_ADDRESS + RESERVED1_LENGTH))
#define DOWNLOAD_FLAG_LENGTH		((uint8_t) 1)

#define PROGRAMMER_ADDR_ADDRESS		((const uint8_t*) (DOWNLOAD_FLAG_ADDRESS + DOWNLOAD_FLAG_LENGTH))
#define PROGRAMMER_ADDR_LENGTH		((uint8_t) 5)

#define RESERVED2_ADDRESS			((const uint8_t*) (PROGRAMMER_ADDR_ADDRESS + PROGRAMMER_ADDR_LENGTH))
#define RESERVED2_LENGTH			((uint8_t) 2)

#define PROGRAMMER_SECRET_KEY		((const uint8_t*) (RESERVED2_ADDRESS + RESERVED2_LENGTH))
#define PROGRAMMER_KEY_LENGTH		10l

#endif /* E2P_LAYOUT_H_ */