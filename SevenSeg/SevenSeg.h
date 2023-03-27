/*
 * SevenSeg.h
 *
 *  Created on: Mar 12, 2023
 *      Author: Seif pc
 */

#ifndef SEVENSEG_H_
#define SEVENSEG_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define NO_SEGS 4
#define DIGS 4
/***********Macro Like Function**************/

/*************Defined Data types**************/

typedef struct
{
	GPIO_CONFIG_t dig[4];
	GPIO_CONFIG_t Control[NO_SEGS];
}SEVEN_SEG_7;

/*********Software Interfaces*******************/
Bool SEVEN_SEG_init(SEVEN_SEG_7 *Seg);
Bool SEVEN_SEG_display(SEVEN_SEG_7 *Seg,uint16 val,uint8 *Timerflag);



#endif /* SEVENSEG_H_ */
