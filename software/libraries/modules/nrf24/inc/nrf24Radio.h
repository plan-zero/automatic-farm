/*
 * nrf24Radio.h
 *
 * Created: 11/19/2019 1:38:52 PM
 *  Author: amestereaga
 */ 


#ifndef NRF24RADIO_H_
#define NRF24RADIO_H_

#include "stdint.h"



typedef enum _crc {
	RADIO_CRC1_ENABLED,
	RADIO_CRC2_ENABLED,
	RADIO_CRC_DISABLED
}radio_crc;

typedef enum _count_wave {
	RADIO_COUNT_WAVE_ENABLED,
	RADIO_COUNT_WAVE_DISABLED
}radio_count_wave;

typedef enum _dynamic_payload {
	RADIO_DYNAMIC_PAYLOAD_ENABLED,
	RADIO_DYNAMIC_PAYLOAD_DISABLED	
}radio_dynamic_payload;

typedef enum _ack_payload {
	RADIO_ACK_PAYLOAD_ENABLED,
	RADIO_ACK_PAYLOAD_DISABLED	
}radio_ack_payload;

typedef enum _dynamic_ack {
	RADIO_DYNAMIC_ACK_ENABLED,
	RADIO_DYNAMIC_ACK_DISABLED	
}radio_dynamic_ack;

typedef enum _address_length{
	RADIO_ADDRESS_ILLEGAL = 0,
	RADIO_ADDRESS_3BYTES  = 0x1,
	RADIO_ADDRESS_4BYTES  = 0x2,
	RADIO_ADDRESS_5BYTES  = 0x3	
}radio_address_length;

typedef enum _retransmit_delay{
	RADIO_RETRANSMIT_WAIT_250US = 0,
	RADIO_RETRANSMIT_WAIT_500US = 0x1,
	RADIO_RETRANSMIT_WAIT_750US = 0x2,
	RADIO_RETRANSMIT_WAIT_1000US = 0x3,
	RADIO_RETRANSMIT_WAIT_1250US = 0x4,
	RADIO_RETRANSMIT_WAIT_1500US = 0x5,
	RADIO_RETRANSMIT_WAIT_1750US = 0x6,
	RADIO_RETRANSMIT_WAIT_2000US = 0x7,
	RADIO_RETRANSMIT_WAIT_2250US = 0x8,
	RADIO_RETRANSMIT_WAIT_2500US = 0x9,
	RADIO_RETRANSMIT_WAIT_2750US = 0xA,
	RADIO_RETRANSMIT_WAIT_3000US = 0xB,
	RADIO_RETRANSMIT_WAIT_3250US = 0xC,
	RADIO_RETRANSMIT_WAIT_3500US = 0xD,
	RADIO_RETRANSMIT_WAIT_3750US = 0xE,
	RADIO_RETRANSMIT_WAIT_4000US = 0xF
}radio_retransmit_delay;

typedef enum _retransmit_count{
	RADIO_RETRANSMIT_DISABLED = 0,
	RADIO_RETRANSMIT_1,
	RADIO_RETRANSMIT_2,
	RADIO_RETRANSMIT_3,
	RADIO_RETRANSMIT_4,
	RADIO_RETRANSMIT_5,
	RADIO_RETRANSMIT_6,
	RADIO_RETRANSMIT_7,
	RADIO_RETRANSMIT_8,
	RADIO_RETRANSMIT_9,
	RADIO_RETRANSMIT_10,
	RADIO_RETRANSMIT_11,
	RADIO_RETRANSMIT_12,
	RADIO_RETRANSMIT_13,
	RADIO_RETRANSMIT_14,
	RADIO_RETRANSMIT_15,
}radio_retrnasmit_count;

typedef enum _pipes {
	RADIO_PIPE0 = 0,
	RADIO_PIPE1 = 1,	
	RADIO_PIPE2 = 2,
	RADIO_PIPE3 = 3,
	RADIO_PIPE4 = 4,
	RADIO_PIPE5 = 5,
	RADIO_PIPE_EMPTY = 7
}radio_pipe;

