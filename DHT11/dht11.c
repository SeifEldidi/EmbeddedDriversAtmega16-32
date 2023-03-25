/*
 * dht11.c
 *
 *  Created on: Mar 22, 2023
 *      Author: Seif pc
 */
#include "dht11.h"

static Timer2_config_t Timer2;

Bool DHT11_INIT(DHT11_CONFIG_t *DHT11)
{
	Bool ret=E_OK;
	Timer2.Mode=TIMER2_NORMAL;
	Timer2.CTC_NON_PWM_mode=TIMER2_CTC_NORMAL_nonPwm;
	Timer2.CTC_FAST_PWM_Mode=TIMER2_CTC_NORMAL;
	Timer2.Prescaler=TIMER2_FOSC_;
	ret=Timer2_init(&Timer2);
	GPIO_PIN_CONFIG(&DHT11->DHT_PIN);
	GPIO_PIN_WRITE(&DHT11->DHT_PIN,GPIO_HIGH);
	return ret;
}

Bool DHT11_SEND_START(DHT11_CONFIG_t *DHT11)
{
	Bool E_State=E_OK;
	DHT11->DHT_PIN.GPIO_STATE=GPIO_OUTPUT;
	GPIO_PIN_CONFIG(&DHT11->DHT_PIN);
	GPIO_PIN_WRITE(&DHT11->DHT_PIN,GPIO_LOW);
	_delay_ms(20);
	SET_BIT(DHT11_PORT_WRITE,DHT11_PIN);
	return E_State;
}

Bool DHT11_RX_RESP(DHT11_CONFIG_t *DHT11)
{
	Bool E_state=E_OK;
	CLEAR_BIT(DHT11_PORT_DIR,DHT11_PIN);
	TCNT2=0;
	while(BIT_IS_SET(DHT11_PORT,DHT11_PIN))//MCU pulls up sensor High
	{
		if(TCNT2>45)
		{
			E_state=E_NOK;
			break;
		}
	}
	if(E_state!=E_NOK)
	{
		TCNT2=0;
		while(BIT_IS_CLEAR(DHT11_PORT,DHT11_PIN))
		{
			if(TCNT2>100)
			{
				E_state=E_NOK;
				break;
			}
		}
	}
	if(E_state!=E_NOK)
	{
		TCNT2=0;
		while(BIT_IS_SET(DHT11_PORT,DHT11_PIN))
		{
			if(TCNT2>100)
			{
				E_state=E_NOK;
				break;
			}
		}
	}
	return E_state;
}

Bool DHT11_FETCH_DATA(DHT11_CONFIG_t *DHT11,uint8 *Arr)
{
	Bool ret=E_OK;
	uint8 Counter = 0,RX_count=0;
	for(Counter=0;Counter<=4;Counter++)
		Arr[Counter]=0;//clear the Temperature/Humidity Buffer
	ret=DHT11_SEND_START(DHT11);//send start to initiate communication
	ret=DHT11_RX_RESP(DHT11);
	for(RX_count=0;RX_count<=4;RX_count++)
	{
		for (Counter = 0; Counter <= BYTE_SIZE - 1; Counter++) {
			TCNT2 = 0;
			while (BIT_IS_CLEAR(DHT11_PORT, DHT11_PIN)) {
				if (TCNT2 > 60) {
					ret = E_NOK;
					break;
				}
				TCNT2 = 0;
			}
			if (ret == E_NOK)
				break;
			TCNT2 = 0;
			while (BIT_IS_SET(DHT11_PORT, DHT11_PIN)) {
				if (TCNT2 > 90) {
					ret = E_NOK;
					break;
				}
			}
			if (ret == E_NOK)
				break;
			uint8 Time_ones = TCNT2;
			if (Time_ones >= 20 && Time_ones <= 40)
				CLEAR_BIT(*(Arr+RX_count), (BYTE_SIZE-1-Counter));
			else if (Time_ones >= 50 && Time_ones <= 90)
				SET_BIT(*(Arr+RX_count), (BYTE_SIZE-1-Counter));
		}
	}
	if(ret==E_OK)
	{
		uint8 Temp_high=*(Arr+3);
		uint8 Temp_low=*(Arr+2);
		uint8 Hum_High=*(Arr+0);
		uint8 Hum_low=*(Arr+1);
		uint8 Checksum=*(Arr+4);
		if((Hum_High+Temp_high+Temp_low+Hum_low)!=Checksum)
			ret=E_NOK;
		else
			ret=E_OK;
	}
	else{}
	return ret;
}

