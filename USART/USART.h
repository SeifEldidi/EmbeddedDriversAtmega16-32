/*
 * USART.h
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */

#ifndef USART_H_
#define USART_H_


/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define USART_TX_COMPLETE  1
#define USART_TX_NOT_COMPLETE 0

#define USART_RX_COMPLETE 1
#define UASRT_RX_NOT_COMPLETE 0

#define USART_BUFFER_EMPTY 1
#define USART_BUFFER_FULL 0

#define USART_DOUBLE_SPEED_ENABLE 1
#define USART_DOUBLE_SPEED_DISABLE 0

#define USART_FRAME_ERROR 1
#define USART_NO_FRAME_ERROR 0

#define USART_DATA_OVERRUN_ERROR 1
#define USART_NO_DATA_OVERRUN_ERROR 0

#define USART_PARITY_ERROR 1
#define USART_NO_PARITY_ERROR 0

#define USART_TWO_STOP_BITS 1
#define USART_ONE_STOP_BIT  0

/***********Macro Like Function**************/
#define USART_DOUBLE_SPEED_ENABLE_()  (SET_BIT(UCSRA,U2X))
#define USART_DOUBLE_SPEED_DISABLE_() (CLEAR_BIT(UCSRA,U2X))

#define USART_RX_ENABLE()  (SET_BIT(UCSRB,RXEN))
#define USART_RX_DISABLE()  (CLEAR_BIT(UCSRB,RXEN))

#define USART_TX_ENABLE()  (SET_BIT(UCSRB,TXEN))
#define USART_TX_DISABLE()  (CLEAR_BIT(UCSRB,TXEN))

#define USART_ASYNC_MODE()  (CLEAR_BIT(UCSRC,UMSEL))
#define USART_SYNC_MODE()   (SET_BIT(UCSRC,UMSEL))

#define USART_TWO_STOP_BITS_() (SET_BIT(UCSRC,USBS))
#define USART_ONE_STOP_BIT_()  (CLEAR_BIT(UCSRC,USBS))

#define USSCRC_SEL() (SET_BIT(UCSRC,URSEL))
#define UBBRH_SEL() (CLEAR_BIT(UCSRC,URSEL))

#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
#define USART_TX_CLEAR_FLAG() (SET_BIT(UCSRA,TXC))
#define USART_RX_CLEAR_FLAG() (SET_BIT(UCSRA,RXC))

#define USART_TX_INT_ENABLE() (SET_BIT(UCSRB,TXEN))
#define USART_RX_INT_ENABLE() (SET_BIT(UCSRB,RXEN))

#define USART_TX_INT_DISABLE() (CLEAR_BIT(UCSRB,TXEN))
#define USART_RX_INT_DISABLE() (CLEAR_BIT(UCSRB,RXEN))

#endif

/*************Defined Data types**************/
typedef enum
{
	USART_5BITS=0,
	USART_6BITS,
	USART_7BITS,
	USART_8BITS,
	USART_RES_1,
	USART_RES_2,
	USART_RES_3,
	USART_9BITS,
}USART_DATA_SIZE;

typedef enum
{
	USART_PARITY_DISABLE=0,
	USART_PARITY_ENABLE_EVEN,
	USART_PARITY_ENABLE_ODD,
}USART_PARITY_SET;

typedef struct
{
	uint16 Baudrate;
	uint8 USART_NO_STOPBITS:1;
	uint8 USART_D2X_ENALBE:1;
	uint8 USART_reserved:6;
	USART_DATA_SIZE Data_Size;
	USART_PARITY_SET Parity;
#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
	void (*INT_Tx_handler)(void);
	void (*INT_Rx_handler)(void);
#endif
}USART_CONFIG_t;

/*********Software Interfaces*******************/
Bool USART_init(USART_CONFIG_t *USART);
Bool USART_send(uint8 data);
Bool USART_send_block(uint8 *data,uint8 size);
Bool USART_send_string(uint8 *data);
Bool USART_send_non_polling(uint8 data);
Bool USART_recieve(uint8 *data);
Bool USART_recieve_non_polling(uint8 *data);
Bool USART_recieve_string(uint8 *data);

#endif /* USART_H_ */
