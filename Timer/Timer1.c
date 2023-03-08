/*
 * Timer1.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Seif pc
 */
#include "Timer1.h"

static uint16 Glob;
uint8 Prescaler;
#if TIMER1_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
static void(*TIMER1_OVF_callback)(void)=NULL;
#endif
#if TIMER1_INTERRUPT_CMPA_ENABLE==FEATURE_ENABLE
static void(*TIMER1_CMPA_callback)(void)=NULL;
#endif
#if TIMER1_INTERRUPT_CMPB_ENABLE==FEATURE_ENABLE
static void(*TIMER1_CMPB_callback)(void)=NULL;
#endif
#if TIMER1_INTERRUPT_ICP_ENABLE==FEATURE_ENABLE
static void(*TIMER1_ICP_callback)(void)=NULL;
#endif

static Bool TIMER1_int_enable(TIMER1_CONFIG_t* Timer1)
{
	Bool ret=E_OK;
	#if TIMER1_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
		GLOBAL_INT_ENABLE();
		TIMER1_OVERFLOW_FLAG_CLEAR();
		TIMER1_OVERFLOW_INT_ENABLE();
		TIMER1_OVF_callback=Timer1->TIMER1_OVERFLOW_CALLBACK;
	#endif
	#if TIMER1_INTERRUPT_CMPA_ENABLE==FEATURE_ENABLE
		GLOBAL_INT_ENABLE();
		TIMER1_OUTPUT_CMP_OC1A_FLAG_CLEAR();
		TIMER1_OUTPUT_CMP_OC1A_INT_ENABLE();
		TIMER1_CMPA_callback=Timer1->TIMER1_OUTPUT_CMP_OC1A_CALLBACK;
	#endif
	#if TIMER1_INTERRUPT_CMPB_ENABLE==FEATURE_ENABLE
		GLOBAL_INT_ENABLE();
		TIMER1_OUTPUT_CMP_OC1B_FLAG_CLEAR();
		TIMER1_OUTPUT_CMP_OC1B_INT_ENABLE();
		TIMER1_CMPB_callback=Timer1->TIMER1_OUTPUT_CMP_OC1B_CALLBACK;
	#endif
	#if TIMER1_INTERRUPT_ICP_ENABLE==FEATURE_ENABLE
		GLOBAL_INT_ENABLE();
		TIMER1_OVERFLOW_FLAG_CLEAR();
		TIMER1_INPUT_CAP_INT_ENABLE();
		TIMER1_ICP_callback=Timer1->TIMER1_INPUT_CAP_CALLBACK;
	#endif
	return ret;
}

