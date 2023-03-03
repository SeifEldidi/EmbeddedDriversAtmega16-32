/*
 * USART.c
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */
#include "USART.h"

#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
	static void (*INT_TxCallback_handler)(void);
	static void (*INT_RxCallabck_handler)(void);
#endif

static inline void USART_DISABLE()
{
	USART_TX_DISABLE();
	USART_RX_DISABLE();
}

static Bool USART_Double_speed_enable(USART_CONFIG_t *USART)
{
	Bool ret=E_OK;
	if(NULL==USART)
	ret=E_NOK;
	else
	{
		switch(USART->USART_D2X_ENALBE)
		{
			case USART_DOUBLE_SPEED_ENABLE:
				USART_DOUBLE_SPEED_ENABLE_();
				break;
			case USART_DOUBLE_SPEED_DISABLE:
				USART_DOUBLE_SPEED_DISABLE_();
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

static Bool USART_Full_duplex(USART_CONFIG_t *USART)
{
	Bool ret = E_OK;
	if (NULL == USART)
		ret = E_NOK;
	else {
		USART_RX_ENABLE();
		USART_TX_ENABLE();
	}
	return ret;
}

static Bool USART_character_size(USART_CONFIG_t *USART)
{
	Bool ret = E_OK;
	if (NULL == USART)
		ret = E_NOK;
	else {
		switch(USART->Data_Size)
		{
			USSCRC_SEL();
			case USART_5BITS:
				CLEAR_BIT(UCSRB,UCSZ2);
				CLEAR_BIT(UCSRC,UCSZ0);
				CLEAR_BIT(UCSRC,UCSZ1);
				break;
			case USART_6BITS:
				CLEAR_BIT(UCSRB,UCSZ2);
				SET_BIT(UCSRC,UCSZ0);
				CLEAR_BIT(UCSRC,UCSZ1);
				break;
			case USART_7BITS:
				CLEAR_BIT(UCSRB,UCSZ2);
				CLEAR_BIT(UCSRC,UCSZ0);
				SET_BIT(UCSRC,UCSZ1);
				break;
			case USART_8BITS:
				CLEAR_BIT(UCSRB,UCSZ2);
				SET_BIT(UCSRC,UCSZ0);
				SET_BIT(UCSRC,UCSZ1);
				break;
			case USART_9BITS:
				SET_BIT(UCSRB,UCSZ2);
				SET_BIT(UCSRC,UCSZ0);
				SET_BIT(UCSRC,UCSZ1);
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

static Bool USART_Parity_set(USART_CONFIG_t *USART)
{
	Bool ret=E_OK;
	switch(USART->Parity)
	{
		case USART_PARITY_DISABLE:
			CLEAR_BIT(UCSRC,UPM0);
			CLEAR_BIT(UCSRC,UPM1);
			break;
		case USART_PARITY_ENABLE_EVEN:
			CLEAR_BIT(UCSRC,UPM0);
			SET_BIT(UCSRC,UPM1);
			break;
		case USART_PARITY_ENABLE_ODD:
			SET_BIT(UCSRC,UPM0);
			SET_BIT(UCSRC,UPM1);
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool USART_STOPbits_set(USART_CONFIG_t *USART)
{
	Bool ret=E_OK;
	switch(USART->USART_NO_STOPBITS)
	{
		case USART_TWO_STOP_BITS:
			USART_TWO_STOP_BITS_();
			break;
		case USART_ONE_STOP_BIT:
			USART_ONE_STOP_BIT_();
			break;
		default:
			ret=E_NOK;
	}
	return ret;
}

static Bool USART_BuadRate_set(USART_CONFIG_t *USART)
{
	Bool ret=E_OK;
	uint16 Baud_r;
	if (NULL == USART)
		ret = E_NOK;
	else
	{
		switch(USART->USART_D2X_ENALBE)
		{
			UBBRH_SEL();
			case USART_DOUBLE_SPEED_ENABLE:
				Baud_r=((F_CPU)/(8UL*USART->Baudrate))-1;
				UBRRL=(uint8)(Baud_r>>8);
				UBRRH=(uint8)Baud_r;
				break;
			case USART_DOUBLE_SPEED_DISABLE:
				Baud_r=((F_CPU)/(16UL*USART->Baudrate))-1;
				UBRRL=(uint8)(Baud_r>>8);
				UBRRH=(uint8)Baud_r;
				break;
			default:
				ret = E_NOK;
		}
	}
	return ret;
}

Bool USART_init(USART_CONFIG_t *USART)
{
	Bool ret=E_OK;
	if(NULL==USART)
		ret=E_NOK;
	else
	{
		USART_DISABLE();
		USART_ASYNC_MODE();
		ret=USART_Double_speed_enable(USART);
		ret=USART_character_size(USART);
		ret=USART_Parity_set(USART);
		ret=USART_STOPbits_set(USART);
		ret=USART_BuadRate_set(USART);
#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
		USART_TX_CLEAR_FLAG();
		USART_RX_CLEAR_FLAG();

		GLOBAL_INT_ENABLE();
		USART_TX_INT_ENABLE();
		USART_RX_INT_ENABLE();

		INT_TxCallback_handler=USART->INT_Tx_handler;
		INT_RxCallabck_handler=USART->INT_Rx_handler;
#endif
		ret=USART_Full_duplex(USART);
	}
	return ret;
}

Bool USART_send(uint8 data)
{
	Bool ret=E_OK;
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR=data;
	return ret;
}

Bool USART_send_non_polling(uint8 data)
{
	Bool ret=E_OK;
	if(BIT_IS_SET(UCSRA,UDRE))
	{
	    UDR=data;
	    ret=E_OK;
	}
	else
		ret=E_NOK;
	return ret;
}

Bool USART_send_block(uint8 *data,uint8 size)
{
	Bool ret=E_OK;
	uint8 Bytes_sent=0;
	while(Bytes_sent<size)
	{
		ret=USART_send(*(data+Bytes_sent));
		Bytes_sent++;
	}
	return ret;
}

Bool USART_send_string(uint8 *data)
{
	Bool ret=E_OK;
	uint8 Bytes_sent = 0;
	while (*(data+Bytes_sent)) {
		ret=USART_send(*(data + Bytes_sent));
		Bytes_sent++;
	}
	return ret;
}

Bool USART_recieve(uint8 *data)
{
	Bool ret = E_OK;
	while (BIT_IS_CLEAR(UCSRA,RXC));
	*data=UDR;
	return ret;
}

Bool USART_recieve_non_polling(uint8 *data)
{
	Bool ret = E_OK;
	if (BIT_IS_SET(UCSRA, RXC))
	{
		*data = UDR;
		ret=E_OK;
	}
	else
		ret=E_NOK;
	return ret;
}

Bool USART_recieve_string(uint8 *data)
{
	Bool ret=E_OK;
	uint8 Byte_RX=0;
	ret=USART_recieve(data+Byte_RX);
	while(*(data+Byte_RX)!='#')
	{
		Byte_RX++;
		ret=USART_recieve(data+Byte_RX);
	}
	*(data+Byte_RX)='\0';
	return ret;
}

void USART_TX_ISR()
{
#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
	USART_TX_CLEAR_FLAG();
	if(INT_TxCallback_handler)
		INT_TxCallback_handler();
#endif
}

void USART_RX_ISR()
{
#if USART_INTERRUPT_ENABLE==FEATURE_ENABLE
	USART_RX_CLEAR_FLAG();
	if (INT_RxCallback_handler)
		INT_RxCallback_handler();
#endif
}
