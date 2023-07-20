/*
 * gpio.h
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_GPIO_H_
#define MCAL_MCAL_GPIO_H_

/**********Includes Section*******/
#include "mcal_std_types.h"
/*******User Defined Types*******/
typedef enum
{
	GPIO_PORTA,
	GPIO_PORTB,
	GPIO_PORTC,
	GPIO_PORTD,
}GPIO_Port;

typedef enum
{
	GPIO_PIN0,
	GPIO_PIN1,
	GPIO_PIN2,
	GPIO_PIN3,
	GPIO_PIN4,
	GPIO_PIN5,
	GPIO_PIN6,
	GPIO_PIN7,
	GPIO_PIN8,
}GPIO_PIN;

typedef enum
{
	GPIO_INPUT,
	GPIO_INPUT_PULLUP,
	GPIO_OUTPUT,
}GPIO_config;

typedef enum
{
	GPIO_LOW,
	GPIO_HIGH,
}GPIO_state;

typedef struct
{
	uint8 Port:2;
	uint8 PIN:3;
    uint8 Config:2;
	uint8 State:1;
}GPIO_PIN_t;

/*******Software Interfaces*******/
uint8 Mcal_GPIO_INIT(GPIO_PIN_t *Gpio);
uint8 Mcal_GPIO_WRITE(GPIO_PIN_t *Gpio,GPIO_state state);
uint8 Mcal_GPIO_TOGGLE(GPIO_PIN_t *Gpio);
uint8 Mcal_GPIO_READ(GPIO_PIN_t *Gpio,GPIO_state *state);

uint8 Mcal_GPIO_Init_PORT(GPIO_Port PORT_index,uint8 Val);
uint8 Mcal_GPIO_WRITE_PORT(GPIO_Port PORT_index,uint8 Val);
uint8 Mcal_GPIO_READ_PORT(GPIO_Port PORT_index,uint8 *Val);
#endif /* MCAL_MCAL_GPIO_H_ */
