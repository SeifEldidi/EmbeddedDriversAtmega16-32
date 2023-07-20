/*
 * mcal_std_types.h
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_STD_TYPES_H_
#define MCAL_MCAL_STD_TYPES_H_
/***********Includes Section*********/
#include "MCAL_MACROS.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/***********Define Section***********/
#define F_CPU 16000000UL
#define TRUE 0x01
#define FALSE 0X00
#define E_OK TRUE
#define E_NOK FALSE
#define MCU_8BIT_ 	TRUE
#define MCU_16BIT_ FALSE
#define MCU_32BIT_ FALSE
#define NULL (void *)0

#if MCU_8BIT_==TRUE
typedef unsigned char uint8;
typedef uint8 Bool;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef char int8;
typedef short int16;
typedef long  int32;
typedef float float32;
#elif MCU_16BIT_==TRUE

#elif MCU_32BIT_==TRUE

#endif



#endif /* MCAL_MCAL_STD_TYPES_H_ */
