/*
 * keypad.h
 *
 *  Created on: Mar 12, 2023
 *      Author: Seif pc
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define NO_ROWS 4
#define NO_COLS 3
#define NOT_PRESSED 128
/***********Macro Like Function**************/

/*************Defined Data types**************/

typedef struct
{
	GPIO_CONFIG_t ROWS[4];
	GPIO_CONFIG_t COLS[3];
}Keypad_CONFIG_t_4_3;

/*********Software Interfaces*******************/
Bool keypad_init(Keypad_CONFIG_t_4_3 *keypad);
Bool keypad_pressed(Keypad_CONFIG_t_4_3 *keypad,uint8 *val);

#endif /* KEYPAD_H_ */
