/*
 * Ecual_rtc.h
 *
 *  Created on: Jul 8, 2023
 *      Author: Seif pc
 */

#ifndef ECAL_ECUAL_RTC_H_
#define ECAL_ECUAL_RTC_H_
/********Includes Section**********/
#include "../MCAL/Mcal_I2c.h"
/******Defines Section*******/
#define RTC_DS1307_ADDRESS 0xD0
#define RTC_DS1307_Seconds_REG 0x00
#define RTC_DS1307_Minutes_REG 0x01
#define RTC_DS1307_Hours_REG 0x02
#define RTC_DS1307_Day_REG 0x03
#define RTC_DS1307_Date_REG 0x04
#define RTC_DS1307_Month_REG 0x05
#define RTC_DS1307_Year_REG 0x06
#define RTC_DS1307_Control_REG 0x07

#define Sun  0x00
#define Mon 0x01
#define Tue   0x02
#define Wed 0x03
#define Thu  0x04
#define Fri    0x05
#define Sat   0x06


#define TIME_AM 0
#define TIME_PM 1
#define TIME_AM_PM_SEL_POS 5
#define TIME_24_HRS_MODE  0x00
#define TIME_12_HRS_MODE  0x01
#define TIME_HRS_MODE_SEL_POS 6
/********Software Interfaces**********/
uint8 ECUAL_DS1307_Init();
uint8 ECUAL_DS1307_Set_Time(uint8 Time_Mode,uint8 AM_PM, uint8 Hrs,uint8 mins,uint8 secs);
uint8 ECUAL_DS1307_Set_date(uint8 Year,uint8 Mnth,uint8 Day,uint8 Date);
uint8 ECUAL_DS1307_Read_Time(uint8 *Buffer);
uint8 ECUAL_DS1307_Read_Date(uint8 *Buffer);

#endif /* ECAL_ECUAL_RTC_H_ */
