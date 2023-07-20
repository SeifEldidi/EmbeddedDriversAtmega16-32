/*
 * mcal_adc.c
 *
 *  Created on: May 26, 2023
 *      Author: Seif pc
 */
#include "macl_adc.h"

#if ADC_interrupt_enable==TRUE
static void (*ADC_call_ptr)(void)=NULL;
#endif

static inline void Adc_set_channel(uint8 Adc_channel)
{
	ADMUX&=0xE0;
	ADMUX|=Adc_channel;
}

static inline void ADC_read(uint8 ADC_justification,uint16*Result);

uint8 ADC_init(ADC_config_t *Adc)
{
	uint8 Err_state=E_OK;
	if(Adc==NULL)
		Err_state=E_NOK;
	else
	{
		ADC_DISABLE();
		ADMUX&=0xE0;
		ADMUX|=Adc->ADC_channel;
		switch(Adc->ADC_justification)
		{
			case ADC_Left_justified:
				ADC_LEFT_justified();
				break;
			case ADC_Right_justified:
				ADC_RIGHT_justified();
				break;
			default:
				Err_state=E_NOK;
		}
		ADCSRA|=Adc->Clock;
		switch(Adc->Voltage_source)
		{
			case ADC_refrence_V_Aref:
				WRITE_BIT(ADMUX,REFS0,FALSE);
				WRITE_BIT(ADMUX,REFS1,FALSE);
				break;
			case ADC_refrence_V_Avcc_cap:
				WRITE_BIT(ADMUX, REFS0, TRUE);
				WRITE_BIT(ADMUX, REFS1, FALSE);
				break;
			case ADC_Internal_V_2_56_Pullup:
				WRITE_BIT(ADMUX, REFS0, TRUE);
				WRITE_BIT(ADMUX, REFS1, TRUE);
				break;
			default:
				Err_state=E_NOK;
		}
#if ADC_interrupt_enable==TRUE
		sei();
		ADC_Flag_Clear();
		ADC_Interrupt_Enable();
		ADC_call_ptr=Adc->Adc_callback;
#endif
		ADC_ENABLE() ;
	}
	ADC_START_Conversion();
	return Err_state;
}

uint8 ADC_config_channel(ADC_config_t *Adc,uint8 Adc_channel)
{
	uint8 Err_state=E_OK;
	if(Adc==NULL)
		Err_state=E_NOK;
	else
	{
		Adc->ADC_channel=Adc_channel;
		Adc_set_channel(Adc_channel);
	}
	return Err_state;
}

uint8 ADC_Conversion_polling(ADC_config_t *Adc,uint16 *Result)
{
	uint8 Err_state=E_OK;
	if (Adc == NULL)
		Err_state = E_NOK;
	else
	{
		ADC_config_channel(Adc,Adc->ADC_channel);
		ADC_START_Conversion();
		while(ADC_CONV_IN_PROGRESS());
		ADC_read(Adc->ADC_justification,Result);
	}
	return Err_state;
}

static  void ADC_read(uint8 ADC_justification,uint16*Result)
{
	switch(ADC_justification)
	{
	case ADC_Left_justified:
		*Result=ADCL+(uint16)256*ADCH;
		break;
	case ADC_Right_justified:
		*Result=(ADCL>>6)+(ADCH&0x3F)+(uint16)((ADCH>>6)*256);
		break;
	}
}

void ADC_Conversion_interrupt(uint8 ADC_justification,uint16*Result,uint8 Next_channel)
{
	switch (ADC_justification) {
	case ADC_Left_justified:
		*Result = ADCL + (uint16) 256 * ADCH;
		break;
	case ADC_Right_justified:
		*Result = (ADCL >> 6) + (ADCH & 0x3F) + (uint16) ((ADCH >> 6) * 256);
		break;
	}
	Adc_set_channel(Next_channel);
}

#if ADC_interrupt_enable==TRUE
void ADC_ISR()
{
	if(ADC_call_ptr)
		ADC_call_ptr();
}
#endif






