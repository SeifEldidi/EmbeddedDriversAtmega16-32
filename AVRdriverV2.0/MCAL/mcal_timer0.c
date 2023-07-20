/*
 * macl_timer1.c
 *
 *  Created on: May 28, 2023
 *      Author: Seif pc
 */

#include "mcal_timer0.h"

#if Timer0_OVF_Int==TRUE
	static void (*TIMER0_OVF_callback)(void)=NULL;
#endif
#if Timer0_CTC_Int==TRUE
	static void (*TIMER0_CTC_callback)(void)=NULL;
#endif

static void  Timer0_wfgm_setting(Timer0_config *Timer0);

uint8 Mcal_Timer0_init(Timer0_config *Timer0)
{
	uint8 Err_state=E_OK;
	if(NULL==Timer0)
		Err_state=E_NOK;
	else
	{
		TIMER0_STOP();
		TIMER0_T0_OVF_Flag_Clear();
		TIMER0_T0_CTC_Flag_Clear();
		Timer0_wfgm_setting(Timer0);
		if(Timer0->WFGM_mode==WFGM_PWM_T0_PHASE_CORRECT||Timer0->WFGM_mode==WFGM_T0_FAST_PWM)
			Mcal_TIMER0_set_duty(Timer0->T0_duty_cycle);
		else
			TCNT0=Timer0->T0_TCNT0_init_val;
#if Timer0_OVF_Int==TRUE
		sei();
		TIMER0_OVF_callback=Timer0->TIMER0_ovf_callback;
		TIMER0_T0_OVF_INT_Enable();
#endif
#if Timer0_CTC_Int==TRUE
		sei();
		TIMER0_CTC_callback=Timer0->TIMER0_ctc_callback;
		TIMER0_T0_CTC_INT_Enable();
#endif
		TCCR0|=Timer0->Prescaler_Counter_setting;
	}
	return Err_state;
}

void	  Mcal_TIMER0_set_duty(uint8 Duty_cycle)
{
	OCR0=((Duty_cycle*255)/100);
}

void  Mcal_Timer0_delay_polling(uint8 TCNT0_val)
{
	uint8 Prescaler;
	Prescaler=TCCR0;
	TIMER0_STOP();
	TCNT0=TCNT0_val;
	TCCR0=Prescaler;
	while(!READ_BIT(TIFR,TOV0));
	TIMER0_T0_OVF_Flag_Clear();
}

static void  Timer0_wfgm_setting(Timer0_config *Timer0)
{
	switch(Timer0->WFGM_mode)
	{
		case WFGM_T0_NORMAL:
			CLEAR_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
			break;
		case WFGM_PWM_T0_PHASE_CORRECT:
			SET_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
			TIMER0_DDR|=(1<<OCO);
			switch(Timer0->Phase_PWM_setting)
			{
				case PHASE_CORRECT_PWM_T0_Normal:
					CLEAR_BIT(TCCR0,COM00);
					CLEAR_BIT(TCCR0,COM01);
					break;
				case PHASE_CORRECT_PWM_T0_NonInverting:
					CLEAR_BIT(TCCR0, COM00);
					SET_BIT(TCCR0, COM01);
					break;
				case PHASE_CORRECT_PWM_T0_Inverting:
					SET_BIT(TCCR0, COM00);
					SET_BIT(TCCR0, COM01);
					break;
				default:
					break;
			}
			break;
		case WFGM_CTC_T0:
			CLEAR_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
		switch (Timer0->Fast_PWM_setting) {
			case CTCT_T0_NORMAL_PORT:
				CLEAR_BIT(TCCR0, COM00);
				CLEAR_BIT(TCCR0, COM01);
				break;
			case CTC_T0_TOGGLE_OC0:
				SET_BIT(TCCR0, COM00);
				CLEAR_BIT(TCCR0, COM01);
				break;
			case CTC_T0_CLEAR_OC0:
				CLEAR_BIT(TCCR0, COM00);
				SET_BIT(TCCR0, COM01);
				break;
			case CTCT_T0_SET_OC0:
				SET_BIT(TCCR0, COM00);
				SET_BIT(TCCR0, COM01);
				break;
			default:
				break;
			}
			break;
		case WFGM_T0_FAST_PWM:
			SET_BIT(TCCR0, WGM00);
			SET_BIT(TCCR0, WGM01);
			TIMER0_DDR|=(1<<OCO);
		switch (Timer0->Fast_PWM_setting) {
			case FAST_PWM_T0_Normal:
				CLEAR_BIT(TCCR0, COM00);
				CLEAR_BIT(TCCR0, COM01);
				break;
			case FAST_PWM_T0_NonInverting:
				CLEAR_BIT(TCCR0, COM00);
				SET_BIT(TCCR0, COM01);
				break;
			case FAST_PWM_T0_Inverting:
				SET_BIT(TCCR0, COM00);
				SET_BIT(TCCR0, COM01);
				break;
			default:
				break;
			}
			break;
	}
}
