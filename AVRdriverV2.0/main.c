/*
 * main.c
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */
#include "main.h"
#include <stdio.h>
#include <string.h>


BitBanged_USART Usart={
		.BaudRate=9600,
		.No_STOP_Bits=NUM_STOP_BITS_1,
		.Parity_bits=Parity_NO,
		.TX.Port=GPIO_PORTB,
		.TX.PIN=PIN0,
		.RX.Port=GPIO_PORTB,
		.RX.PIN=PIN1,
};


uint8 Data='x';

int main()
{
	DDRD=(1<<PD0);
	BitBanged_Usart_init(&Usart);
	while(1)
	{
		BitBanged_Usart_RX(&Usart,&Data);
		if(Data=='b')
		{
			PORTD^=(1<<PD0);
		}
	}
}

void main_init()
{

}



















