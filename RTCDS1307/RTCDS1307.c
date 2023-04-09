/*
 * RTCDS1307.c
 *
 *  Created on: Apr 8, 2023
 *      Author: Seif pc
 */
#include "RTCDS1307.h"


Bool RTCDS1307_SET_TIME(uint8 Mode,uint8 Sec,uint8 Mins,uint8 Hrs)
{
	Bool E_stat=E_OK;
	uint8 Time_Arr[3];
	if((Sec>59)||(Mins>59))
		E_stat=E_NOK;
	if((Mode==MODE_24H&&Hrs>23)||(Mode!=MODE_24H&&Hrs>12))
		E_stat=E_NOK;
	if(E_stat==E_OK)
	{
		Time_Arr[0]=((Sec/10)<<4)+Sec%10;
		Time_Arr[1]=((Mins/10)<<4)+Mins%10;
		Time_Arr[2]=((Hrs/10)<<4)+Hrs%10;
		switch(Mode)
		{
			case MODE_AM:
				CLEAR_BIT(Time_Arr[2],AM_PM_MODE_POS);
				SET_BIT(Time_Arr[2],TW_TFR_MODE_POS);
				break;
			case MODE_PM:
				SET_BIT(Time_Arr[2], AM_PM_MODE_POS);
				SET_BIT(Time_Arr[2], TW_TFR_MODE_POS);
				break;
			case MODE_24H:
				CLEAR_BIT(Time_Arr[2],TW_TFR_MODE_POS);
		}
		E_stat=I2C_WRITE_NBYTES(RTCDS1307_ADD,RTCDS1307_SEC_REG,Time_Arr,3);
	}
	return E_stat;
}

Bool RTCDS1307_SET_DATE(uint8 Day,uint8 Date,uint8 Month,uint8 Year)
{
	Bool E_stat=E_OK;
	uint8 Date_Arr[4]={0};
	if((Date>31||Date<1)||(Month>12||Month<1)||(Year>99))
		E_stat=E_NOK;
	if(E_stat==E_OK)
	{
		Date_Arr[0]=Day;
		Date_Arr[1]=((Date/10)<<4)+Date%10;
		Date_Arr[2]=((Month/10)<<4)+Month%10;
		Date_Arr[3]=((Year/10)<<4)+Year%10;
		E_stat=I2C_WRITE_NBYTES(RTCDS1307_ADD, RTCDS1307_DAY_REG, Date_Arr, 4);
	}
	return E_stat;
}

Bool RTCDS1307_GET_TIME(uint8 *Time_Buff)
{
	Bool E_stat=E_OK;
	uint8 Temp;
	E_stat=I2C_READ_NBYTES(RTCDS1307_ADD,RTCDS1307_SEC_REG,Time_Buff,3);
	Time_Buff[0]=(Temp>>4)*10+(Time_Buff[0]&0x0F);
	Time_Buff[1]=((Time_Buff[1])>>4)*10+(Time_Buff[1]&0xF);
	Temp=Time_Buff[2]&0xE0;
	Time_Buff[2]&=0x1F;
	Time_Buff[2]=((Time_Buff[2])>>4)*10+(Time_Buff[2]&0x0F);
	return E_stat;
}

Bool RTCDS1307_GET_DATE(uint8 *Date_Buff)
{
	Bool E_stat=E_OK;
	E_stat=I2C_READ_NBYTES(RTCDS1307_ADD,RTCDS1307_DAY_REG,Date_Buff,4);
	*Date_Buff=*Date_Buff&0x07;
	*(Date_Buff+1)=(*(Date_Buff+1)>>4)*10+*(Date_Buff+1);
	*(Date_Buff+2)=(*(Date_Buff+2)>>4)*10+*(Date_Buff+2);
	*(Date_Buff+3)=(*(Date_Buff+3)>>4)*10+*(Date_Buff+3);
	return E_stat;
}

