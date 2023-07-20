/*
 * gpio.c
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */
#include "Mcal_gpio.h"

/**
 * Array of pointers to registers
 */
uint8 *DDRX_REGS[]={(uint8 *)&DDRA,(uint8 *)&DDRB,(uint8 *)&DDRC,(uint8 *)&DDRD};
uint8 *PORTX_REGS[]={(uint8 *)&PORTA,(uint8 *)&PORTB,(uint8 *)&PORTC,(uint8 *)&PORTD};
uint8 *PINX_REGS[]={(uint8 *)&PINA,(uint8 *)&PINB,(uint8 *)&PINC,(uint8 *)&PIND};

uint8 Mcal_GPIO_INIT(GPIO_PIN_t *Gpio)
{
	uint8 Err_state=E_OK;
	if(Gpio->Port>GPIO_PORTD||Gpio->PIN>GPIO_PIN8||Gpio->Config>GPIO_OUTPUT)
		Err_state=E_NOK;
	else
	{
		if(Gpio->Config==GPIO_INPUT)
			CLEAR_BIT(*DDRX_REGS[Gpio->Port],Gpio->PIN);
		else if(Gpio->Config==GPIO_INPUT_PULLUP)
		{
			CLEAR_BIT(*DDRX_REGS[Gpio->Port],Gpio->PIN);
			SET_BIT(*PORTX_REGS[Gpio->Port],Gpio->PIN);
		}
		else if(Gpio->Config==GPIO_OUTPUT)
		{
			SET_BIT(*DDRX_REGS[Gpio->Port],Gpio->PIN);
			switch(Gpio->State)
			{
				case GPIO_LOW:
					CLEAR_BIT(*PORTX_REGS[Gpio->Port],Gpio->PIN);
					break;
				case GPIO_HIGH:
					SET_BIT(*PORTX_REGS[Gpio->Port],Gpio->PIN);
					break;
			}
		}
		else
			Err_state=E_NOK;
	}
	return Err_state;
}

uint8 Mcal_GPIO_WRITE(GPIO_PIN_t *Gpio,GPIO_state state)
{
	uint8 Err_state=E_OK;
	if(Gpio->Port>GPIO_PORTD||Gpio->PIN>GPIO_PIN8)
		Err_state=E_NOK;
	else
		WRITE_BIT(*PORTX_REGS[Gpio->Port],Gpio->PIN,state);
	return Err_state;
}

uint8 Mcal_GPIO_READ(GPIO_PIN_t *Gpio,GPIO_state *state)
{
	uint8 Err_state=E_OK;
	if(Gpio->Port>GPIO_PORTD||Gpio->PIN>GPIO_PIN8)
		Err_state=E_NOK;
	else
	{
		*state=READ_BIT(*PINX_REGS[Gpio->Port],Gpio->PIN);
	}
	return Err_state;
}

uint8 Mcal_GPIO_TOGGLE(GPIO_PIN_t *Gpio)
{
	uint8 Err_state = E_OK;
	if (Gpio->Port > GPIO_PORTD || Gpio->PIN > GPIO_PIN8)
		Err_state = E_NOK;
	else
		TOGGLE_BIT(*PORTX_REGS[Gpio->Port],Gpio->PIN);
	return Err_state;
}

uint8 Mcal_GPIO_Init_PORT(GPIO_Port PORT_index,uint8 Val)
{
	uint8 Err_state = E_OK;
	switch (PORT_index) {
		case GPIO_PORTA:
			DDRA = Val;
			break;
		case GPIO_PORTB:
			DDRB = Val;
			break;
		case GPIO_PORTC:
			DDRC = Val;
			break;
		case GPIO_PORTD:
			DDRD = Val;
			break;
		default:
			Err_state = E_NOK;
	}
	return Err_state;
}

uint8 Mcal_GPIO_WRITE_PORT(GPIO_Port PORT_index,uint8 Val)
{
	uint8 Err_state=E_OK;
	switch(PORT_index)
	{
		case GPIO_PORTA:
			PORTA=Val;
			break;
		case GPIO_PORTB:
			PORTB=Val;
			break;
		case GPIO_PORTC:
			PORTC=Val;
			break;
		case GPIO_PORTD:
			PORTD=Val;
			break;
		default:
			Err_state=E_NOK;
	}
	return Err_state;
}

uint8 Mcal_GPIO_READ_PORT(GPIO_Port PORT_index,uint8 *Val)
{
	uint8 Err_state = E_OK;
	switch (PORT_index) {
		case GPIO_PORTA:
			*Val=PINA;
			break;
		case GPIO_PORTB:
			*Val=PINB;
			break;
		case GPIO_PORTC:
			*Val=PINC;
			break;
		case GPIO_PORTD:
			*Val=PIND;
			break;
		default:
			Err_state = E_NOK;
	}
	return Err_state;
}
