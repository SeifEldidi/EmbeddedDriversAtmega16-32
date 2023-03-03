/*
 * Timer0.h
 *
 *  Created on: Mar 2, 2023
 *      Author: Seif pc
 */

#ifndef TIMER0_H_
#define TIMER0_H_
/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define TIMER0_TIMER_MODE 0
#define TIMER0_COUNTER_MODE 1
/***********Macro Like Function**************/
#define TIMER0_FOCO_ENABLE()   (SET_BIT(TCCR0,FOC0))
#define TIMER0_FOCO_DISABLE()  (CLEAR_BIT(TCCR0,FOC0))

#define TIMER0_STOP() (TCCR0&=0xF8)

#if TIMER0_INTERRUPT_ENABLE==FEATURE_ENABLE

#define TIMER0_OVIE_ENABLE()   (SET_BIT(TIMSK,OCIE0))
#define TIMER0_OVIE_DISABLE()  (CLEAR_BIT(TIMSK,OCIE0))

#define TIMER0_TOEI_ENABLE() (SET_BIT(TIMSK,TOIE0))
#define TIMER0_TOEI_DISABLE() (CLEAR_BIT(TIMSK,TOIE0))

#define TIMER0_OVIF_CLEAR() (SET_BIT(TIFR,OCF0))
#define TIMER0_TOIF_CLEAR() (SET_BIT(TIFR,TOV0))

#endif


/*************Defined Data types**************/
typedef enum
{
	TIMER0_NORMAL,
	TIMER0_PWM_PHASE_CORRECT,
	TIMER0_CTC,
	TIMER0_FAST_PWM,
}TIMER0_modes;

typedef enum
{
	TIMER0_CTC_NORMAL_nonPwm,
	TIMER0_CTC_SET_TC0_nonPwm,
	TIMER0_CTC_TOGGLE_TC0_nonPwm,
	TIMER0_CTC_CLEAR_TC0_nonPwm,
}TIMER0_CTC_NON_PMW_MODES;

typedef enum
{
	TIMER0_CTC_NORMAL,
	TIMER0_CTC_RESERVED,
	TIMER0_CTC_CLEAR_TC0,
	TIMER0_CTC_SET_TC0,
}TIMER0_CTC_FAST_PMW_MODES;

typedef enum
{
	TIMER0_FOSC_,
	TIMER0_FOSC_8,
	TIMER0_FOSC_64,
	TIMER0_FOSC_256,
	TIMER0_FOSC_1024,
	TIMER0_T0_COUNTER_MODE_FEDG,
	TIMER0_T0_COUNTER_MODE_REDG,
}TIMER0_PRESCALERS;

typedef struct
{
	TIMER0_modes Mode;
	TIMER0_CTC_NON_PMW_MODES CTC_NON_PWM_mode;
	TIMER0_CTC_FAST_PMW_MODES CTC_FAST_PWM_Mode;
	TIMER0_PRESCALERS Prescaler;
	uint8 Preload_val;
}Timer0_config_t;


/*********Software Interfaces*******************/
Bool Timer0_init(Timer0_config_t *Timer0);
Bool Timer0_Setduty(uint8 Duty);


#endif /* TIMER0_H_ */
