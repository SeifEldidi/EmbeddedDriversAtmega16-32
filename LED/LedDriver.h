/*
 * LedDriver.h
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_
/************Includes Section**************/
#include "mcalStdtypes.h"

/************Define Section******************/
#define LED_OFF 0
#define LED_ON 1
#define PORT_MAX 4
#define PIN_MAX 8
/***********Macro Like Function**************/

/*************Defined Data types**************/

typedef struct
{
	uint8 LED_PORT:2;
	uint8 LED_PIN:3;
	uint8 LED_reserved:2;
	uint8 LED_LOGIC_LEVEL:1;
}LED_t;

/*********Software Interfaces*******************/
Bool LED_CONFIG(LED_t *GPIO_PIN);
Bool LED_on(LED_t *GPIO_PIN);
Bool LED_of(LED_t *GPIO_PIN);
Bool LED_TOGGLE(LED_t *GPIO_PIN);

#endif /* LEDDRIVER_H_ */