static Bool TIMER1_wfmg_init(TIMER1_CONFIG_t* Timer1)
{
	Bool ret=E_OK;
	switch(Timer1->WVFM_GEN)
	{
		case TIMER1_Normal_T1:
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			TIMER1_SET_VAL(Timer1,Timer1->Preload_val);
			break;
		case TIMER1_PWM_Phase_Correct_8_bit:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Phase_Correct_9_bit:
			TIMER1_PWM_MODE();
			CLEAR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Phase_Correct_10_bit:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_CTC_OCR1:
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Fast_8_bit:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Fast_9_bit:
			TIMER1_PWM_MODE();
			CLEAR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Fast_10_bit:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Phase_and_Frequency_Correct_ICR1:
			TIMER1_PWM_MODE();
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Phase_and_Frequency_Correct_OCR1:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Phase_Correct_ICR1:
			TIMER1_PWM_MODE();
			CLEAR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_Phase_Correct_OCR1:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_CTC_ICR1:
			CLEAR_BIT(TCCR1A, WGM10);
			CLEAR_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1B, WGM12);
			SET_BIT(TCCR1B, WGM13);
			break;
		case TIMER1_PWM_FAST_ICR1:
			TIMER1_PWM_MODE();
			CLEAR_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		case TIMER1_PWM_FAST_OCR1:
			TIMER1_PWM_MODE();
			SET_BIT(TCCR1A,WGM10);
			SET_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1B,WGM13);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool TIMER1_COMP_reg_init(TIMER1_CONFIG_t* Timer1)
{
	Bool ret=E_OK;
	switch(Timer1->TIMER1_mode)
	{
		case TIMER1_Normal_non_pwm_mode:
		case TIMER1_Normal_Fast_PWM_Mode:
		case TIMER1_Normal_Phase_Correct_PWM:
			switch(Timer1->REG_active)
			{
				case TIMER1_COMPA_ACTIVE:
					CLEAR_BIT(TCCR1A, COM1A1);
					CLEAR_BIT(TCCR1A, COM1A0);
					break;
				case TIMER1_COMPB_ACTIVE:
					CLEAR_BIT(TCCR1A, COM1B1);
					CLEAR_BIT(TCCR1A, COM1B0);
					break;
				case TIMER1_COMPA_COMPB_ACTIVE:
					CLEAR_BIT(TCCR1A, COM1A1);
					CLEAR_BIT(TCCR1A, COM1A0);
					CLEAR_BIT(TCCR1A, COM1B1);
					CLEAR_BIT(TCCR1A, COM1B0);
					break;
				default:
					ret=E_NOK;
			}
			break;
		case TIMER1_ToggleOC_non_pwm_mode:
		case TIMER1_Normal_ReservedToggle_Fast_PWM_Mode:
		case TIMER1_Toggle_Phase_Correct_PWM:
			switch (Timer1->REG_active)
			{
				case TIMER1_COMPA_ACTIVE:
					CLEAR_BIT(TCCR1A, COM1A1);
					SET_BIT(TCCR1A, COM1A0);
					SET_BIT(TIMER1_PORT_DIR, OC1A);
					break;
				case TIMER1_COMPB_ACTIVE:
					CLEAR_BIT(TCCR1A, COM1B1);
					SET_BIT(TCCR1A, COM1B0);
					SET_BIT(TIMER1_PORT_DIR, OC1B);
					break;
				case TIMER1_COMPA_COMPB_ACTIVE:
					CLEAR_BIT(TCCR1A, COM1A1);
					SET_BIT(TCCR1A, COM1A0);
					CLEAR_BIT(TCCR1A, COM1B1);
					SET_BIT(TCCR1A, COM1B0);
					SET_BIT(TIMER1_PORT_DIR, OC1A);
					SET_BIT(TIMER1_PORT_DIR, OC1B);
					break;
				default:
					ret = E_NOK;
			}
			break;
		case TIMER1_ClearOC_non_pwm_mode:
		case TIMER1_ClearOC_Fast_PWM_Mode:
		case TIMER1_ClearOC_Phase_Correct_PWM:
			switch (Timer1->REG_active)
			{
				case TIMER1_COMPA_ACTIVE:
					SET_BIT(TCCR1A, COM1A1);
					CLEAR_BIT(TCCR1A, COM1A0);
					SET_BIT(TIMER1_PORT_DIR, OC1A);
					break;
				case TIMER1_COMPB_ACTIVE:
					SET_BIT(TCCR1A, COM1B1);
					CLEAR_BIT(TCCR1A, COM1B0);
					SET_BIT(TIMER1_PORT_DIR, OC1B);
					break;
				case TIMER1_COMPA_COMPB_ACTIVE:
					SET_BIT(TCCR1A, COM1A1);
					CLEAR_BIT(TCCR1A, COM1A0);
					SET_BIT(TCCR1A, COM1B1);
					CLEAR_BIT(TCCR1A, COM1B0);
					SET_BIT(TIMER1_PORT_DIR, OC1A);
					SET_BIT(TIMER1_PORT_DIR, OC1B);
					break;
				default:
					ret = E_NOK;
			}
			break;
		case TIMER1_setOC_Fast_PWM_Mode:
		case TIMER1_SetOC_non_pwm_mode:
		case TIMER1_setOC_Phase_Correct_PWM:
			switch (Timer1->REG_active)
			{
				case TIMER1_COMPA_ACTIVE:
					SET_BIT(TCCR1A, COM1A1);
					SET_BIT(TCCR1A, COM1A0);
					SET_BIT(TIMER1_PORT_DIR, OC1A);
					break;
				case TIMER1_COMPB_ACTIVE:
					SET_BIT(TCCR1A, COM1B1);
					SET_BIT(TCCR1A, COM1B0);
					SET_BIT(TIMER1_PORT_DIR, OC1B);
					break;
				case TIMER1_COMPA_COMPB_ACTIVE:
					SET_BIT(TCCR1A, COM1A1);
					SET_BIT(TCCR1A, COM1A0);
					SET_BIT(TCCR1A, COM1B1);
					SET_BIT(TCCR1A, COM1B0);
					SET_BIT(TIMER1_PORT_DIR, OC1A);
					SET_BIT(TIMER1_PORT_DIR, OC1B);
					break;
				default:
					ret = E_NOK;
			}
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool TIMER1_CS_set(TIMER1_CONFIG_t* Timer1)
{
	Bool ret=E_OK;
	switch(Timer1->Prescaler)
	{
		  case TIMER_FOSC_1:
			TCCR1B|=(1<<CS10);
			Prescaler=1;
			break;
		  case TIMER_FOSC_8:
			TCCR1B|=(1<<CS11);
			Prescaler=8;
			break;
		 case TIMER_FOSC_64:
			TCCR1B|=(1<<CS11|1<<CS10);
			Prescaler=64;
			break;
		 case TIMER_FOSC_256:
			TCCR1B|=(1<<CS12);
			Prescaler=256;
			break;
		 case TIMER_FOSC_1024:
			TCCR1B|=(1<<CS12|1<<CS10);
			Prescaler=1024;
			break;
		 case EXTERNAL_SOURCE_COUNTER_T1_FALLING_EDG:
			TCCR1B|=(1<<CS12|1<<CS11);
			break;
		 case EXTERNAL_SOURCE_COUNTER_T1_RISING_EDG:
			TCCR1B|=(1<<CS12|1<<CS11|1<<CS10);
			break;
		default:
			ret=E_NOK;
			break;
	}
	return ret;
}

Bool TIMER1_INIT(TIMER1_CONFIG_t *Timer1)
{
	Bool ret=E_OK;
	if(NULL==Timer1)
		ret=E_NOK;
	else
	{
		Glob=Timer1->Preload_val;
		TIMER1_STOP();
		ret|=TIMER1_wfmg_init(Timer1);
		ret|=TIMER1_COMP_reg_init(Timer1);
		ret|=TIMER1_int_enable(Timer1);
		ret|=TIMER1_SET_Freq(Timer1,Timer1->PWM_freq);
		ret|=TIMER1_SET_DUTY(Timer1,Timer1->Duty_Cycle_1,Timer1->Duty_Cycle_2);
		switch(Timer1->Input_capture_edg)
		{
			case TIMER_ICP1_REDG:
				TIMER1_INPUT_CAPTURE_RISING_EDG();
				break;
			case TIMER_ICP1_FEDG:
				TIMER1_INPUT_CAPTURE_FALLING_EDG();
				break;
		}
		ret|=TIMER1_CS_set(Timer1);
	}
	return ret;
}

Bool TIMER1_SET_DUTY(TIMER1_CONFIG_t *Timer1,uint8 duty,uint8 duty_1)
{
	Bool ret = E_OK;
	if (NULL == Timer1)
		ret = E_NOK;
	else {
		switch (Timer1->WVFM_GEN)
		{
			case TIMER1_PWM_FAST_ICR1:
				switch(Timer1->REG_active)
				{
					case TIMER1_COMPA_ACTIVE:
						OCR1A=(duty/(float)100)*ICR1;
						break;
					case TIMER1_COMPB_ACTIVE:
						OCR1B=(duty_1/(float)100)*ICR1;
						break;
					case TIMER1_COMPA_COMPB_ACTIVE:
						OCR1A=(duty/(float)100)*ICR1;
						OCR1B=(duty_1/(float)100)*ICR1;
						break;
				}
				break;
			case TIMER1_PWM_FAST_OCR1:
				OCR1B=(duty/(float)100)*OCR1A;
				break;
			case TIMER1_PWM_Fast_8_bit:
				switch (Timer1->REG_active)
				{
					case TIMER1_COMPA_ACTIVE:
						OCR1A = (duty / (float) 100) * PWM_8BITS_MAX;
						break;
					case TIMER1_COMPB_ACTIVE:
						OCR1B = (duty_1 / (float) 100) * PWM_8BITS_MAX;
						break;
					case TIMER1_COMPA_COMPB_ACTIVE:
						OCR1A = (duty / (float) 100) * PWM_8BITS_MAX;
						OCR1B = (duty_1 / (float) 100) * PWM_8BITS_MAX;
						break;
				}
				break;
			case TIMER1_PWM_Fast_9_bit:
				switch (Timer1->REG_active)
				{
					case TIMER1_COMPA_ACTIVE:
						OCR1A = (duty / (float) 100) * PWM_9BITS_MAX;
						break;
					case TIMER1_COMPB_ACTIVE:
						OCR1B = (duty_1 / (float) 100) * PWM_9BITS_MAX;
						break;
					case TIMER1_COMPA_COMPB_ACTIVE:
						OCR1A = (duty / (float) 100) * PWM_9BITS_MAX;
						OCR1B = (duty_1 / (float) 100) * PWM_9BITS_MAX;
						break;
				}
				break;
			case TIMER1_PWM_Fast_10_bit:
				switch (Timer1->REG_active)
				{
					case TIMER1_COMPA_ACTIVE:
						OCR1A = (duty / (float) 100) * PWM_10BITS_MAX;
						break;
					case TIMER1_COMPB_ACTIVE:
						OCR1B = (duty_1 / (float) 100) * PWM_10BITS_MAX;
						break;
					case TIMER1_COMPA_COMPB_ACTIVE:
						OCR1A = (duty / (float) 100) * PWM_10BITS_MAX;
						OCR1B = (duty_1 / (float) 100) * PWM_10BITS_MAX;
						break;
				}
				break;
			default:
				ret = E_NOK;
		}
	}
	return ret;
}

Bool TIMER1_SET_Freq(TIMER1_CONFIG_t *Timer1,uint32 Freq)
{
	Bool ret = E_OK;
	uint16 TOP;
	if (NULL == Timer1)
		ret = E_NOK;
	else {
		switch(Timer1->WVFM_GEN)
		{
			case TIMER1_PWM_FAST_ICR1:
				/*
				 * Frequency=Fosc/(N.(1+TOP))
				 * TOP=(Fosc/Frequency.N)-1
				 */
				 switch(Timer1->Prescaler)
				 {
					case TIMER_FOSC_1:
						TOP=(F_CPU/(float)Freq)-1;
						ICR1 =TOP;
						break;
					case TIMER_FOSC_8:
						TOP=(F_CPU/((float)Freq*8))-1;
						ICR1 =TOP;
						break;
					case TIMER_FOSC_64:
						TOP=(F_CPU/((float)Freq*64))-1;
						ICR1 =TOP;
						break;
					case TIMER_FOSC_256:
						TOP=(F_CPU/((float)Freq*256))-1;
						ICR1 =TOP;
						break;
					case TIMER_FOSC_1024:
						TOP=(F_CPU/((float)Freq*1024))-1;
						ICR1 =TOP;
						break;
					default:
						ret=E_NOK;
				 }
				break;
			case TIMER1_PWM_FAST_OCR1:
			switch (Timer1->Prescaler)
			{
				case TIMER_FOSC_1:
					TOP = (F_CPU /(float) Freq) - 1;
					OCR1A  = TOP;
					break;
				case TIMER_FOSC_8:
					TOP = (F_CPU / ((float)Freq * 8)) - 1;
					OCR1A  = TOP;
					break;
				case TIMER_FOSC_64:
					TOP = (F_CPU / ((float)Freq* 64)) - 1;
					OCR1A  = TOP;
					break;
				case TIMER_FOSC_256:
					TOP = (F_CPU / ((float)Freq * 256)) - 1;
					OCR1A  = TOP;
					break;
				case TIMER_FOSC_1024:
					TOP = (F_CPU / ((float)Freq * 1024)) - 1;
					OCR1A  = TOP;
					break;
				default:
					ret = E_NOK;
			}
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

Bool TIMER1_SET_VAL(TIMER1_CONFIG_t *Timer1,uint16 VAL)
{
	Bool ret = E_OK;
	if (NULL == Timer1)
		ret = E_NOK;
	else {
		TCNT1L=(uint8)VAL;
		TCNT1H=(uint8)(VAL>>8);
	}
	return ret;
}

Bool TIMER1_GET_VAL(TIMER1_CONFIG_t *Timer1,uint16 *VAL)
{
	Bool ret = E_OK;
	if (NULL == Timer1||NULL==VAL)
		ret = E_NOK;
	else {
		*VAL= (uint16)(TCNT1L+ TCNT1H*256);
	}
	return ret;
}

Bool TIMER1_delay_polling(TIMER1_CONFIG_t *Timer1,uint16 VAL)
{
	Bool ret = E_OK;
	if (NULL == Timer1 )
		ret = E_NOK;
	else {
		uint8 Prescaler_Temp=TCCR1B;
		TIMER1_STOP();
		TIMER1_SET_VAL(Timer1,VAL);
		TCCR1B=Prescaler_Temp;
		while(BIT_IS_CLEAR(TIFR,TOV1));
		TIMER1_OVERFLOW_FLAG_CLEAR();
	}
	return ret;
}

void Timer1_ovf_ISR()
{
	TIMER1_OVERFLOW_FLAG_CLEAR();
	TCNT1L=(uint8)Glob;
	TCNT1H=(uint8)(Glob>>8);
	if (TIMER1_OVF_callback)
		TIMER1_OVF_callback();
	else {
	}
}

void Timer1_capt_ISR()
{
#if TIMER1_INTERRUPT_ICP_ENABLE==FEATURE_ENABLE
	TIMER1_INPUT_CAP_INT_FLAG_CLEAR();
	if (TIMER1_ICP_callback)
		TIMER1_ICP_callback();
	else {
	}
#endif
}


