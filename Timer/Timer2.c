/*
 * Timer2.c
 *
 *  Created on: Mar 10, 2023
 *      Author: Seif pc
 */
#include "Timer2.h"

#if TIMER2_INTERRUPT_COMP_ENABLE==FEATURE_ENABLE
	void (*INT_comp_Callback)(void)=NULL;
#endif
#if TIMER2_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
	void (*INT_ovf_Callback)(void)=NULL;
#endif

static Bool Timer2_mode_set(Timer2_config_t *Timer2);
static Bool Timer2_CTC_Normal_mode(Timer2_config_t *Timer2);
static Bool Timer2_CTC_FastPwm_mode(Timer2_config_t *Timer2);
static Bool Timer2_clk_prescaler(Timer2_config_t *Timer2);
static Bool Timer2_Interrupt_set(Timer2_config_t *Timer2);

Bool Timer2_init(Timer2_config_t *Timer2)
{
	Bool ret=E_OK;
	if(NULL==Timer2)
		ret=E_NOK;
	else
	{
		TIMER2_STOP();
		ret=Timer2_mode_set(Timer2);
		ret=Timer2_Interrupt_set(Timer2);
		ret=Timer2_clk_prescaler(Timer2);
	}
	return ret;
}

static Bool Timer2_Interrupt_set(Timer2_config_t *Timer2)
{
	Bool ret=E_OK;
	if(NULL==Timer2)
		ret=E_NOK;
	else
	{
		#if TIMER2_INTERRUPT_COMP_ENABLE==FEATURE_ENABLE
			TIMER2_TOIF_CLEAR();
			INT_comp_Callback=Timer2->INT_COMP_Callback;
		#endif
		#if TIMER2_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
			TIMER2_OVIF_CLEAR();
			INT_ovf_Callback=Timer2->INT_OVF_Callback;
		#endif
	}
	return ret;
}

static Bool Timer2_mode_set(Timer2_config_t *Timer2)
{
	Bool ret=E_OK;
	switch(Timer2->Mode)
	{
		case TIMER2_NORMAL:
			TIMER2_FOCO_ENABLE();
			CLEAR_BIT(TCCR2,WGM20);
			CLEAR_BIT(TCCR2,WGM21);
			TCNT2=Timer2->Preload_val;
			break;
		case TIMER2_PWM_PHASE_CORRECT:
			TIMER2_FOCO_DISABLE();
			SET_BIT(TCCR2,WGM20);
			CLEAR_BIT(TCCR2,WGM21);
			break;
		case TIMER2_CTC:
			TIMER2_FOCO_ENABLE();
			CLEAR_BIT(TCCR2,WGM20);
			SET_BIT(TCCR2,WGM21);
			SET_BIT(TIMER2_DIR,OC2);
			Timer2_CTC_Normal_mode(Timer2);
			OCR2=Timer2->Preload_val;
			break;
		case TIMER2_FAST_PWM:
			TIMER2_FOCO_DISABLE();
			SET_BIT(TCCR2,WGM20);
			SET_BIT(TCCR2,WGM21);
			SET_BIT(TIMER2_DIR,OC2);
			Timer2_CTC_FastPwm_mode(Timer2);
			Timer2_Setduty(Timer2->Preload_val);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool Timer2_CTC_Normal_mode(Timer2_config_t *Timer0)
{
	Bool ret=E_OK;
	switch(Timer0->CTC_NON_PWM_mode)
	{
		case TIMER2_CTC_NORMAL_nonPwm:
			CLEAR_BIT(TCCR2,COM20);
			CLEAR_BIT(TCCR2,COM21);
			break;
		case TIMER2_CTC_SET_TC2_nonPwm:
			SET_BIT(TCCR2,COM20);
			CLEAR_BIT(TCCR2,COM21);
			break;
		case TIMER2_CTC_TOGGLE_TC2_nonPwm:
			CLEAR_BIT(TCCR2,COM20);
			SET_BIT(TCCR2,COM21);
			break;
		case TIMER2_CTC_CLEAR_TC2_nonPwm:
			SET_BIT(TCCR2,COM20);
			SET_BIT(TCCR2,COM21);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool Timer2_CTC_FastPwm_mode(Timer2_config_t *Timer2)
{
	Bool ret=E_OK;
	switch(Timer2->CTC_FAST_PWM_Mode)
	{
		case TIMER2_CTC_NORMAL:
			CLEAR_BIT(TCCR2,COM20);
			CLEAR_BIT(TCCR2,COM21);
			break;
		case TIMER2_CTC_RESERVED:
			ret=E_OK;
			break;
		case TIMER2_CTC_CLEAR_TC2:
			CLEAR_BIT(TCCR2,COM20);
			SET_BIT(TCCR2,COM21);
			break;
		case TIMER2_CTC_SET_TC2:
			SET_BIT(TCCR2,COM20);
			SET_BIT(TCCR2,COM21);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool Timer2_clk_prescaler(Timer2_config_t *Timer2)
{
	Bool ret=E_OK;
	switch(Timer2->Prescaler)
	{
		case TIMER2_FOSC_:
			SET_BIT(TCCR2,CS20);
			CLEAR_BIT(TCCR2,CS21);
			CLEAR_BIT(TCCR2,CS22);
			break;
		case TIMER2_FOSC_8:
			CLEAR_BIT(TCCR2,CS00);
			SET_BIT(TCCR2,CS21);
			CLEAR_BIT(TCCR2,CS22);
			break;
		case TIMER2_FOSC_64:
			SET_BIT(TCCR2,CS20);
			SET_BIT(TCCR2,CS21);
			CLEAR_BIT(TCCR2,CS22);
			break;
		case TIMER2_FOSC_256:
			CLEAR_BIT(TCCR2,CS20);
			CLEAR_BIT(TCCR2,CS21);
			SET_BIT(TCCR2,CS22);
			break;
		case TIMER2_FOSC_1024:
			SET_BIT(TCCR2,CS20);
			CLEAR_BIT(TCCR2,CS21);
			SET_BIT(TCCR2,CS22);
			break;
		case TIMER2_T2_COUNTER_MODE_FEDG:
			CLEAR_BIT(TCCR2,CS20);
			SET_BIT(TCCR2,CS21);
			SET_BIT(TCCR2,CS22);
			break;
		case TIMER2_T2_COUNTER_MODE_REDG:
			SET_BIT(TCCR2,CS20);
			SET_BIT(TCCR2,CS21);
			SET_BIT(TCCR2,CS22);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

Bool Timer2_Setduty(uint8 Duty)
{
	Bool ret=E_OK;
	OCR2=(uint8)((Duty/(float)100)*256);
	return ret;
}

Bool Timer2_set_Val(uint8 Val)
{
	Bool ret=E_OK;
	TCNT2=Val;
	return ret;
}

Bool Timer2_get_Val(uint8* Val)
{
	Bool ret = E_OK;
	if (NULL==Val)
		ret = E_NOK;
	else
	{
		*Val=TCNT2;
	}
	return ret;
}

Bool Timer2_delay_polling(Timer2_config_t *Timer2)
{
	Bool ret=E_OK;
	if (NULL==Timer2)
			ret = E_NOK;
	else
	{
		uint8 Prescaler=TCCR2&=0x07;
		TIMER2_STOP();
		Timer2_set_Val(Timer2->Preload_val);
		TCCR2|=Prescaler;
		while(!(READ_BIT(TIFR,TOV2)));
		TIMER2_OVIF_CLEAR();
	}
	return ret;
}

