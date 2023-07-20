/*
 * Ecual_rtc.c
 *
 *  Created on: Jul 8, 2023
 *      Author: Seif pc
 */
#include "Ecual_rtc.h"

uint8 ECUAL_DS1307_Init()
{
	uint8 Err_status=E_OK;
	I2C_CONFIG_t I2C={
			.Prescaler_Freq=TWPS_PRESCALER_64,
			.SCL_FREQ=I2C_STANDARD_MODE,
	};
	Err_status=Mcal_I2C_init(&I2C);
	return Err_status;
}
uint8 ECUAL_DS1307_Set_Time(uint8 Time_Mode,uint8 AM_PM, uint8 Hrs,uint8 mins,uint8 secs)
{
	uint8 Err_status = E_OK;
	if ( Hrs > 23 || mins > 59 || secs > 59)
		Err_status = E_NOK;
	else {
		uint8 Buffer[3];
		Buffer[0] = ((secs / 10) << 4) | (secs % 10);
		Buffer[1] = ((mins / 10) << 4) | (mins % 10);
		Buffer[2] = ((Hrs / 10) << 4) | (Hrs % 10);
		switch (Time_Mode) {
		case TIME_12_HRS_MODE:
			Buffer[2] |= (TIME_12_HRS_MODE << TIME_HRS_MODE_SEL_POS);
			switch (AM_PM) {
			case TIME_AM:
				break;
			case TIME_PM:
				Buffer[2] |= (TIME_PM << TIME_AM_PM_SEL_POS);
				break;
			}
			break;
		case TIME_24_HRS_MODE:
			break;
		}
		Err_status = Mcal_I2C_write_buffer(RTC_DS1307_ADDRESS,
		RTC_DS1307_Seconds_REG, Buffer, 3);
	}
	return Err_status;
}

uint8 ECUAL_DS1307_Set_date(uint8 Year,uint8 Mnth,uint8 Date,uint8 Day)
{
	uint8 Err_status = E_OK;
	if (Mnth >12 || Day > 31 || Date > 7 || Year > 99)
		Err_status = E_NOK;
	else {
		uint8 Buffer[4];
		Buffer[0] = Day;
		Buffer[1] = ((Date / 10) << 4) | (Date % 10);
		Buffer[2] = ((Mnth / 10) << 4) | (Mnth % 10);
		Buffer[3] = ((Year / 10) << 4) | (Year % 10);
		Err_status = Mcal_I2C_write_buffer(RTC_DS1307_ADDRESS,
		RTC_DS1307_Day_REG, Buffer, 4);
	}
	return Err_status;
}

uint8 ECUAL_DS1307_Read_Time(uint8 *Buffer)
{
	uint8 E_Stat=E_OK;
	if(Buffer==NULL)
		E_Stat=E_NOK;
	else{
		uint8 Pos=0;
		uint8 Hrs;
		uint8 Mins;
		uint8 Secs;
		uint8 Time_24_flag=0x00;
		uint8 Temp_Buffer[3];
		Mcal_I2C_read_buffer(RTC_DS1307_ADDRESS,RTC_DS1307_Seconds_REG,Temp_Buffer,3);
		Secs=Temp_Buffer[0];
		Mins=Temp_Buffer[1];
		Hrs=Temp_Buffer[2];
		if(Hrs&(TIME_12_HRS_MODE<<TIME_HRS_MODE_SEL_POS))
		{
			Time_24_flag|=(1<<0);
			Hrs&=~(TIME_12_HRS_MODE<<TIME_HRS_MODE_SEL_POS);
			if(Hrs&(TIME_PM<<TIME_AM_PM_SEL_POS))
			{
				Hrs&=~(TIME_PM<<TIME_AM_PM_SEL_POS);
				Time_24_flag|=(1<<1);
			}
		}
		Buffer[Pos++]=(Hrs>>4)+'0';
		Buffer[Pos++]=(Hrs&0x0F)+'0';
		Buffer[Pos++]=':';
		Buffer[Pos++]=(Mins>>4)+'0';
		Buffer[Pos++]=(Mins&0x0F)+'0';
		Buffer[Pos++]=':';
		Buffer[Pos++] = (Secs >>4) + '0';
		Buffer[Pos++] = (Secs&0x0F) + '0';
		if(Time_24_flag&0x01)
		{
			if(Time_24_flag&0x02)
			{
				Buffer[Pos++] = 'P';
				Buffer[Pos++] = 'M';
			}
			else{
				Buffer[Pos++] = 'A';
				Buffer[Pos++] = 'M';
			}
		}
	}
	return E_Stat;
}

