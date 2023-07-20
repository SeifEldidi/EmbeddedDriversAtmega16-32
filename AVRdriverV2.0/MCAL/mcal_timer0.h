/*
 * Timer1.h
 *
 *  Created on: May 28, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_TIMER0_H_
#define MCAL_MCAL_TIMER0_H_

/************Includes Section*********/
#include "mcal_std_types.h"
#include "Mcal_External_Interrupts.h"
/********Define Section**************/
#define TIMER0_DDR DDRB
#define OCO 3
#define TIMER0_PRESCALER_1 0x01
#define TIMER0_PRESCALER_8 0x02
#define TIMER0_PRESCALER_64 0x03
#define TIMER0_PRESCALER_256 0x04
#define TIMER0_PRESCALER_1024 0x05
#define TIMER0_COUNTER_T0_PIN_FEDG 0x06
#define TIMER0_COUNTER_T0_PIN_REDG 0x07
/**********Function Macro Section**********/
#define TIMER0_STOP() {\
	CLEAR_BIT(TCCR0,CS00);\
	CLEAR_BIT(TCCR0,CS01);\
	CLEAR_BIT(TCCR0,CS02);\
}
#define TIMER0_RESET(){\
	TCCR0=0x00;\
	TCNT0=0x00;\
}
#define TIMER0_READ(Timer0_read) (Timer0_read=TCNT0)
#define TIMER0_Write(Timer0_Write) (TCNT0=Timer0_Write)

#define TIMER0_T0_OVF_INT_Enable() (SET_BIT(TIMSK,TOIE0))
#define TIMER0_T0_CTC_INT_Enable() (SET_BIT(TIMSK,OCIE0))

#define TIMER0_T0_OVF_Flag_Clear() (SET_BIT(TIFR,TOV0))
#define TIMER0_T0_CTC_Flag_Clear() (SET_BIT(TIFR,OCF0))
/***********User Defined Section**********/
typedef enum
{
	WFGM_T0_NORMAL,
	WFGM_PWM_T0_PHASE_CORRECT,
	WFGM_CTC_T0,
	WFGM_T0_FAST_PWM,
}WFGM_T0;

typedef enum
{
	CTCT_T0_NORMAL_PORT,
	CTC_T0_TOGGLE_OC0,
	CTC_T0_CLEAR_OC0,
	CTCT_T0_SET_OC0,
}CTC_T0_Setting;

typedef enum
{
	FAST_PWM_T0_Normal,
	FAST_PWM_T0_Rese,
	FAST_PWM_T0_NonInverting,
	FAST_PWM_T0_Inverting,
}FAST_PWM_T0_setting;

typedef enum
{
	PHASE_CORRECT_PWM_T0_Normal,
	PHASE_CORRECT_PWM_T0_Rese,
	PHASE_CORRECT_PWM_T0_NonInverting,
	PHASE_CORRECT_PWM_T0_Inverting,
}PHASE_CORRECT_PWM_T0_setting;

typedef struct
{
	WFGM_T0 WFGM_mode;
	CTC_T0_Setting CTC_mode;
	FAST_PWM_T0_setting Fast_PWM_setting;
	PHASE_CORRECT_PWM_T0_setting Phase_PWM_setting;
	uint8 Prescaler_Counter_setting;
	uint8 T0_duty_cycle;
	uint8 T0_TCNT0_init_val;
#if Timer0_OVF_Int==TRUE
	void (*TIMER0_ovf_callback)(void);
#endif
#if Timer0_CTC_Int==TRUE
	void (*TIMER0_ctc_callback)(void);
#endif
}Timer0_config;

/*************Software Interfaces***************/
uint8 Mcal_Timer0_init(Timer0_config *Timer0);
void	  Mcal_TIMER0_set_duty(uint8 Duty_cycle);
void  Mcal_Timer0_delay_polling(uint8 TCNT0_val);

#endif /* MCAL_MCAL_TIMER0_H_ */
