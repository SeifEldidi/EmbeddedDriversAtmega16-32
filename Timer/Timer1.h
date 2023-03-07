/*
 * Timer1.h
 *
 *  Created on: Mar 3, 2023
 *      Author: Seif pc
 */

#ifndef TIMER1_H_
#define TIMER1_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/

#define TIMER1_PORT_DIR DDRD
#define OC1A PD5
#define OC1B PD4
#define PWM_8BITS_MAX 256
#define PWM_9BITS_MAX 512
#define PWM_10BITS_MAX 1024
/***********Macro Like Function**************/
#define TIMER1_PWM_MODE() {CLEAR_BIT(TCCR1A,FOC1A);\
							CLEAR_BIT(TCCR1A,FOC1B);}

#define TIMER1_NORMAL_MODE() {SET_BIT(TCCR1A,FOC1A);\
							 SET_BIT(TCCR1A,FOC1B);}

#define TIMER1_INPUT_NOISE_CANCEL_EN()  (SET_BIT(TCCR1B,ICNC1))
#define TIMER1_INPUT_NOISE_CANCEL_DIS() (CLEAR_BIT(TCCR1B,ICNC1))

#define TIMER1_INPUT_CAPTURE_RISING_EDG()  (SET_BIT(TCCR1B,ICES1))
#define TIMER1_INPUT_CAPTURE_FALLING_EDG() (CLEAR_BIT(TCCR1B,ICES1))

#define TIMER1_STOP() (TCCR1B&=0xF8)

#define TIMER1_INPUT_CAP_INT_FLAG_CLEAR() (SET_BIT(TIFR,ICF1))
#define TIMER1_OUTPUT_CMP_OC1A_FLAG_CLEAR() (SET_BIT(TIFR,OCF1A))
#define TIMER1_OUTPUT_CMP_OC1B_FLAG_CLEAR() (SET_BIT(TIFR,OCF1B))
#define TIMER1_OVERFLOW_FLAG_CLEAR() (SET_BIT(TIFR,TOV1))

#if TIMER1_INTERRUPT_ICP_ENABLE==FEATURE_ENABLE

#define TIMER1_INPUT_CAP_INT_ENABLE() (SET_BIT(TIMSK,TICIE1))
#define TIMER1_INPUT_CAP_INT_DISABLE() (CLEAR_BIT(TIMSK,TICIE1))
#endif

#if TIMER1_INTERRUPT_CMPA_ENABLE==FEATURE_ENABLE
#define TIMER1_OUTPUT_CMP_OC1A_INT_ENABLE() (SET_BIT(TIMSK,OCIE1A))
#define TIMER1_OUTPUT_CMP_OC1A_INT_DISABLE() (CLEAR_BIT(TIMSK,OCIE1A))
#endif

#if TIMER1_INTERRUPT_CMPB_ENABLE==FEATURE_ENABLE
#define TIMER1_OUTPUT_CMP_OC1B_INT_ENABLE()  (SET_BIT(TIMSK,OCIE1B))
#define TIMER1_OUTPUT_CMP_OC1B_INT_DISABLE() (CLEAR_BIT(TIMSK,OCIE1B))
#endif

#if TIMER1_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
#define TIMER1_OVERFLOW_INT_ENABLE()  (SET_BIT(TIMSK,TOIE1))
#define TIMER1_OVERFLOW_INT_DISABLE() (CLEAR_BIT(TIMSK,TOIE1))
#endif

/*************Defined Data types**************/
typedef enum
{
	TIMER1_COMPA_ACTIVE,
	TIMER1_COMPB_ACTIVE,
	TIMER1_COMPA_COMPB_ACTIVE,
}COMPARE_UNIT_ACTIVE;

