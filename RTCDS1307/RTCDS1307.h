/*
 * RTCDS1307.h
 *
 *  Created on: Apr 8, 2023
 *      Author: Seif pc
 */

#ifndef RTCDS1307_H_
#define RTCDS1307_H_

#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
#include "I2C.h"
/************Define Section******************/
#define RTCDS1307_ADD     0xD0
#define	RTCDS1307_SEC_REG 0x00
#define RTCDS1307_MIN_REG 0x01
#define RTCDS1307_HRS_REG 0x02
#define RTCDS1307_DAY_REG 0x03
#define RTCDS1307_DAT_REG 0x04
#define RTCDS1307_MNT_REG 0x05
#define RTCDS1307_YER_REG 0x06
#define RTCDS1307_CNT_REG 0x07

#define SUNDAY 1
#define MONDAY 2
#define TUESDAY 3
#define WEDNESDAY 4
#define THURSDAY 5
#define FRIDAY 6
#define SATURDAY 7

#define MODE_24H 0
#define MODE_AM  1
#define MODE_PM  2
#define AM_PM_MODE_POS 5
#define TW_TFR_MODE_POS 6
/*********Software Interfaces*******************/
Bool RTCDS1307_SET_TIME(uint8 Mode,uint8 Sec,uint8 Mins,uint8 Hrs);
Bool RTCDS1307_SET_DATE(uint8 Day,uint8 Date,uint8 Month,uint8 Year);

Bool RTCDS1307_GET_TIME(uint8 *Time_Buff);
Bool RTCDS1307_GET_DATA(uint8 *Date_Buff);


#endif /* RTCDS1307_H_ */
