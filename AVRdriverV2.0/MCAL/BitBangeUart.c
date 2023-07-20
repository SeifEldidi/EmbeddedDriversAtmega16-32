/*
 * BitBangeUart.c
 *
 *  Created on: Jul 10, 2023
 *      Author: Seif pc
 */
#include "BitBangedUart.h"

uint16 TCNT1_Val;

static inline void Usart_Delay(uint16 Val)
{
	TIFR |= (1 << TOV1);// Clear Flag
	TCNT1 = Val;
	while (!READ_BIT(TIFR, TOV1));
	TIFR |= (1 << TOV1);
}

uint8 BitBanged_Usart_init(BitBanged_USART *Usart)
{
	uint8 Err_Status=E_OK;
	if(Usart==NULL)
		Err_Status=E_NOK;
	else
	{
		/****Init Send And Recieve Bits**/
		Usart->TX.State=GPIO_HIGH;
		Usart->TX.Config=GPIO_OUTPUT;
		Usart->RX.Config=GPIO_OUTPUT;
		Usart->RX.State=GPIO_HIGH;
		Mcal_GPIO_INIT(&Usart->TX);
		Mcal_GPIO_INIT(&Usart->RX);
		Usart->RX.Config=GPIO_INPUT;
		Mcal_GPIO_INIT(&Usart->RX);
		/*Initiliaze Timer 1 with suitable Prescaler to make  Time of Tick =1us*/
		TCCR1B|=(1<<CS11);
		uint16 Temp_Val=(uint16)((uint32)1000000/(Usart->BaudRate));
		Temp_Val*=2;
		TCNT1_Val=65536-Temp_Val+(8*Temp_Val)/100;
	}
	return Err_Status;
}

GPIO_state sTATE;

uint8 BITBanged_Usart_Send(BitBanged_USART *Usart,uint8 Data)
{
	uint8 Err_Status=E_OK;
	if(Usart!=NULL){
			uint8 Bits_Tx=0;
			uint8 XOR_val=((Data>>7)&0x01)^((Data>>6)&0x01)^((Data>>5)&0x01)^((Data>>4)&0x01)^((Data>>3)&0x01)^((Data>>2)&0x01)^((Data>>1)&0x01)^((Data>>0)&0x01);
			/*****Send Start Bit********/
			Mcal_GPIO_WRITE(&Usart->TX,GPIO_LOW);
			Usart_Delay(TCNT1_Val);
			/****Send 8 Bits**********/
			for(Bits_Tx=0;Bits_Tx<=7;Bits_Tx++)
			{
				Mcal_GPIO_WRITE(&Usart->TX,(Data>>Bits_Tx)&0x01);
				Usart_Delay(TCNT1_Val);
			}
			/***Send Parity Bits******/
			switch(Usart->Parity_bits)
			{
				case Parity_EVEN:
					Mcal_GPIO_WRITE(&Usart->TX,XOR_val);
					Usart_Delay(TCNT1_Val);
					break;
				case Parity_ODD:
					Mcal_GPIO_WRITE(&Usart->TX,XOR_val^(1));
					Usart_Delay(TCNT1_Val);
					break;
				default:
					break;
			}
			/***Send Stop Bits**/
			switch (Usart->No_STOP_Bits) {
				case NUM_STOP_BITS_1:
					Mcal_GPIO_WRITE(&Usart->TX,GPIO_HIGH);
					Usart_Delay(TCNT1_Val);
					break;
				case NUM_STOP_BITS_2://reduced complexity
					Mcal_GPIO_WRITE(&Usart->TX, GPIO_HIGH);
					Usart_Delay(TCNT1_Val);
					Mcal_GPIO_WRITE(&Usart->TX, GPIO_HIGH);
					Usart_Delay(TCNT1_Val);
					break;
				default:
					break;
			}
	}else{
		Err_Status=E_NOK;
	}
	return Err_Status;
}

uint8 BITBanged_Usart_SendStr(BitBanged_USART *Usart,uint8 *Data)
{
	uint8 Err_Status=E_OK;
	if (Usart != NULL&&Data!=NULL) {
		while(*Data)
		{
			BITBanged_Usart_Send(Usart,*Data);
			Data++;
		}
	} else {
		Err_Status = E_NOK;
	}
	return Err_Status;
}

uint8 BitBanged_Usart_RX(BitBanged_USART *Usart,uint8 *Data)
{
	uint8 Err_Status=E_OK;
	uint16 L_TCNT_1Val;
	if(Usart!=NULL&&Data!=NULL)
	{
		uint8 flag=0;
		/*Check A transition from High to Low on RX pin which is the start Bit*/
		if(flag==0){
			L_TCNT_1Val=TCNT1_Val-16;
			flag=TRUE;
		}
		*Data=0;
		uint8 Bits_Tx=0;
		GPIO_state State_RX_PIN=GPIO_HIGH;
		GPIO_state State_RX_NEW_PIN;
		Mcal_GPIO_READ(&Usart->RX,&State_RX_PIN);
		Mcal_GPIO_READ(&Usart->RX,&State_RX_NEW_PIN);
		if((State_RX_PIN==GPIO_HIGH&&State_RX_NEW_PIN==GPIO_LOW)||(State_RX_PIN==GPIO_LOW&&State_RX_NEW_PIN==GPIO_LOW))
		{
			Usart_Delay(L_TCNT_1Val);
			for (Bits_Tx = 0; Bits_Tx <= 7; Bits_Tx++) {
				Mcal_GPIO_READ(&Usart->RX,&State_RX_PIN);
				WRITE_BIT(*Data, Bits_Tx,State_RX_PIN);
				Usart_Delay(L_TCNT_1Val);
			}
		switch (Usart->No_STOP_Bits) {
			case NUM_STOP_BITS_1:
				Usart_Delay(L_TCNT_1Val);
				break;
			case NUM_STOP_BITS_2: //reduced complexity
				Usart_Delay(L_TCNT_1Val);
				Usart_Delay(L_TCNT_1Val);
				break;
			default:
				break;
		}
	}
	}else{
		Err_Status=E_NOK;
	}
	return Err_Status;
}

uint8 BitBanged_Usart_RXStr(BitBanged_USART *Usart,uint8 *Data)
{
	uint8 Err_Status=E_OK;
	if(Usart!=NULL&&Data!=NULL)
	{
		uint8 i=0;
		do{
			BitBanged_Usart_RX(Usart,Data+i);
		}while(*(Data+i)!='\0'&&*(Data+i)!='#');
	}else{
		Err_Status=E_NOK;
	}
	return Err_Status;
}

