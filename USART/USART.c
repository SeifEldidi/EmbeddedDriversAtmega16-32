/*
 * USART.c
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */
#include "mcal_uart.h"

#define BAUD_PRESCALE(USART_BAUDRATE) (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#if USART_INT_TX==TRUE
	void (*UASRT_TX_callback_ptr)(void)=NULL;
	#endif
#if USART_INT_RX==TRUE
	void(* UASRT_RX_callback_ptr)(void)=NULL;
#endif

static void Baud_rate_set(USART_Config_t *USART)
{
	uint32 UBRR;
	switch(USART->Enhanced_Speed)
	{
		case USART_DOUBLE_SPEED_ENABLE:
			USART_D2X_ENABLE();
			UBRR=(((F_CPU / (USART->Baud_rate * 8UL))) - 1);
			UBRRL=(uint8)UBRR;
			UBRRH=(uint8)(UBRR>>8);
			break;
		case USART_DOUBLE_SPEED_DISABLE:
			USART_D2X_DISABLE();
			UBRR=(((F_CPU / (USART->Baud_rate * 16UL))) - 1);
			UBRRL=(uint8)UBRR;
			UBRRH=(uint8)(UBRR>>8);
			break;
	}
}

static void USART_BUFF_SIZE_set(USART_Config_t *USART)
{
	switch(USART->Buff_size)
	{
		case USART_DATA_5_BITS:
			WRITE_BIT(UCSRB,UCSZ2,FALSE);
			break;
		case USART_DATA_6_BITS:
			UCSRC|=(1<<URSEL|1<<UCSZ0);
			WRITE_BIT(UCSRB,UCSZ2,FALSE);
					break;
		case USART_DATA_7_BITS:
			UCSRC|=(1<<URSEL|1<<UCSZ1);
			WRITE_BIT(UCSRB,UCSZ2,FALSE);
					break;
		case USART_DATA_8_BITS:
			UCSRC|=(1<<URSEL|1<<UCSZ1|1<<UCSZ0);
			WRITE_BIT(UCSRB,UCSZ2,FALSE);
					break;
		case USART_DATA_9_BITS:
			UCSRC|=(1<<URSEL|1<<UCSZ1|1<<UCSZ0);
			WRITE_BIT(UCSRB,UCSZ2,TRUE);
			break;
			default:
				break;
	}
	switch (USART->P_Mode) {
		case USART_PARITY_EVEN:
			UCSRC |= (1 << URSEL | 1 << UPM1);
			break;
		case USART_PARITY_ODD:
			UCSRC |= (1 << URSEL | 1 << UPM1 | 1 << UPM0);
			break;
		default:
			break;
	}
}

uint8 Mcal_Usart_init(USART_Config_t *USART)
{
	uint8 Err_state=E_OK;
	if(USART==NULL)
		Err_state=E_NOK;
	else
	{
		Baud_rate_set(USART);
		USART_BUFF_SIZE_set(USART);
		switch (USART->NO_stop_bits) {
		case USART_1_STOP_BITS:
			break;
		case USART_2_STOP_BITS:
			UCSRC|=(1<<URSEL|1<<USBS);
			break;
#if USART_INT_TX==TRUE
			sei();
			USART_TX_INT_ENABLE();
			UASRT_TX_callback_ptr=USART->UASRT_TX_callback;
	#endif
#if USART_INT_RX==TRUE
			sei();
			USART_RX_INT_ENABLE();
			UASRT_RX_callback_ptr=USART->UASRT_RX_callback;
#endif
		USART_TX_ENABLE();
		USART_RX_ENABLE();
		}
	}
	return Err_state;
}


uint8 Mcal_Usart_send(USART_Config_t *USART,uint8 Data)
{
	uint8 Err_state = E_OK;
	if (USART == NULL)
		Err_state = E_NOK;
	else {
		uint8 Parity_bit=0;
		switch(USART->Buff_size)
		{
			case USART_DATA_9_BITS:
			{
				switch(USART->P_Mode)
				{
				case USART_PARITY_EVEN:
					Parity_bit = (READ_BIT(Data, 7)) ^ (READ_BIT(Data, 6))
						^ (READ_BIT(Data, 5)) ^ (READ_BIT(Data, 4))
						^ (READ_BIT(Data, 3)) ^ (READ_BIT(Data, 2))
						^ (READ_BIT(Data, 1)) ^ (READ_BIT(Data, 0));
					while (USART_BUFFER_NEMPTY());
					WRITE_BIT(UCSRB,TXB8,Parity_bit);
					UDR=Data;
					break;
				case USART_PARITY_ODD:
						Parity_bit = !((READ_BIT(Data, 7)) ^ (READ_BIT(Data, 6))
						^ (READ_BIT(Data, 5)) ^ (READ_BIT(Data, 4))
						^ (READ_BIT(Data, 3)) ^ (READ_BIT(Data, 2))
						^ (READ_BIT(Data, 1)) ^ (READ_BIT(Data, 0)));
						while (USART_BUFFER_NEMPTY());
						WRITE_BIT(UCSRB,TXB8,Parity_bit);
						UDR = Data;
						break;
				default:
					break;
				}
			}
			default:
				while (USART_BUFFER_NEMPTY());
						UDR = Data ;
				break;
		}
	}
	return Err_state;
}

uint8 Mcal_Usart_send_string(USART_Config_t *USART,uint8* Data)
{
	uint8 Err_state = E_OK;
	if (USART == NULL)
		Err_state = E_NOK;
	else {
		uint8 i=0;
		while(*(Data+i)!='\0')
		{
			Mcal_Usart_send(USART,*(Data+i));
			i++;
		}
	}
	return Err_state;
}

uint8 Mcal_Usart_rx(USART_Config_t *USART,uint8 *Data)
{
	uint8 Err_state = E_OK;
	if (USART == NULL)
			Err_state = E_NOK;
	else {
		while (!READ_BIT(UCSRA , RXC));/* Wait till data is received */
		*Data=UDR;			/* Return the byte*/
	}
	return Err_state;
}

uint8 Usart_rx_string(USART_Config_t *USART,uint8 *Data)
{
	uint8 Err_state = E_OK;
	if (USART == NULL)
		Err_state = E_NOK;
	else {
			uint8 i = 0;
			while (*(Data + i) != '\0') {
				Mcal_Usart_rx(USART,Data+i);
				i++;
			}
	}
	return Err_state;
}


void USART_TX_ISR()
{
#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
	USART_TX_FLAG_CLEAR();
	if(UASRT_TX_callback_ptr)
		UASRT_TX_callback_ptr();
#endif
}

void USART_RX_ISR()
{
#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
	USART_RX_FLAG_CLEAR();
	if (UASRT_RX_callback_ptr)
		UASRT_RX_callback_ptr();
#endif
}
