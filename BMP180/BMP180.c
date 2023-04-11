/*
 * BMP180.c
 *
 *  Created on: Apr 11, 2023
 *      Author: Seif pc
 */
#include "BMP180.h"

static int32 pow_2(uint8 exp)
{
	int8 Counter=0;
	int32 Res=1;
	for(Counter=0;Counter<=exp-1;Counter++)
		Res*=2;
	return Res;
}

Bool BMP180_READ_TEMP_PRESSURE(BMP180_Config_t *BMP,float *Temp,float *Pres)
{
	/***************Variable Declaration******************/
	uint8 Arr1[3]={0};
	Bool E_Stat=E_OK;
	int16 AC1_val;
	int16 AC2_val;
	int16 AC3_val;
	uint16 AC4_val;
	uint16 AC5_val;
	uint16 AC6_val;
	int16 B1_val;
	int16 B2_val;
	int16 MB_val;
	int16 MC_val;
	int16 MD_val;
	int32 X1;
	int32 X2;
	int32 X3;
	int32 B3;
	int32 B5;
	int32 B6;
	uint32 B4,B7;
	int32 UT;
	int32 Temp_Val;
	int32 UP;
	int32 P;
	/**************Calibration values*******************/
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, AC1, Arr1, 2);
	AC1_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, AC2, Arr1, 2);
	AC2_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, AC3, Arr1, 2);
	AC3_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, AC4, Arr1, 2);
	AC4_val = (uint16) (Arr1[0] << 8) + Arr1[1];
	E_Stat=I2C_READ_NBYTES(BMP180_ADD,AC5,Arr1,2);
	AC5_val=(uint16)(Arr1[0]<<8)+Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, AC6, Arr1, 2);
	AC6_val = (uint16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, MB, Arr1, 2);
	MB_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, MC, Arr1, 2);
	MC_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, MD, Arr1, 2);
	MD_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, B1, Arr1, 2);
	B1_val = (int16) (Arr1[0] << 8) + Arr1[1];
	E_Stat = I2C_READ_NBYTES(BMP180_ADD, B2, Arr1, 2);
	B2_val = (int16) (Arr1[0] << 8) + Arr1[1];
	/***********Uncalibrated Temp Read******************/
	Arr1[0]=GET_TEMP_VAL;
	E_Stat=I2C_WRITE_NBYTES(BMP180_ADD,UT_REG_0,Arr1,1);
	_delay_ms(5);
	E_Stat=I2C_READ_NBYTES(BMP180_ADD,UT_REG_1,Arr1,2);
	UT=(int16)(Arr1[0]<<8)+Arr1[1];
	/***********Temperature Calculation****************/
	X1=(UT-AC6_val)*AC5_val/(int32)pow_2(15);
	X2=(int32)MC_val*pow_2(11)/(X1+MD);
	B5=X1+X2;
	Temp_Val=(B5+8)/(int32)pow_2(4);
	*Temp=Temp_Val*0.1;
	/***********Uncalibrated Pres Read******************/
	Arr1[0]=GET_PRES_VAL;
	Arr1[0]|=(BMP->OSS_val<<6);
	E_Stat=I2C_WRITE_NBYTES(BMP180_ADD,UT_REG_0,Arr1,1);
	switch(BMP->OSS_val)
	{
		case OSS_VAL:
			_delay_ms(5);
			break;
		case OSS_VAL_2:
			_delay_ms(10);
			break;
		case OSS_VAL_3:
			_delay_ms(15);
			break;
		case OSS_VAL_4:
			_delay_ms(28);
			break;
		default:
			E_Stat=E_NOK;
	}
	Arr1[0]=0;
	Arr1[1]=0;
	Arr1[2]=0;
	E_Stat=I2C_READ_NBYTES(BMP180_ADD,PT_REG_1,Arr1,3);
	UP=(((int32)Arr1[0]<<16)+((int32)Arr1[1]<<8)+Arr1[0])>>(8-BMP->OSS_val);
	/***********Pressure Calculation****************/
	B6=(int32)B5-4000;
	X1=((int32)B2*(B6*B6/pow_2(12)))/pow_2(11);
	X2=AC2_val*B6/(int32)pow_2(11);
	X3=X1+X2;
	B3=(((int32)(AC1_val*4+X3)<<BMP->OSS_val)+2)/4;
	X1=(int32)AC3_val*B6/pow_2(13);
	X2=(B1*((int32)B6*B6/pow_2(12)))/pow_2(16);
	X3=((int32)(X1+X2)+2)/4;
	B4=AC4_val*(uint32)(X3+32768)/pow_2(15);
	B7=((uint32)UP-B3)*(50000>>BMP->OSS_val);
	if(B7<(uint32)0x80000000)
		P=((int32)B7*2)/B4;
	else
		P=((int32)B7/B4)*2;
	X1=((int32)P/pow_2(8))*((int32)P/pow_2(8));
	X1=((int32)X1*3038)/(int32)pow_2(16);
	X2=((int32)-7357*P)/(int32)pow_2(16);
	P+=(int32)(X1+X2+3791)/pow_2(4);
	*Pres=P/(float)100;
	return E_Stat;
}

