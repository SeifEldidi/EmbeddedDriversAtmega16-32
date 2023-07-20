/*
 * Hal_Ultrasonic.h
 *
 *  Created on: May 30, 2023
 *      Author: Seif pc
 */

#ifndef ECAL_ECUAL_ULTRASONIC_H_
#define ECAL_ECUAL_ULTRASONIC_H_
/********Includes Section**************/
#include "../MCAL/Mcal_gpio.h"
#include "H:/AVR/Workspace/AvrDrivers_Enum_Way/MCAL/mcal_std_types.h"
#include "H:/AVR/Workspace/AvrDrivers_Enum_Way/MCAL/mcal_timer0.h"
/************Define Section******************/
#define Ultrasonic_max 11765
#define Ultrasonic_TimeOut 20000
#define OVF_MAX 95
/*********Software Interfaces*******************/
typedef struct
{
	GPIO_PIN_t Echo;
	GPIO_PIN_t Trigger;
}Ultrasonic_t;

uint8 Ecual_Ultrasonic_Read(Ultrasonic_t *Ut,uint16 *Result);
void Ecual_Ultrasonic_Init(Ultrasonic_t *Ut);

#endif /* ECAL_ECUAL_ULTRASONIC_H_ */
