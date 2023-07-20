/*
 * Hal_DHT11.c
 *
 *  Created on: May 31, 2023
 *      Author: Seif pc
 */
#include "Ecual_DHT11.h"

uint32 Int_global_cntr;
uint8 OLD_temp_low;
uint8 OLD_temp_high;
uint8 OLD_humid_low;
uint8 OLD_humid_high;

void TIMER_ISR_Internal_2()
{
	Int_global_cntr++;
}

void  Ecual_DHT11_init(dht11_t  *Dht)
{
	Mcal_GPIO_INIT(&Dht->data);
}

static uint8  DHT11_Resp(dht11_t *Dht)
{
	uint8 Ret=E_OK;
	uint8 Tick_Time=0;
	GPIO_state State = GPIO_LOW;
	Dht->data.Config = GPIO_OUTPUT;
	Ecual_DHT11_init(Dht);
	Mcal_GPIO_WRITE(&Dht->data, GPIO_LOW);
	_delay_ms(20);
	Mcal_GPIO_WRITE(&Dht->data, GPIO_HIGH);
	Dht->data.Config = GPIO_INPUT;
	Mcal_GPIO_INIT(&Dht->data);
	Mcal_GPIO_READ(&Dht->data, &State);
	TIMER0_Write(0);
	while (State == GPIO_HIGH) {
		Tick_Time = TCNT0;
		Mcal_GPIO_READ(&Dht->data, &State);
		if (Tick_Time > 240) {
			Ret = E_NOK;
			break;
		}
	}
	if(Ret==E_OK)
	{
		TIMER0_Write(0);
		while (State == GPIO_LOW) {
			Tick_Time = TCNT0;
			Mcal_GPIO_READ(&Dht->data, &State);
			if (Tick_Time > 240) {
				Ret = E_NOK;
				break;
			}
		}
		if(Ret==E_OK)
		{
			TIMER0_Write(0);
			while (State == GPIO_HIGH) {
				Tick_Time = TCNT0;
				Mcal_GPIO_READ(&Dht->data, &State);
				if (Tick_Time > 240) {
					Ret = E_NOK;
					break;
				}
	      	}
		}
	}
	return Ret;
}

uint8  Ecual_DHT11_get_data(dht11_t  *Dht,uint8 *Arr)
{
		uint8 Rx_counter=0;
		uint8 Bit_counter=0;
		uint8 Ret=E_OK;
		Timer0_config Timer_t;
		uint8 Tick_Time=0;
		GPIO_state State = GPIO_LOW;
		for (Rx_counter = 0; Rx_counter <= 4; Rx_counter++)
			Arr[Rx_counter] = 0; //clear the Temperature/Humidity Buffer
		/**************Timer Mode********/
		TIMER0_RESET();
		Timer_t.WFGM_mode = WFGM_T0_NORMAL;
		Timer_t.T0_TCNT0_init_val = 0;
		Timer_t.Prescaler_Counter_setting=TIMER0_PRESCALER_8; //if 16 Mhz then input Clock= 2Mhz hence Ttick=0.5us hence Timer max=127.5us
		Timer_t.TIMER0_ovf_callback = NULL;
		Mcal_Timer0_init(&Timer_t);//Timer Set
		/********************DHT11 READ*******/
		Ret=DHT11_Resp(Dht);
		if(Ret==E_OK)
		{
			for(Rx_counter=0;Rx_counter<=4;Rx_counter++)
			{
				for(Bit_counter=0;Bit_counter<=7;Bit_counter++)
				{
						TIMER0_Write(0);
						Mcal_GPIO_READ(&Dht->data, &State);
						while (State == GPIO_LOW) {
							Tick_Time=TCNT0;
							Mcal_GPIO_READ(&Dht->data, &State);
							if (Tick_Time > 240) {
								Ret = E_NOK;
								break;
							}
							}
						if (Ret == E_NOK)
							break;
						TIMER0_Write(0);
						Mcal_GPIO_READ(&Dht->data, &State);
						while (State == GPIO_HIGH) {
							Tick_Time=TCNT0;
							Mcal_GPIO_READ(&Dht->data, &State);
							if (TCNT0 > 270) {
								Ret = E_NOK;
								break;
							}
						}
						if (Ret == E_NOK)
							break;
						if(Tick_Time>40&&Tick_Time<90)
							CLEAR_BIT(*(Arr+Rx_counter),(7-Bit_counter));
						else if(Tick_Time<180&&Tick_Time>120)
							SET_BIT(*(Arr+Rx_counter),(7-Bit_counter));
				}
				if(Ret==E_NOK)
					break;
			}
			if(Ret==E_OK)
			{
				OLD_temp_low=*(Arr + 3);
				OLD_temp_high=*(Arr + 2);
				OLD_humid_low=*(Arr + 1);
				OLD_humid_high=*(Arr + 0);
				if ((*(Arr + 0) + *(Arr + 3) + *(Arr + 2) + *(Arr + 1)) !=  *(Arr + 4))
					Ret = E_NOK;
				else
					Ret = E_OK;
			}
		}
		else{
			 *(Arr + 3)=OLD_temp_low;
			 *(Arr + 2)=OLD_temp_high;
			 *(Arr + 1)=OLD_humid_low;
			 *(Arr + 0)=OLD_humid_high;
		}
		return Ret;
}


