/*
 * Timer1.c
 *
 *  Created on: Mar 2, 2023
 *      Author: Seif pc
 */
#include "Timer0.h"

static Bool Timer0_mode_set(Timer0_config_t *Timer0);
static Bool Timer0_CTC_Normal_mode(Timer0_config_t *Timer0);
static Bool Timer0_CTC_FastPwm_mode(Timer0_config_t *Timer0);
static Bool Timer0_clk_prescaler(Timer0_config_t *Timer0);

Bool Timer0_init(Timer0_config_t *Timer0)
{
	Bool ret=E_OK;
	if(NULL==Timer0)
		ret=E_NOK;
	else
	{
		TIMER0_STOP();
		ret=Timer0_mode_set(Timer0);
#if TIMER0_INTERRUPT_ENABLE==FEATURE_ENABLE
#endif
		ret=Timer0_clk_prescaler(Timer0);
	}
	return ret;
}

static Bool Timer0_mode_set(Timer0_config_t *Timer0)
{
	Bool ret=E_OK;
	switch(Timer0->Mode)
	{
		case TIMER0_NORMAL:
			TIMER0_FOCO_ENABLE();
			CLEAR_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
			TCNT0=Timer0->Preload_val;
			break;
		case TIMER0_PWM_PHASE_CORRECT:
			TIMER0_FOCO_DISABLE();
			SET_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
			break;
		case TIMER0_CTC:
			TIMER0_FOCO_ENABLE();
			CLEAR_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
			Timer0_CTC_Normal_mode(Timer0);
			OCR0=Timer0->Preload_val;
			break;
		case TIMER0_FAST_PWM:
			TIMER0_FOCO_DISABLE();
			SET_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
			SET_BIT(DDRB,PB3);
			Timer0_CTC_FastPwm_mode(Timer0);
			OCR0=Timer0->Preload_val;
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool Timer0_CTC_Normal_mode(Timer0_config_t *Timer0)
{
	Bool ret=E_OK;
	switch(Timer0->CTC_NON_PWM_mode)
	{
		case TIMER0_CTC_NORMAL_nonPwm:
			CLEAR_BIT(TCCR0,COM00);
			CLEAR_BIT(TCCR0,COM01);
			break;
		case TIMER0_CTC_SET_TC0_nonPwm:
			SET_BIT(TCCR0,COM00);
			CLEAR_BIT(TCCR0,COM01);
			break;
		case TIMER0_CTC_TOGGLE_TC0_nonPwm:
			CLEAR_BIT(TCCR0,COM00);
			SET_BIT(TCCR0,COM01);
			break;
		case TIMER0_CTC_CLEAR_TC0_nonPwm:
			SET_BIT(TCCR0,COM00);
			SET_BIT(TCCR0,COM01);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool Timer0_CTC_FastPwm_mode(Timer0_config_t *Timer0)
{
	Bool ret=E_OK;
	switch(Timer0->CTC_FAST_PWM_Mode)
	{
		case TIMER0_CTC_NORMAL:
			CLEAR_BIT(TCCR0,COM00);
			CLEAR_BIT(TCCR0,COM01);
			break;
		case TIMER0_CTC_RESERVED:
			ret=E_OK;
			break;
		case TIMER0_CTC_CLEAR_TC0:
			CLEAR_BIT(TCCR0,COM00);
			SET_BIT(TCCR0,COM01);
			break;
		case TIMER0_CTC_SET_TC0:
			SET_BIT(TCCR0,COM00);
			SET_BIT(TCCR0,COM01);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool Timer0_clk_prescaler(Timer0_config_t *Timer0)
{
	Bool ret=E_OK;
	switch(Timer0->Prescaler)
	{
		case TIMER0_FOSC_:
			SET_BIT(TCCR0,CS00);
			CLEAR_BIT(TCCR0,CS01);
			CLEAR_BIT(TCCR0,CS02);
			break;
		case TIMER0_FOSC_8:
			CLEAR_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS01);
			CLEAR_BIT(TCCR0,CS02);
			break;
		case TIMER0_FOSC_64:
			SET_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS01);
			CLEAR_BIT(TCCR0,CS02);
			break;
		case TIMER0_FOSC_256:
			CLEAR_BIT(TCCR0,CS00);
			CLEAR_BIT(TCCR0,CS01);
			SET_BIT(TCCR0,CS02);
			break;
		case TIMER0_FOSC_1024:
			SET_BIT(TCCR0,CS00);
			CLEAR_BIT(TCCR0,CS01);
			SET_BIT(TCCR0,CS02);
			break;
		case TIMER0_T0_COUNTER_MODE_FEDG:
			CLEAR_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS01);
			SET_BIT(TCCR0,CS02);
			break;
		case TIMER0_T0_COUNTER_MODE_REDG:
			SET_BIT(TCCR0,CS00);
			SET_BIT(TCCR0,CS01);
			SET_BIT(TCCR0,CS02);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

Bool Timer0_Setduty(uint8 Duty)
{
	Bool ret=E_OK;
	OCR0=(uint8)((Duty/(float)100)*256);
	return ret;
}
