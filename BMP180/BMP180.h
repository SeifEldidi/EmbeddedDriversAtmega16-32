/*
 * BMP180.h
 *
 *  Created on: Apr 11, 2023
 *      Author: Seif pc
 */

#ifndef BMP180_H_
#define BMP180_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "stdlibraries.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
#include "I2C.h"
/************Define Section******************/
#define AC1  0xAA  //lsb address
#define AC2  0xAC
#define AC3  0xAE
#define AC4  0xB0
#define AC5  0xB2
#define AC6  0xB4
#define B1   0xB6
#define B2   0xB8
#define MB   0xBA
#define MC   0xBC
#define MD   0xBE

#define BMP180_ADD 0b11101110

#define UT_REG_0 0xF4
#define UT_REG_1 0xF6
#define UT_REG_2 0xF7

#define PT_REG_1 0xF6
#define PT_REG_2 0xF7
#define PT_REG_3 0xF8

#define GET_TEMP_VAL 0x2E
#define GET_PRES_VAL 0x34

#define OSS_VAL 0
#define OSS_VAL_2 1
#define OSS_VAL_3 2
#define OSS_VAL_4 3
/*************Defined Data types**************/
typedef enum
{
	ULTRA_LOW_POWER,
	STANDARD,
	HIGH_RESOLUTION,
	ULTRA_HIGH_RESOLUTION
}BMP180_Sampling_t;

typedef struct
{
	BMP180_Sampling_t OSS_val;
}BMP180_Config_t;
/*********Software Interfaces*******************/
Bool BMP180_READ_TEMP_PRESSURE(BMP180_Config_t *BMP,float *Temp,float *Pres);

#endif /* BMP180_H_ */
