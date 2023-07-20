/*
 * Hal_Ultrasonic.c
 *
 *  Created on: May 30, 2023
 *      Author: Seif pc
 */
#include "Ecual_Ultrasonic.h"

volatile uint32 OV_count;

void TIMER_ISR_internal()
{
	OV_count++;
}

void Ecual_Ultrasonic_Init(Ultrasonic_t *Ut)
{
	Mcal_GPIO_INIT(&Ut->Echo);
	Mcal_GPIO_INIT(&Ut->Trigger);
}

static void Ultrasonic_send_pulse(Ultrasonic_t *Ut)
{
	Mcal_GPIO_WRITE(&Ut->Trigger, GPIO_HIGH);
	_delay_us(15);
	Mcal_GPIO_WRITE(&Ut->Trigger, GPIO_LOW);
}

uint8 Ecual_Ultrasonic_Read(Ultrasonic_t *Ut,uint16 *Result)
{
	uint32 Timer_start=0;
	uint32 Timer_end=0;
	uint32 Timer=0;
	uint16 distance=0;
	uint32 OVF_old=0;
	GPIO_state State_read=GPIO_LOW;
	Timer0_config Timer_t;
	uint8 Timeout=E_OK;
	/**************Timer Mode********/
	TIMER0_RESET();
	Timer_t.WFGM_mode=WFGM_T0_NORMAL;
	Timer_t.T0_TCNT0_init_val=0;
	Timer_t.Prescaler_Counter_setting=TIMER0_PRESCALER_8; //if 16 Mhz then input Clock= 2Mhz hence Ttick=0.5us hence Timer max=127.5us
	Timer_t.TIMER0_ovf_callback=TIMER_ISR_internal;
	Mcal_Timer0_init(&Timer_t);
	/*********Ultrasonic Burst***********/
	Ultrasonic_send_pulse(Ut);
	/**********Read Data************/
	Mcal_GPIO_READ(&Ut->Echo,&State_read);
	TIMER0_Write(0);
	OVF_old=OV_count;
	while(State_read == GPIO_LOW)
	{
		Mcal_GPIO_READ(&Ut->Echo,&State_read);
		Timer = ((TCNT0+ (uint32) (OV_count-OVF_old) * 256 )/2)/2;
		if(Timer>Ultrasonic_TimeOut)
		{
			Timeout=E_NOK;
			break;
		}
	}
	if(Timeout==E_OK)
	{
		Timer_start=TCNT0;
		OVF_old=OV_count;
		while (State_read == GPIO_HIGH)
		{
			Mcal_GPIO_READ(&Ut->Echo, &State_read);
			if(OV_count>OVF_MAX+OVF_old)
				break;
		}
		Mcal_GPIO_READ(&Ut->Echo, &State_read);
		Timer_end = TCNT0;
		Timer = ((Timer_end -Timer_start+ (uint32) (OV_count-OVF_old) * 256 )/2)/2;
		if(Timer>Ultrasonic_max)
			Timer=Ultrasonic_max;
		distance=((340*Timer)/((uint32)10000));
		*Result=distance;
	}
	return Timeout;
}
