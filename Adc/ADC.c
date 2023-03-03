/*
 * ADC.c
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#include "ADC.h"

#if ADC_INTERRUPT_ENABLE==FEATURE_ENABLE
static void (*ADC_callback)(void)=NULL;
#endif

static inline Bool get_res(uint8 Just,uint16 *res)
{
	Bool ret=E_OK;
	switch(Just)
	{
	 case ADC_LEFT_JUSTIFIED:
		 *res=((ADCL+(uint16)(ADCH*256))>>6);
		 break;
	 case ADC_RIGHT_JUSTIFIED:
		 *res=(ADCL+(uint16)(ADCH*256));
		 break;
	}
	return ret;
}

static Bool ADC_REF_voltage_set(ADC_CONFIG_t *ADC_obj)
{
	Bool ret = E_OK;
	if (NULL == ADC_obj)
		ret = E_NOK;
	else {
		switch(ADC_obj->ADC_Voltage_Supply)
		{
			case AREF_VOLTAGE:
				CLEAR_BIT(ADMUX,REFS1);
				CLEAR_BIT(ADMUX,REFS0);
				break;
			case AVCC_EXTERNAL_CAP:
				CLEAR_BIT(ADMUX,REFS1);
				SET_BIT(ADMUX,REFS0);
				break;
			case RES:
				ret=E_NOK;
				break;
			case INTERNAL_2_56_VREF_EXT_CAP:
				SET_BIT(ADMUX,REFS0);
				SET_BIT(ADMUX,REFS1);
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

static Bool ADC_REF_JUST_set(ADC_CONFIG_t *ADC_obj)
{
	Bool ret = E_OK;
	if (NULL == ADC_obj)
		ret = E_NOK;
	else {
		switch(ADC_obj->ADC_justification)
		{
			case ADC_LEFT_JUSTIFIED:
				ADC_LEFT_JUST();
				break;
			case ADC_RIGHT_JUSTIFIED:
				ADC_RIGHT_JUST();
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

static Bool ADC_FREQ_set(ADC_CONFIG_t *ADC_obj)
{
	Bool ret = E_OK;
	if (NULL == ADC_obj)
		ret = E_NOK;
	else {
		switch(ADC_obj->ADC_Freq)
		{
			case OSC_FREQ_2:
			case OSC_FREQ_2_R:
				CLEAR_BIT(ADCSRA,ADPS0);
				CLEAR_BIT(ADCSRA,ADPS1);
				CLEAR_BIT(ADCSRA,ADPS2);
				break;
			case OSC_FREQ_4:
				CLEAR_BIT(ADCSRA,ADPS0);
				SET_BIT(ADCSRA,ADPS1);
				CLEAR_BIT(ADCSRA,ADPS2);
				break;
			case OSC_FREQ_8:
				SET_BIT(ADCSRA,ADPS0);
				SET_BIT(ADCSRA,ADPS1);
				CLEAR_BIT(ADCSRA,ADPS2);
				break;
			case OSC_FREQ_16:
				CLEAR_BIT(ADCSRA,ADPS0);
				CLEAR_BIT(ADCSRA,ADPS1);
				SET_BIT(ADCSRA,ADPS2);
				break;
			case OSC_FREQ_32:
				SET_BIT(ADCSRA,ADPS0);
				CLEAR_BIT(ADCSRA,ADPS1);
				SET_BIT(ADCSRA,ADPS2);
				break;
			case OSC_FREQ_64:
				CLEAR_BIT(ADCSRA,ADPS0);
				SET_BIT(ADCSRA,ADPS1);
				SET_BIT(ADCSRA,ADPS2);
				break;
			case OSC_FREQ_128:
				SET_BIT(ADCSRA,ADPS0);
				SET_BIT(ADCSRA,ADPS1);
				SET_BIT(ADCSRA,ADPS2);
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

static Bool ADC_CHANNEL_set(ADC_CONFIG_t *ADC_obj)
{
	Bool ret = E_OK;
	if (NULL == ADC_obj)
		ret = E_NOK;
	else {
		ADMUX&=0xE0;
		switch (ADC_obj->ADC_Channel) {
			case ADC0_CHANNEL:
				CLEAR_BIT(ADMUX, MUX0);
				CLEAR_BIT(ADMUX, MUX1);
				CLEAR_BIT(ADMUX, MUX2);
				break;
			case ADC1_CHANNEL:
				SET_BIT(ADMUX, MUX0);
				CLEAR_BIT(ADMUX, MUX1);
				CLEAR_BIT(ADMUX, MUX2);
				break;
			case ADC2_CHANNEL:
				CLEAR_BIT(ADMUX, MUX0);
				SET_BIT(ADMUX, MUX1);
				CLEAR_BIT(ADMUX, MUX2);
				break;
			case ADC3_CHANNEL:
				SET_BIT(ADMUX, MUX0);
				SET_BIT(ADMUX, MUX1);
				CLEAR_BIT(ADMUX, MUX2);
				break;
			case ADC4_CHANNEL:
				CLEAR_BIT(ADMUX, MUX0);
				CLEAR_BIT(ADMUX, MUX1);
				SET_BIT(ADMUX, MUX2);
				break;
			case ADC5_CHANNEL:
				SET_BIT(ADMUX, MUX0);
				CLEAR_BIT(ADMUX, MUX1);
				SET_BIT(ADMUX, MUX2);
				break;
			case ADC6_CHANNEL:
				CLEAR_BIT(ADMUX, MUX0);
				SET_BIT(ADMUX, MUX1);
				SET_BIT(ADMUX, MUX2);
				break;
			case ADC7_CHANNEL:
				SET_BIT(ADMUX, MUX0);
				SET_BIT(ADMUX, MUX1);
				SET_BIT(ADMUX, MUX2);
				break;
			default:
				ret = E_NOK;
		}
	}
	return ret;
}



Bool ADC_INIT(ADC_CONFIG_t *ADC_obj)
{
	Bool ret=E_OK;
	if(NULL==ADC_obj)
		ret=E_NOK;
	else
	{
		ADC_DISABLE();
		ADC_FREQ_set(ADC_obj);
		ADC_REF_voltage_set(ADC_obj);
		ADC_REF_JUST_set(ADC_obj);
		ADC_CHANNEL_set(ADC_obj);
#if ADC_INTERRUPT_ENABLE==FEATURE_ENABLE
		ADC_callback=ADC_obj->ADC_ISR_callback;
		ADC_FLAG_CLEAR();
		ADC_INTERRUPT_ENABLE_();
		GLOBAL_INT_ENABLE();//global interrupts enable
#endif
		ADC_ENABLE();
		ADC_START_CONV();
	}
	return ret;
}

Bool ADC_get_Conv(ADC_CONFIG_t *ADC_obj,ADC_CHANNEL_SEL Channel,uint16 *res)
{
	Bool ret = E_OK;
	if (NULL == ADC_obj)
		ret = E_NOK;
	else {
		ADC_obj->ADC_Channel=Channel;
		ADC_CHANNEL_set(ADC_obj);
		ADC_START_CONV();
		while(BIT_IS_SET(ADCSRA,ADSC));
		get_res(ADC_obj->ADC_justification,res);
	}
	return ret;
}

Bool ADC_Get_result(ADC_CONFIG_t *ADC_obj,ADC_CHANNEL_SEL Channel,uint16 *res)
{
	Bool ret=E_OK;
	ADC_obj->ADC_Channel=Channel;
	ADC_CHANNEL_set(ADC_obj);
	ret=get_res(ADC_obj->ADC_justification,res);
	return ret;
}

void ADC_ISR_()
{
	ADC_FLAG_CLEAR();
	if(ADC_callback)
		ADC_callback();
	else{}
}
