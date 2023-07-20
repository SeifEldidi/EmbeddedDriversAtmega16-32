/*
 * mcal_uart.h
 *
 *  Created on: May 26, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_UART_H_
#define MCAL_MCAL_UART_H_

/*************Includes Section*********/
#include "mcal_std_types.h"
/*************Define Section*********/
#define USART_DOUBLE_SPEED_ENABLE 	0x00
#define USART_DOUBLE_SPEED_DISABLE 	0x01

#define USART_1_STOP_BITS 0x00
#define USART_2_STOP_BITS 0x01
/**********Function Like Macro******/
#define USART_D2X_ENABLE() (SET_BIT(UCSRA,U2X))
#define USART_D2X_DISABLE() (CLEAR_BIT(UCSRA,U2X))
#define USART_ASYNCMODE() (CLEAR_BIT(UCSRC,UMSEL))

#define USART_TX_INT_ENABLE() (SET_BIT(UCSRB,TXCIE))
#define USART_RX_INT_ENABLE() (SET_BIT(UCSRB,RXCIE))

#define USART_TX_FLAG_CLEAR() (SET_BIT(UCSRA,TXC))
#define USART_RX_FLAG_CLEAR() (SET_BIT(UCSRA,RXC))

#define USART_TX_INT_DISABLE() (CLEAR_BIT(UCSRB,TXCIE))
#define USART_RX_INT_DISABLE() (CLEAR_BIT(UCSRB,RXCIE))

#define USART_TX_ENABLE() (SET_BIT(UCSRB,TXEN))
#define USART_RX_ENABLE() (SET_BIT(UCSRB,RXEN))

#define USART_TX_DISABLE() (CLEAR_BIT(UCSRB,TXEN))
#define UASRT_RX_DISABLE() (CLEAR_BIT(UCSRB,RXEN))

#define USART_TX_IN_PROGRESS() (READ_BIT(UCSRA,TXC))
#define UASRT_RX_IN_PROGRESS() (!READ_BIT(UCSRA,RXC))

#define USART_BUFFER_NEMPTY() (!READ_BIT(UCSRA,UDRE))

#define USART_USCRC_SEL() (SET_BIT(UCSRC,URSEL))
#define USART_PARITY_ERROR() (READ_BIT(UCSRA,PE))
/***************Defined Data types**********/
typedef enum
{
	USART_PARITY_DISABLED,
	USART_PARITY_RES,
	USART_PARITY_EVEN,
	USART_PARITY_ODD,
}Parity_Mode;

typedef enum
{
	USART_DATA_5_BITS,
	USART_DATA_6_BITS,
	USART_DATA_7_BITS,
	USART_DATA_8_BITS,
	USART_DATA_res_BITS,
	USART_DATA_res_BITS_,
	USART_DATA_res_BITS__,
	USART_DATA_9_BITS,
}USART_BUFF_SIZE;

typedef struct
{
	uint32 Baud_rate;
	uint8 Enhanced_Speed;
	uint8 NO_stop_bits;
	Parity_Mode P_Mode;
	USART_BUFF_SIZE Buff_size;
	#if USART_INT_TX==TRUE
	void (*UASRT_TX_callback)(void);
	#endif
	#if USART_INT_RX==TRUE
	void(* UASRT_RX_callback)(void);
	#endif
}USART_Config_t;

uint8 Mcal_Usart_init(USART_Config_t *USART);
uint8 Mcal_Usart_send(USART_Config_t *USART,uint8 Data);
uint8 Mcal_Usart_send_string(USART_Config_t *USART,uint8* Data);
uint8 Mcal_Usart_rx(USART_Config_t *USART,uint8 *Data);
uint8 Mcal_Usart_rx_string(USART_Config_t *USART,uint8 *Data);
#endif /* MCAL_MCAL_UART_H_ */
