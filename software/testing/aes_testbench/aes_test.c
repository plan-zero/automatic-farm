#include "stdlib.h"
#include <avr/io.h>
#include <util/delay.h>

#include "aes.h"
#include "uart.h"

uint8_t data[] = {'A', 'N', 'D', 'R', 'E', 'I','A', 'N', 'D', 'R', 'E', 'I','1','2','3','4'};
<<<<<<< HEAD
uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
=======
>>>>>>> a345e23fcef637e3a9744de000e1cbaea67fc7cb

int main()
{
    DDRB |= 1;
    uart_init(UART_9600BAUD, UART_8MHZ, UART_PARITY_NONE);
    uart_printString("AES test",1);
    uint8_t key[16] = {'1','2','3','4','5','6','7','8','9','1','2','3','4','5','6','7'};
    uint8_t expkey[176] = {0};
    aesKeyExpand(key, expkey);
<<<<<<< HEAD
    AES_init_ctx(&ctx, key);
=======
>>>>>>> a345e23fcef637e3a9744de000e1cbaea67fc7cb
    for(uint8_t i = 0; i < 16; i++)
        uart_sendByte(data[i]);
    uartNewLine();
    PORTB ^= 1;
    aesCipher(expkey, data);
<<<<<<< HEAD
    AES_ECB_encrypt(&ctx, data);
=======
>>>>>>> a345e23fcef637e3a9744de000e1cbaea67fc7cb
    for(uint8_t i = 0; i < 16; i++)
        uart_sendByte(data[i]);
    uartNewLine();
    PORTB ^= 1;
    aesInvCipher(expkey, data);
<<<<<<< HEAD
    AES_ECB_decrypt(&ctx, data);
=======
>>>>>>> a345e23fcef637e3a9744de000e1cbaea67fc7cb
    for(uint8_t i = 0; i < 16; i++)
        uart_sendByte(data[i]);
    uartNewLine();

    uart_printString("Aes done",1);
    PORTB ^= 1;
    
    
    while(1)
    {
        _delay_ms(1000);
    }

    return 0;
}