Bool RTCDS1307_TIME_STR(uint8 *Time_Buff)
{
	Bool E_stat = E_OK;
	Bool Hrs_flag=0;
	uint8 Temp_Time_Buff[3];
	E_stat = I2C_READ_NBYTES(RTCDS1307_ADD, RTCDS1307_SEC_REG,Temp_Time_Buff, 3);
	if(BIT_IS_SET(Temp_Time_Buff[2],TW_TFR_MODE_POS))
	{
		Time_Buff[0]=ZERO+((Temp_Time_Buff[2]&0x10)>>4);
		Time_Buff[1]=ZERO+(Temp_Time_Buff[2]&0x0F);
		if(BIT_IS_SET(Temp_Time_Buff[2],AM_PM_MODE_POS))
			Hrs_flag=1;//PM
		else
			Hrs_flag=2;//AM
	}
	else
	{
		Time_Buff[0]=ZERO+((Temp_Time_Buff[2]&0x30)>>4);
		Time_Buff[1]=ZERO+(Temp_Time_Buff[2]&0x0F);
	}
	Time_Buff[2] = ':';
	Time_Buff[3] = ZERO + (Temp_Time_Buff[1] >> 4);
	Time_Buff[4] = ZERO + (Temp_Time_Buff[1] & 0x0F);
	Time_Buff[5] = ':';
	Time_Buff[6] = ZERO + (Temp_Time_Buff[0] >> 4);
	Time_Buff[7] = ZERO + (Temp_Time_Buff[0] & 0x0F);
	if(Hrs_flag==1)
	{
		Time_Buff[8] = ' ';
		Time_Buff[9] = 'P';
		Time_Buff[10] = 'M';
		Time_Buff[11] = NULLChar;
	}
	else if(Hrs_flag==2)
	{
		Time_Buff[8] = ' ';
		Time_Buff[9] = 'A';
		Time_Buff[10] = 'M';
		Time_Buff[11] = NULLChar;
	}
	else{
		Time_Buff[8] = NULLChar;
	}
	return E_stat;
}

static void Strcpy(uint8 *Dest,int8 *Src)
{
	uint8 Cnt=0;
	while(*(Src+Cnt)!='\0')
	{
		*(Dest+Cnt)=*(Src+Cnt);
		Cnt++;
	}
}

Bool RTCDS1307_DATE_STR(uint8 *Date_Buff)
{
	Bool E_stat = E_OK;
	uint8 Temp_Date_Buff[4];
	E_stat = I2C_READ_NBYTES(RTCDS1307_ADD, RTCDS1307_DAY_REG, Temp_Date_Buff, 4);
	Date_Buff[0]=ZERO+(Temp_Date_Buff[1]>>4);
	Date_Buff[1]=ZERO+(Temp_Date_Buff[1]&0x0F);
	Date_Buff[2]='/';
	Date_Buff[3] = ZERO + (Temp_Date_Buff[2] >> 4);
	Date_Buff[4] = ZERO + (Temp_Date_Buff[2] & 0x0F);
	Date_Buff[5] = '/';
	Date_Buff[6] = '2';
	Date_Buff[7] = '0';
	Date_Buff[8] = ZERO + (Temp_Date_Buff[3] >> 4);
	Date_Buff[9] = ZERO + (Temp_Date_Buff[3] & 0x0F);
	Date_Buff[10] = '/';
	switch(Temp_Date_Buff[0])
	{
		case SUNDAY:
			Strcpy(Date_Buff+10," Sun");
			break;
		case MONDAY:
			Strcpy(Date_Buff+10," Mon");
			break;
		case TUESDAY:
			Strcpy(Date_Buff+10," Tue");
			break;
		case WEDNESDAY:
			Strcpy(Date_Buff+10," Wed");
			break;
		case THURSDAY:
			Strcpy(Date_Buff+10," Thur");
			break;
		case FRIDAY:
			Strcpy(Date_Buff+10," Fri");
			break;
		case SATURDAY:
			Strcpy(Date_Buff+10," Sat");
			break;
	}
	return E_stat;
}

