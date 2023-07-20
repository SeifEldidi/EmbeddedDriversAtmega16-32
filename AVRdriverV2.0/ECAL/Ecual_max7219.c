/*
 * Ecual_max7219.c
 *
 *  Created on: Jul 6, 2023
 *      Author: Seif pc
 */
#include "Ecual_max7219.h"


uint8 Data_arr[6]={0};
uint8 Data_arr_2[6]={0};
static uint8_t MAX721DIG[]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7f,0x7b,0x77,0x01,0x4E};

static void CLEAR_DISPLAY_Arr();
static uint8_t no_digits(int32_t data);
static Bool Max7219_Send_DArr(uint8 Address,uint8 Data,uint8 size);
static void CLEAR_DISPLAY();
static uint8 str_len(char *str);

Bool MAX7219_INIT()
{
	Bool ret=E_OK;
	MAX7219_SEND_Data(DECODE_MODE,EXTERNAL_DECODER);
	MAX7219_SEND_Data(INTENSITY_REG,INTENSITY_LEVEL7);
	MAX7219_SEND_Data(SCAN_LIM_REG,NO_DIGITS-1);
	MAX7219_SEND_Data(SHUTDOWN_REG,POWER_ON);
	MAX7219_SEND_Data(DISPLAY_TEST_REG,NORMAL_OP);
	CLEAR_DISPLAY();
	return ret;
}

Bool MAX7219_INIT_ARR()
{
	Bool ret = E_OK;
	Max7219_Send_DArr(DECODE_MODE, EXTERNAL_DECODER,2);
	Max7219_Send_DArr(INTENSITY_REG, INTENSITY_LEVEL7,2);
	Max7219_Send_DArr(SCAN_LIM_REG, NO_DIGITS - 1,2);
	Max7219_Send_DArr(SHUTDOWN_REG, POWER_ON,2);
	Max7219_Send_DArr(DISPLAY_TEST_REG, NORMAL_OP,2);
	return ret;
}

static Bool Max7219_Send_DArr(uint8 Address,uint8 Data,uint8 size)
{
	Bool ret=E_OK;
	uint8 Seg_no=0;
	SPI_CLEAR_SS();
	for(Seg_no=0;Seg_no<=size-1;Seg_no++)
	{
		SPI_SEND_BYTE(Address);
		SPI_SEND_BYTE(Data);
	}
	SPI_SET_SS();
	return ret;
}

Bool Max7219_Send_DArr_Ddata(uint8 Address,uint8 Data[],uint8 size,Bool *temp_flag,Bool *temp_flag_2)
{
	Bool ret = E_OK;
	uint8 Seg_no = 0;
	SPI_CLEAR_SS();
	for (Seg_no = 0; Seg_no <= size - 1; Seg_no++) {
		 SPI_SEND_BYTE(Address);
		if(*temp_flag==1&&Seg_no==size-1)
		{
			*temp_flag=0;
			 SPI_SEND_BYTE(MAX721DIG[Data[Seg_no]]|0x80);
		}
		else if(*temp_flag_2==1&&Seg_no==1)
		{
			*temp_flag_2 = 0;
			 SPI_SEND_BYTE(MAX721DIG[12]);
		}
		else
			 SPI_SEND_BYTE(MAX721DIG[Data[Seg_no]]);
	}
	SPI_SET_SS();
	return ret;
}

static void Convert_digs(int32 Num,uint8 Arr[])
{
	int8 size=0;
	while(Num!=0)
	{
		Arr[size++]=Num%10;
		Num/=10;
	}
}

void MAX7219Disp_float_num(float Val,Bool Temp_flag,uint32 Num)
{
	Bool flag=0,flag_2=0;
	int8 Counter=DIGIT_5;
	uint8 Arr[2];
	int32 int_val=Val*1000;

	Convert_digs(Num,Data_arr);
	Convert_digs(int_val,Data_arr_2);
	CLEAR_DISPLAY_Arr();
	for(Counter=DIGIT_5;Counter>=1;Counter--)
	{
		Arr[0] = Data_arr[DIGIT_5-Counter];
		Arr[1] = Data_arr_2[DIGIT_5-Counter];
		if(Counter==DIGIT_2)
			flag=1;
		if(Counter==DIGIT_5)
			flag_2=1;
		Max7219_Send_DArr_Ddata(Counter, Arr, 2,&flag,&flag_2);
	}
}

Bool MAX7219_SEND_Data(uint8 Address,uint8 Data)
{
	Bool ret=E_OK;
	SPI_CLEAR_SS();
	SPI_SEND_BYTE(Address);
	SPI_SEND_BYTE(Data);
	SPI_SET_SS();
	return ret;
}

static void CLEAR_DISPLAY()
{
	MAX7219_SEND_Data(DIGIT_7, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_6, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_5, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_4, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_3, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_2, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_1, MAX721DIG[0]);
	MAX7219_SEND_Data(DIGIT_0, MAX721DIG[0]);
}

static void CLEAR_DISPLAY_Arr()
{
	Max7219_Send_DArr(DIGIT_7, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_6, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_5, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_4, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_3, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_2, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_1, MAX721DIG[0],2);
	Max7219_Send_DArr(DIGIT_0, MAX721DIG[0],2);
}

static uint8_t no_digits(int32_t data)
{
	int len=0;
	while(data!=0)
	{
		data/=10;
		len++;
	}
	return len;
}


void MAX7219Disp_Number(int32_t data)
{
		Bool neg_flag=0;
		if(data==0)
			CLEAR_DISPLAY();
		else if(data<0)
		{
			neg_flag=1;
			data*=-1;
		}
		if(data!=0)
		{
			CLEAR_DISPLAY();
			uint8_t dig = no_digits(data);
			uint8_t Cnt = 0;
			if (neg_flag == 1) {
				MAX7219_SEND_Data(dig + 1, MAX721DIG[11]);
			}
			for (Cnt = 0; Cnt <= dig - 1; Cnt++) {
				MAX7219_SEND_Data(Cnt + 1, MAX721DIG[data % 10]);
				data /= 10;
			}
		}
}


static uint8 str_len(char *str)
{
	uint8 len=0;
	if(str!=NULL)
	{
		uint8 i=0;
		while(*(str+i)!='\0')
		{
			len++;
		    i++;
		}
	}
	return len;
}

void MAX7219Disp_string(char *str)
{
		if(str!=NULL)
		{
			uint8 i=0;
			uint8 len=str_len(str);
			len=len>8?8:len;
			while(*(str+i)!='\0')
			{
				uint8 data;
				if(*(str+i)>='0'&&*(str+i)<='9')
					data=*(str+i)-'0';
				else
					data=11;
				MAX7219_SEND_Data(len-i , MAX721DIG[data]);
				i++;
			}
		}
}

void MAX7219Disp_float(float Val,Bool Temp_flag)
{
	int32 int_val=Val*1000;
	CLEAR_DISPLAY();
	uint8_t dig = no_digits(int_val);
	uint8_t Cnt = 0;
	for (Cnt = 0; Cnt <= dig - 1; Cnt++) {
		if(Cnt==0&&Temp_flag)
			MAX7219_SEND_Data(Cnt + 1, MAX721DIG[12]);
		else if(Cnt==3)
			MAX7219_SEND_Data(Cnt + 1, (MAX721DIG[(int_val % 10)]|0x80));
		else
			MAX7219_SEND_Data(Cnt + 1, MAX721DIG[int_val % 10]);
		int_val /= 10;
	}
}
