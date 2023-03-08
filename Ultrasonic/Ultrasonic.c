/*
 * Ultrasonic.c
 *
 *  Created on: Mar 8, 2023
 *      Author: Seif pc
 */

#include "Ultrasonic.h"

extern uint8 Prescaler;

Bool Ultrasonic_init(Ultrasonic_t * USensor)
{
	Bool ret = E_OK;
	if (NULL == USensor)
		ret = E_NOK;
	else {
		GPIO_PIN_CONFIG(&USensor->Trigger_pin);
	}
	return ret;
}

Bool Ultrasonic_send_Trigger(Ultrasonic_t * USensor)
{
	Bool ret=E_OK;
	if(NULL==USensor)
		ret=E_NOK;
	else
	{
		GPIO_PIN_WRITE(&USensor->Trigger_pin,GPIO_HIGH);
		_delay_us(15);
		GPIO_PIN_WRITE(&USensor->Trigger_pin,GPIO_LOW);
	}
	return ret;
}

Bool Ultrasonic_read_distance_Interrupt(uint16 *Distance)
{
	Bool ret=E_OK;
	static int T_rising_edge;
	static uint8 no_edges = 0;
	if (no_edges == Rising_edge)
	{
		T_rising_edge = TCNT1;
		TIMER1_INPUT_CAPTURE_FALLING_EDG();
		TIMER1_INPUT_CAP_INT_FLAG_CLEAR();
		no_edges = 1;
	} else if (no_edges == Falling_edge)
	{
		uint16 T_falling_edge = TCNT1;
		TIMER1_INPUT_CAPTURE_RISING_EDG();
		TIMER1_INPUT_CAP_INT_FLAG_CLEAR();
		uint32 D1 = 340 * (uint32) Prescaler * 50 * (T_falling_edge- T_rising_edge);//max distance=400cm max time in counter=11764us/8=1470 Tick no overflow needed
		*Distance = (uint16) (D1 / (uint32) (F_CPU));
		no_edges = 0;
	}
	return ret;
}