typedef enum
{
	TIMER1_Normal_non_pwm_mode,
	TIMER1_ToggleOC_non_pwm_mode,
	TIMER1_ClearOC_non_pwm_mode,
	TIMER1_SetOC_non_pwm_mode,
	TIMER1_Normal_Fast_PWM_Mode,
	TIMER1_Normal_ReservedToggle_Fast_PWM_Mode,
	TIMER1_ClearOC_Fast_PWM_Mode,
	TIMER1_setOC_Fast_PWM_Mode,
	TIMER1_Normal_Phase_Correct_PWM,
	TIMER1_Toggle_Phase_Correct_PWM,
	TIMER1_ClearOC_Phase_Correct_PWM,
	TIMER1_setOC_Phase_Correct_PWM,
}COMPARE_OUTPUT_MODE;

typedef enum
{
	TIMER1_Normal_T1,
	TIMER1_PWM_Phase_Correct_8_bit,
	TIMER1_PWM_Phase_Correct_9_bit,
	TIMER1_PWM_Phase_Correct_10_bit,
	TIMER1_CTC_OCR1,
	TIMER1_PWM_Fast_8_bit,
	TIMER1_PWM_Fast_9_bit,
	TIMER1_PWM_Fast_10_bit,
	TIMER1_PWM_Phase_and_Frequency_Correct_ICR1,
	TIMER1_PWM_Phase_and_Frequency_Correct_OCR1,
	TIMER1_PWM_Phase_Correct_ICR1,
	TIMER1_PWM_Phase_Correct_OCR1,
	TIMER1_CTC_ICR1,
	TIMER1_PWM_FAST_ICR1,
	TIMER1_PWM_FAST_OCR1,
}TIMER1_WAVEFORM_GENERATION;

typedef enum
{
	TIMER_FOSC_1,
	TIMER_FOSC_8,
	TIMER_FOSC_64,
	TIMER_FOSC_256,
	TIMER_FOSC_1024,
	EXTERNAL_SOURCE_COUNTER_T1_FALLING_EDG,
	EXTERNAL_SOURCE_COUNTER_T1_RISING_EDG,
}TIMER1_PRESCALER_CONFIG;

typedef struct
{
	COMPARE_UNIT_ACTIVE REG_active;
	COMPARE_OUTPUT_MODE TIMER1_mode;
	TIMER1_WAVEFORM_GENERATION WVFM_GEN;
	TIMER1_PRESCALER_CONFIG Prescaler;
	uint16 Preload_val;
	uint32 PWM_freq;
	uint8 Duty_Cycle_1;
	uint8 Duty_Cycle_2;
#if TIMER1_INTERRUPT_OVF_ENABLE==FEATURE_ENABLE
	void (*TIMER1_OVERFLOW_CALLBACK)(void);
#endif
#if TIMER1_INTERRUPT_CMPA_ENABLE==FEATURE_ENABLE
	void (*TIMER1_OUTPUT_CMP_OC1A_CALLBACK)(void);
#endif
#if TIMER1_INTERRUPT_CMPB_ENABLE==FEATURE_ENABLE
	void (*TIMER1_OUTPUT_CMP_OC1B_CALLBACK)(void);
#endif
#if TIMER1_INTERRUPT_ICP_ENABLE==FEATURE_ENABLE
	void (*TIMER1_INPUT_CAP_CALLBACK)(void);
#endif
}TIMER1_CONFIG_t;

/*********Software Interfaces*******************/
Bool TIMER1_INIT(TIMER1_CONFIG_t *Timer1);
Bool TIMER1_SET_DUTY(TIMER1_CONFIG_t *Timer1,uint8 duty,uint8 duty_1);
Bool TIMER1_SET_Freq(TIMER1_CONFIG_t *Timer1,uint32 Freq);
Bool TIMER1_SET_VAL(TIMER1_CONFIG_t *Timer1,uint16 VAL);
Bool TIMER1_GET_VAL(TIMER1_CONFIG_t *Timer1,uint16 *VAL);
Bool TIMER1_delay_polling(TIMER1_CONFIG_t *Timer1,uint16 VAL);


#endif /* TIMER1_H_ */
