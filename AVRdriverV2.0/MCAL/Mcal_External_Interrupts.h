/*
 * External_Interrupts.h
 *
 *  Created on: May 27, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_EXTERNAL_INTERRUPTS_H_
#define MCAL_MCAL_EXTERNAL_INTERRUPTS_H_
/******Includes Section***********/
#include "mcal_std_types.h"
/************Define Section***************/
#define ADC_interrupt_enable TRUE
#define SPI_INT_EN FALSE
#define USART_INT_RX  FALSE
#define USART_INT_TX  FALSE
#define Timer0_OVF_Int  TRUE
#define Timer0_CTC_Int  FALSE
#define I2C_RECIEVE_INTERRUPT FALSE
/***********Software Interfaces**********/
#if ADC_interrupt_enable==TRUE
void ADC_ISR(void);
#endif
#if USART_INT_TX==TRUE
void UASRT_TX_ISR(void);
#endif
#if USART_INT_RX==TRUE
void UASRT_RX_ISR(void);
#endif
#if Timer0_OVF_Int==TRUE
void TIMER_ISR_internal(void);
#endif
#endif /* MCAL_MCAL_EXTERNAL_INTERRUPTS_H_ */
