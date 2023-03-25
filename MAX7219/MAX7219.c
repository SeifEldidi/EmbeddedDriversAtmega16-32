/*
 * MAX7219.c
 *
 *  Created on: Mar 21, 2023
 *      Author: Seif pc
 */
#include "MAX7219.h"

static uint8_t MAX721DIG[]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7f,0x7b,0x77,0x01};

Bool MAX7219_INIT()
{
	Bool ret=E_OK;
	MAX7219_SEND_Data(DECODE_MODE,EXTERNAL_DECODER);
	MAX7219_SEND_Data(INTENSITY_REG,INTENSITY_LEVEL7);
	MAX7219_SEND_Data(SCAN_LIM_REG,NO_DIGITS-1);
	MAX7219_SEND_Data(SHUTDOWN_REG,POWER_ON);
	MAX7219_SEND_Data(DISPLAY_TEST_REG,NORMAL_OP);
	return ret;
}

Bool MAX7219_SEND_Data(uint8 Address,uint8 Data)
{
	Bool ret;
	SPI_CLEAR_SS();
	ret=SPI_SEND_blocking(Address);
	ret=SPI_SEND_blocking(Data);
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

