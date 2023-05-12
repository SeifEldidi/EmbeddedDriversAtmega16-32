/*
 * Internal_Interrupts.c
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */
#include "Interrupts.h"

//ISR(ADC_vect)
//{
//	ADC_ISR_();
//}

//ISR(USART_TXC_vect)
//{
//	USART_TX_ISR();
//}
//
//ISR(USART_RXC_vect)
//{
//	USART_RX_ISR();
//}

//ISR(TIMER1_OVF_vect)
//{
//	Timer1_ovf_ISR();
//}
//
//ISR(TIMER1_CAPT_vect)
//{
//	Timer1_capt_ISR();
//}

#if I2C_RECIEVE_INTERRUPT==FEATURE_ENABLE
ISR(TWI_vect)
{
	I2C_ISR_();
}
#endif

