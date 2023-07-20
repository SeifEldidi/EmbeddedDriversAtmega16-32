/*
 * BitBangedUart.h
 *
 *  Created on: Jul 10, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_BITBANGEDUART_H_
#define MCAL_BITBANGEDUART_H_

/**Includes Section**/
#include "Mcal_gpio.h"
/***Define Section***/
#define NUM_STOP_BITS_2 0x00
#define NUM_STOP_BITS_1 0x01

#define Parity_EVEN 0x00
#define Parity_ODD  0x01
#define Parity_NO     0x02
/***typedef section***/
typedef struct
{
	GPIO_PIN_t TX;
	GPIO_PIN_t RX;
	uint8 No_STOP_Bits:1;
	uint8 Parity_bits:2;
	uint8 Res:5;
	uint32 BaudRate;
}BitBanged_USART;

/**Software Interfaces**/
uint8 BitBanged_Usart_init(BitBanged_USART *Usart);
uint8 BITBanged_Usart_Send(BitBanged_USART *Usart,uint8 Data);
uint8 BITBanged_Usart_SendStr(BitBanged_USART *Usart,uint8 *Data);
uint8 BitBanged_Usart_RX(BitBanged_USART *Usart,uint8 *Data);
uint8 BitBanged_Usart_RXStr(BitBanged_USART *Usart,uint8 *Data);

#endif /* MCAL_BITBANGEDUART_H_ */
