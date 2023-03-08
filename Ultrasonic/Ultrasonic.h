/*
 * Ultrasonic.h
 *
 *  Created on: Mar 8, 2023
 *      Author: Seif pc
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_


/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
#include "Timer1.h"
/************Define Section******************/
#define Rising_edge 0
#define Falling_edge 1

/***********Macro Like Function**************/

/*************Defined Data types**************/
typedef struct
{
	GPIO_CONFIG_t Trigger_pin;
}Ultrasonic_t;

/*********Software Interfaces*******************/
Bool Ultrasonic_init(Ultrasonic_t * USensor);
Bool Ultrasonic_send_Trigger(Ultrasonic_t * USensor);
Bool Ultrasonic_read_distance_Interrupt(uint16 *Distance);


#endif /* ULTRASONIC_H_ */
