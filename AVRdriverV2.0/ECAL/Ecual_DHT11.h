/*
 * HAL_DHT11.h
 *
 *  Created on: May 31, 2023
 *      Author: Seif pc
 */

#ifndef ECAL_ECUAL_DHT11_H_
#define ECAL_ECUAL_DHT11_H_
/********Includes Section**************/
#include "../MCAL/Mcal_gpio.h"
#include "H:/AVR/Workspace/AvrDrivers_Enum_Way/MCAL/mcal_std_types.h"
#include "H:/AVR/Workspace/AvrDrivers_Enum_Way/MCAL/mcal_timer0.h"
/************Define Section******************/
#define US_1 0
#define US_0_25 1
#define US_0_5 2

#define ms     0
#define ms_2	 1
#define ms_4  2
#define ms_8  3
#define ms_16 4
/*****************Macro Section**********/
#define TIMER0_SET() (TCCR0&0XF8)
/*********Software Interfaces*******************/
typedef struct
{
	GPIO_PIN_t data;
}dht11_t;

void  Ecual_DHT11_init(dht11_t  *Dht);
uint8  Ecual_DHT11_get_data(dht11_t  *Dht,uint8 *Arr);

#endif /* ECAL_ECUAL_DHT11_H_ */
