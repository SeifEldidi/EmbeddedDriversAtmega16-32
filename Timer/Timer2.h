/*
 * Timer2.h
 *
 *  Created on: Mar 10, 2023
 *      Author: Seif pc
 */

#ifndef TIMER2_H_
#define TIMER2_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define TIMER2_TIMER_MODE 0
#define TIMER2_COUNTER_MODE 1
#define OC2 PD7
#define TIMER2_DIR DDRD
/***********Macro Like Function**************/
#define TIMER2_FOCO_ENABLE()   (SET_BIT(TCCR2,FOC2))
#define TIMER2_FOCO_DISABLE()  (CLEAR_BIT(TCCR2,FOC2))

#define TIMER2_OVIF_CLEAR() (SET_BIT(TIFR,OCF2))
#define TIMER2_TOIF_CLEAR() (SET_BIT(TIFR,TOV2))

#define TIMER2_STOP() (TCCR2&=0xF8)

#if TIMER2_INTERRUPT_ENABLE==FEATURE_ENABLE

#define TIMER2_OVIE_ENABLE()   (SET_BIT(TIMSK,OCIE2))
#define TIMER2_OVIE_DISABLE()  (CLEAR_BIT(TIMSK,OCIE2))

#define TIMER2_TOEI_ENABLE() (SET_BIT(TIMSK,TOIE2))
#define TIMER2_TOEI_DISABLE() (CLEAR_BIT(TIMSK,TOIE2))

#endif


/*************Defined Data types**************/
typedef enum
{
	TIMER2_NORMAL,
	TIMER2_PWM_PHASE_CORRECT,
	TIMER2_CTC,
	TIMER2_FAST_PWM,
}TIMER2_modes;

typedef enum
{
	TIMER2_CTC_NORMAL_nonPwm,
	TIMER2_CTC_SET_TC2_nonPwm,
	TIMER2_CTC_TOGGLE_TC2_nonPwm,
	TIMER2_CTC_CLEAR_TC2_nonPwm,
}TIMER2_CTC_NON_PMW_MODES;

typedef enum
{
	TIMER2_CTC_NORMAL,
	TIMER2_CTC_RESERVED,
	TIMER2_CTC_CLEAR_TC2,
	TIMER2_CTC_SET_TC2,
}TIMER2_CTC_FAST_PMW_MODES;

typedef enum
{
	TIMER2_FOSC_,
	TIMER2_FOSC_8,
	TIMER2_FOSC_64,
	TIMER2_FOSC_256,
	TIMER2_FOSC_1024,
	TIMER2_T2_COUNTER_MODE_FEDG,
	TIMER2_T2_COUNTER_MODE_REDG,
}TIMER2_PRESCALERS;

typedef struct
{
	TIMER2_modes Mode;
	TIMER2_CTC_NON_PMW_MODES CTC_NON_PWM_mode;
	TIMER2_CTC_FAST_PMW_MODES CTC_FAST_PWM_Mode;
	TIMER2_PRESCALERS Prescaler;
	uint8 Preload_val;
#if TIMER2_INTERRUPT_COMP_ENABLE==FEATURE_ENABLE
	void (*INT_COMP_Callback)(void);
#endif
#if TIMER2_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
	void (*INT_OVF_Callback)(void);
#endif
}Timer2_config_t;


/*********Software Interfaces*******************/
Bool Timer2_init(Timer2_config_t *Timer2);
Bool Timer2_set_Val(uint8 Val);
Bool Timer2_get_Val(uint8* Val);
Bool Timer2_delay_polling(Timer2_config_t *Timer2);
Bool Timer2_Setduty(uint8 Duty);



#endif /* TIMER2_H_ */