typedef enum _data_rate {
	RADIO_2MBIT,
	RADIO_1MBIT,
	RADIO_250KBIT
}radio_data_rate;

typedef enum _frequency {
	CHANNEL_112 = 112,
	CHANNEL_111 = 111,
	CHANNEL_110 = 110
}radio_freq_channel;

typedef enum _rf_power {
	RADIO_LOWEST_18DBM = 0x0,
	RADIO_LOW_12DBM = 0x1,
	RADIO_HIGH_6DBM = 0x2,
	RADIO_HIGHEST_0DBM = 0x3
}radio_rf_power;

typedef enum _pipe_enable_aa {
	RADIO_PIPE_AA_DISABLED,
	RADIO_PIPE_AA_ENABLED
}radio_pipe_enable_aa;

typedef enum _pipe_enable_rx_address {
	RADIO_PIPE_RX_ENABLED,
	RADIO_PIPE_RX_DISABLED
}radio_pipe_enable_rx_address;

typedef enum _pipe_enable_dynamic_payload {
	RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED,
	RADIO_PIPE_DYNAMIC_PYALOAD_DISABLED
}radio_pipe_enable_dynamic_payload;

typedef enum _radio_state {
	RADIO_STARTUP,
	RADIO_POWER_DOWN,
	RADIO_STANDBY_1,
	RADIO_STANDBY_2,
	RADIO_PTX,
	RADIO_PRX
}radio_state;

typedef enum _radio_usage{
	RADIO_APPLICATION,
	RADIO_BOOTLOADER
}radio_usage;

typedef enum {
	RADIO_ERR_UNKNOWN,
	RADIO_ERR_OK,
	RADIO_ERR_INVALID,
	RADIO_ERR_WRITE_ERR,
}radio_error_code;

typedef enum {
	RADIO_TX_OK,
	RADIO_TX_MAX_RT,
	RADIO_TX_OK_ACK_PYL
}radio_tx_status;

typedef enum {
	RADIO_RX_OK
}radio_rx_status;

typedef enum {
	RADIO_WAIT_TX,
	RADIO_RETURN_ON_TX
}radio_transmision_type;


//radio defines
#define RADIO_MAX_ADDRESS	5
#define RADIO_MAX_PIPES		6

