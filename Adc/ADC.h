/*
 * ADC.h
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#ifndef ADC_H_
#define ADC_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define ADC_LEFT_JUSTIFIED 1
#define ADC_RIGHT_JUSTIFIED 0
/***********Macro Like Function**************/
#define ADC_RIGHT_JUST() (CLEAR_BIT(ADMUX,ADLAR))
#define ADC_LEFT_JUST() (SET_BIT(ADMUX,ADLAR))

#define ADC_DISABLE() (CLEAR_BIT(ADCSRA,ADEN))
#define ADC_ENABLE()  (SET_BIT(ADCSRA,ADEN))

#define ADC_START_CONV() (SET_BIT(ADCSRA,ADSC))
#define ADC_STOP_CONV() (CLEAR_BIT(ADCSRA,ADSC))

#define ADC_AUTO_TRIGGER() ((SET_BIT(ADCSRA,ADATE))
#define ADC_MANUAL_TRIGGER() (CLEAR_BIT(ADCSRA,ADATE))

#define ADC_INTERRUPT_ENABLE_() (SET_BIT(ADCSRA,ADIE))
#define ADC_INTERRUPT_DIABLE_() (CLEAR_BIT(ADCSRA,ADIE))
#define ADC_FLAG_CLEAR()       (SET_BIT(ADCSRA,ADIF))
/*************Defined Data types**************/
typedef enum
{
	AREF_VOLTAGE=0,
	AVCC_EXTERNAL_CAP,
	RES,
	INTERNAL_2_56_VREF_EXT_CAP,
}VOLTAGE_LEVEL;

typedef enum
{
	ADC0_CHANNEL=0,
	ADC1_CHANNEL,
	ADC2_CHANNEL,
	ADC3_CHANNEL,
	ADC4_CHANNEL,
	ADC5_CHANNEL,
	ADC6_CHANNEL,
	ADC7_CHANNEL,
}ADC_CHANNEL_SEL;

typedef enum
{
	OSC_FREQ_2=0,
	OSC_FREQ_2_R,
	OSC_FREQ_4,
	OSC_FREQ_8,
	OSC_FREQ_16,
	OSC_FREQ_32,
	OSC_FREQ_64,
	OSC_FREQ_128,
}ADC_FREQ;

typedef struct
{
	uint8 ADC_justification:1;
	VOLTAGE_LEVEL ADC_Voltage_Supply;
	ADC_CHANNEL_SEL ADC_Channel;
	ADC_FREQ ADC_Freq;
#if ADC_INTERRUPT_ENABLE==FEATURE_ENABLE
	void (*ADC_ISR_callback)(void);
#endif
}ADC_CONFIG_t;


/*********Software Interfaces*******************/

Bool ADC_INIT(ADC_CONFIG_t *ADC_obj);
Bool ADC_get_Conv(ADC_CONFIG_t *ADC_obj,ADC_CHANNEL_SEL Channel,uint16 *res);
Bool ADC_Get_result(ADC_CONFIG_t *ADC_obj,ADC_CHANNEL_SEL Channel,uint16 *res);


#endif /* ADC_H_ */
