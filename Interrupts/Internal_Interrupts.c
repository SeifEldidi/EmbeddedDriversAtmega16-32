/*
 * Internal_Interrupts.c
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */
#include "Interrupts.h"

ISR(ADC_vect)
{
	ADC_ISR_();
}

ISR(USART_TXC_vect)
{
	USART_TX_ISR();
}

ISR(USART_RXC_vect)
{
	USART_RX_ISR();
}