typedef struct _radio_registers {
	union {
		struct {
			uint8_t _PRIM_RX : 1;
			uint8_t _PWR_UP : 1;
			uint8_t _CRCO : 1;
			uint8_t _EN_CRC : 1;
			uint8_t _MASK_MAX_RT : 1;
			uint8_t _MASK_TX_DS : 1;
			uint8_t _MASK_RX_DR : 1;
			uint8_t _Reserved_config : 1;
		};
		uint8_t config;
	};
	union {
		struct {
			uint8_t _ENAA_P0 : 1;
			uint8_t _ENAA_P1 : 1;
			uint8_t _ENAA_P2 : 1;
			uint8_t _ENAA_P3 : 1;
			uint8_t _ENAA_P4 : 1;
			uint8_t _ENAA_P5 : 1;
			uint8_t _Reserved_en_aa : 2;
		};
		uint8_t en_aa;	
	};
	union {
		struct {
			uint8_t _ERX_P0 : 1;
			uint8_t _ERX_P1 : 1;
			uint8_t _ERX_P2 : 1;
			uint8_t _ERX_P3 : 1;
			uint8_t _ERX_P4 : 1;
			uint8_t _ERX_P5 : 1;
			uint8_t _Reserved_en_rxaddr : 2;
		};	
		uint8_t en_rxaddr;
	};
	union {
		struct {
			uint8_t _AW : 2;
			uint8_t _Reserved_setup_aw : 6;
		};
		uint8_t setup_aw;	
	};
	union {
		struct {
			uint8_t _ARC : 4;
			uint8_t _ARD : 4;
		};
		uint8_t setup_retr;	
	};
	union {
		struct {
			uint8_t _RF_CH : 7;
			uint8_t _Reserved_rf_ch : 1;
		};
		uint8_t rf_ch;
	};
	union {
		struct {
			uint8_t _obsolete_rf_setup : 1;
			uint8_t _RF_PWR : 2;
			uint8_t _RF_DR_HIGH : 1;
			uint8_t _PLL_LOCK : 1;
			uint8_t _RF_DR_LOW : 1;
			uint8_t _Reserved_rf_setup : 1;
			uint8_t _COUNT_WAVE : 1;
		};
		uint8_t rf_setup;
	};
	union {
		struct {
			uint8_t _TX_FULL : 1;
			uint8_t _RX_P_NO : 3;
			uint8_t _MAX_RT : 1;
			uint8_t _TX_DS : 1;
			uint8_t _RX_DR : 1;
			uint8_t _Reserved_status : 1;
		};
		uint8_t status;
	};
	union {
		struct {
			uint8_t _ARC_CNT : 4;
			uint8_t _PLOS_CNT : 4;
		};
		uint8_t observed_tx;
	};
	union {
		struct {
			uint8_t _RPD : 1;
			uint8_t _Reserved_rpd : 7;
		};
		uint8_t rpd;
	};
	union {
		struct {
			uint64_t rx_addr_p0_0 : 8;
			uint64_t rx_addr_p0_1 : 8;
			uint64_t rx_addr_p0_2 : 8;
			uint64_t rx_addr_p0_3 : 8;
			uint64_t rx_addr_p0_4 : 8;
			uint64_t rx_addr_p0_unused1 : 8;
			uint64_t rx_addr_p0_unused2 : 8;
			uint64_t rx_addr_p0_unused3 : 8;
		};
		uint64_t rx_addr_p0;
	};
	union {
		struct {
			uint64_t rx_addr_p1_0 : 8;
			uint64_t rx_addr_p1_1 : 8;
			uint64_t rx_addr_p1_2 : 8;
			uint64_t rx_addr_p1_3 : 8;
			uint64_t rx_addr_p1_4 : 8;
			uint64_t rx_addr_p1_unused1 : 8;
			uint64_t rx_addr_p1_unused2 : 8;
			uint64_t rx_addr_p1_unused3 : 8;
		};
		uint64_t rx_addr_p1;
	};
	uint8_t rx_addr_p2;
	uint8_t rx_addr_p3;
	uint8_t rx_addr_p4;
	uint8_t rx_addr_p5;
	union {
		struct {
			uint64_t tx_addr_0 : 8;
			uint64_t tx_addr_1 : 8;
			uint64_t tx_addr_2 : 8;
			uint64_t tx_addr_3 : 8;
			uint64_t tx_addr_4 : 8;
			uint64_t tx_addr_unused1 : 8;
			uint64_t tx_addr_unused2 : 8;
			uint64_t tx_addr_unused3 : 8;
		};
		uint64_t tx_addr;
	};
	uint8_t rx_pw_p0;
	uint8_t rx_pw_p1;
	uint8_t rx_pw_p2;
	uint8_t rx_pw_p3;
	uint8_t rx_pw_p4;
	uint8_t rx_pw_p5;
	union {
		struct {
			uint8_t _RX_EMPTY : 1;
			uint8_t _RX_FULL : 1;
			uint8_t _Reserved_fifo_status1 : 2;
			uint8_t _TX_EMPTY : 1;
			uint8_t _TX_FULL_FIFO : 1;
			uint8_t _TX_REUSE : 1;
			uint8_t _Reserved_fifo_status2 : 1;
		};
		uint8_t fifo_status;	
	};
	union {
		struct {
			uint8_t _DPL_P0 : 1;
			uint8_t _DPL_P1 : 1;
			uint8_t _DPL_P2 : 1;
			uint8_t _DPL_P3 : 1;
			uint8_t _DPL_P4 : 1;
			uint8_t _DPL_P5 : 1;
			uint8_t _Reserved_dynpd : 2;
		};
		uint8_t dynpd;	
	};
	union {
		struct {
			uint8_t _EN_DYN_ACK : 1;
			uint8_t _EN_ACK_PAY : 1;
			uint8_t _EN_DPL : 1;
			uint8_t _Reserved_feature : 5;	
		};
		uint8_t feature;	
	};

}radio_registers;


