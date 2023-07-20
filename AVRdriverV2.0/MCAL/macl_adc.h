/*
 * ADC.h
 *
 *  Created on: May 26, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MACL_ADC_H_
#define MCAL_MACL_ADC_H_

/**********Includes Section********/
#include "mcal_std_types.h"
#include "Mcal_External_Interrupts.h"
/*********Define Section**********/
#define Adc_channel0   0x00
#define Adc_channel1   0x01
#define Adc_channel2   0x02
#define Adc_channel3   0x03
#define Adc_channel4   0x04
#define Adc_channel5   0x05
#define Adc_channel6   0x06
#define Adc_channel7   0x07

#define ADC_refrence_V_Aref 				0x00
#define ADC_refrence_V_Avcc_cap		0x01
#define ADC_Internal_V_2_56_Pullup 	0x03

#define ADC_Left_justified 			0x00
#define ADC_Right_justified	 	0x01
/*************Macro Function Section**********/
#define ADC_LEFT_justified()  (CLEAR_BIT(ADCSRA,ADLAR))
#define ADC_RIGHT_justified() (SET_BIT(ADCSRA,ADLAR))

#define ADC_ENABLE() (SET_BIT(ADCSRA,ADEN))
#define ADC_DISABLE() (CLEAR_BIT(ADCSRA,ADEN))

#define ADC_START_Conversion() (SET_BIT(ADCSRA,ADSC))
#define ADC_CONV_IN_PROGRESS() (READ_BIT(ADCSRA,ADSC))

#define ADC_Interrupt_Enable()  (SET_BIT(ADCSRA,ADIE))
#define ADC_Interrupt_Disable()  (CLEAR_BIT(ADCSRA,ADIE))

#define ADC_Flag_Clear() (SET_BIT(ADCSRA,ADIF))
/*********************User Defined Data types***********/
typedef enum
{
	AVR_CLOCK_2,
	AVR_CLOCK_2_,
	AVR_CLOCK_4,
	AVR_CLOCK_8,
	AVR_CLOCK_16,
	AVR_CLOCK_32,
	AVR_CLOCK_64,
	AVR_CLOCK_128,
}Adc_clock;

typedef struct
{
	Adc_clock Clock;
	uint8 Voltage_source:2;
	uint8 ADC_justification:1;
	uint8 ADC_channel:3;
	uint8 reserved:2;
#if ADC_interrupt_enable==TRUE
	void(*Adc_callback)(void);
#endif
}ADC_config_t;

uint8 ADC_init(ADC_config_t *Adc);
uint8 ADC_config_channel(ADC_config_t *Adc,uint8 Adc_channel);
uint8 ADC_Conversion_polling(ADC_config_t *Adc,uint16 *Result);
void ADC_Conversion_interrupt(uint8 ADC_justification,uint16*Result,uint8 Next_channel);
#endif /* MCAL_MACL_ADC_H_ */
