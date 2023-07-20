/*
 * External_Interrupts.c
 *
 *  Created on: May 27, 2023
 *      Author: Seif pc
 */
#include "Mcal_External_Interrupts.h"

#if ADC_interrupt_enable==TRUE
ISR(ADC_vect)
{
	ADC_ISR();
}
#endif

#if Timer0_OVF_Int==TRUE
ISR(TIMER0_OVF_vect)
{
	TIMER_ISR_internal();
}
#endif