typedef struct _radio_config {
	radio_address_length address_length;
	radio_retransmit_delay retransmit_delay;
	radio_retrnasmit_count retransmit_count;
	radio_freq_channel frequency_channel;
	radio_data_rate data_rate;
	radio_crc crc;
	radio_count_wave enable_count_wave;
	radio_rf_power rf_power;
	radio_dynamic_payload en_dynamic_payload_length;
	radio_ack_payload en_ack_payload;
	radio_dynamic_ack en_dynamic_ack;
	radio_usage usecase;
}radio_config;

typedef struct _pipe_config{
	radio_pipe pipe;
	uint8_t * rx_address;
	uint8_t payload_width;
	radio_pipe_enable_rx_address enable_rx_address;
	radio_pipe_enable_aa enable_aa;
	radio_pipe_enable_dynamic_payload enable_dynamic_payload_length;
}pipe_config;

typedef struct _radio_context{
	radio_state currentState;
	uint8_t address_length;
	uint8_t dynamic_payload;
	uint8_t txAddress[RADIO_MAX_ADDRESS];
	uint8_t rxAddressP0[RADIO_MAX_ADDRESS];
	uint8_t rxAddressP1[RADIO_MAX_ADDRESS];
	uint8_t rxAddressP2[1];
	uint8_t rxAddressP3[1];
	uint8_t rxAddressP4[1];
	uint8_t rxAddressP5[1];
	uint8_t rx_pipe_widths[RADIO_MAX_PIPES];
	uint16_t txHistory;
	volatile uint8_t irq_triggered;
	volatile uint8_t irq_status;
	void (*tx_callback)(radio_tx_status);
	void (*rx_callback)(uint8_t, uint8_t*, uint8_t);
}radio_context;


radio_error_code nrfRadio_GetInfo(radio_context *instance, radio_registers * reg);
radio_error_code nrfRadio_Init(radio_context *instance, radio_config cfg);
radio_error_code nrfRadio_PipeConfig(radio_context *instance, pipe_config pipe_cfg);
radio_error_code nrfRadio_LoadMessages(radio_context *instance, uint8_t * payload, uint8_t payload_length);
radio_tx_status nrfRadio_Transmit(radio_context *instance, uint8_t * tx_address, radio_transmision_type trans_type);
radio_error_code nrfRadio_Main(radio_context *instance);
radio_error_code nrfRadio_TransmitMode(radio_context *instance);
radio_error_code nrfRadio_ListeningMode(radio_context *instance);
radio_error_code nrfRadio_PowerUp(radio_context *instance);
radio_error_code nrfRadio_PowerDown(radio_context *instance);
radio_error_code nrfRadio_ChangeDataRate(radio_context *instance, radio_data_rate datarate);
radio_error_code nrfRadio_ChangePower(radio_context *instance, radio_rf_power power);
radio_error_code nrfRadio_SetTxCallback(radio_context *instance, void (*callback)(radio_tx_status));
radio_error_code nrfRadio_SetRxCallback(radio_context *instance, void (*callback)(uint8_t, uint8_t *, uint8_t));
radio_error_code nrfRadio_LoadAckPayload(radio_context *instance, radio_pipe pipe, uint8_t * payload, uint8_t payload_length);

typedef void(*fptr_t)(void);



#endif /* NRF24RADIO_H_ */