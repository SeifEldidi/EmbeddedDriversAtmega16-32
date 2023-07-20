/*
 * lcd.c
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */
#include "Ecual_lcd.h"

static Bool LCD_SEND_4BITS(LCD_4BIT_t *LCD,uint8 CMD)
{
	Bool ret=E_OK;
	if(NULL==LCD)
		ret=E_NOK;
	else
	{
		uint8 LCD_PINS=0;
		for(LCD_PINS=0;LCD_PINS<=LCD_4BIT_PINS-1;LCD_PINS++)
			ret=Mcal_GPIO_WRITE(&LCD->LCD_Data[LCD_PINS],((CMD>>LCD_PINS)&BIT_MASK));
	}
	return ret;
}

static Bool LCD_SEND_8BITS(LCD_8BIT_t *LCD,uint8 CMD)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		uint8 LCD_PINS = 0;
		for (LCD_PINS = 0; LCD_PINS <= LCD_8BIT_PINS-1; LCD_PINS++)
			ret = Mcal_GPIO_WRITE(&LCD->LCD_Data[LCD_PINS],
					((CMD >> LCD_PINS) & BIT_MASK));
	}
	return ret;
}

static Bool LCD_ENABLE_4BITS(LCD_4BIT_t *LCD)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_E),GPIO_HIGH);
		_delay_us(5);
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_E),GPIO_LOW);
	}
	return ret;
}

static Bool LCD_ENABLE_8BITS(LCD_8BIT_t *LCD)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_E), GPIO_HIGH);
		_delay_us(5);
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_E), GPIO_LOW);
	}
	return ret;
}

Bool LCD_4BIT_INIT(LCD_4BIT_t *LCD)
{
	Bool ret=E_OK;
	if(NULL==LCD)
		ret=E_NOK;
	else
	{
		uint8 LCD_PINS=0;
		ret=Mcal_GPIO_INIT(&(LCD->LCD_E));
		ret=Mcal_GPIO_INIT(&(LCD->LCD_RS));
		ret=Mcal_GPIO_INIT(&(LCD->LCD_RW));
		for(LCD_PINS=0;LCD_PINS<=LCD_4BIT_PINS-1;LCD_PINS++)
			ret=Mcal_GPIO_INIT(&LCD->LCD_Data[LCD_PINS]);
		for(LCD_PINS=0;LCD_PINS<=LCD_4BIT_PINS-1;LCD_PINS++)
			ret=Mcal_GPIO_WRITE(&LCD->LCD_Data[LCD_PINS],GPIO_LOW);
		_delay_ms(20);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_8BIT_TWO_LINE);
		_delay_ms(5);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_8BIT_TWO_LINE);
		_delay_us(150);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_8BIT_TWO_LINE);

		ret=LCD_4BIT_SEND_CMD(LCD,LCD_CLEAR_DISPLAY);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_RETURN_HOME);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_INCREMENT_SHIFT_OFF);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_DISPLAY_ON_CURSOR_OFF);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_4BIT_TWO_LINE);
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_SET_DDRAM_ADD);
	}
	return ret;
}

Bool LCD_4BIT_SEND(LCD_4BIT_t *LCD,uint8 data)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else
	{
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_RS),GPIO_HIGH);
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_RW),GPIO_LOW);
		ret = LCD_SEND_4BITS(LCD, data >> LCD_4BIT_PINS);
		ret = LCD_ENABLE_4BITS(LCD);
		ret = LCD_SEND_4BITS(LCD, data & 0x0F);
		ret = LCD_ENABLE_4BITS(LCD);
	}
	return ret;
}

Bool LCD_4BIT_SET_POS(LCD_4BIT_t *LCD,uint8 row,uint8 col)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		switch(row)
		{
			case ROW1:
				ret=LCD_4BIT_SEND_CMD(LCD,(LCD_SET_DDRAM_ADD|ROW1_ADDRESS)+col);
				break;
			case ROW2:
				ret=LCD_4BIT_SEND_CMD(LCD,(LCD_SET_DDRAM_ADD|ROW2_ADDRESS)+col);
				break;
			case ROW3:
				ret=LCD_4BIT_SEND_CMD(LCD,(LCD_SET_DDRAM_ADD|ROW3_ADDRESS)+col);
				break;
			case ROW4:
				ret=LCD_4BIT_SEND_CMD(LCD,(LCD_SET_DDRAM_ADD|ROW4_ADDRESS)+col);
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

Bool LCD_4BIT_SEND_CMD(LCD_4BIT_t *LCD,uint8 Cmd)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_RS),GPIO_LOW);
		ret=Mcal_GPIO_WRITE(&(LCD->LCD_RW),GPIO_LOW);
		ret=LCD_SEND_4BITS(LCD,Cmd>>LCD_4BIT_PINS);
		ret=LCD_ENABLE_4BITS(LCD);
		ret=LCD_SEND_4BITS(LCD,Cmd&0x0F);
		ret=LCD_ENABLE_4BITS(LCD);
	}
	return ret;
}

Bool LCD_4BIT_SEND_STRING(LCD_4BIT_t *LCD,uint8 *str)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		uint8 Char_pos=0;
		while(*(str+Char_pos)!='\0')
		{
			ret=LCD_4BIT_SEND(LCD,*(str+Char_pos));
			Char_pos++;
		}
	}
	return ret;
}

float Arg;
uint8 Str[25];

static void float_string(float Num,int8 *Str)
{
	uint8 Counter=7;
	uint8 LEN=0;
	int32 Int_part=(int32)Num;
	int32 Float_part=((float)(Num)-(int32)Num)*1000000;
	while(Int_part!=0)
	{
		*(Str+Counter++)=Int_part%10+'0';
		Int_part/=10;
		LEN++;
	}
	*(Str+Counter)='\0';
	Counter=0;
	while(Float_part!=0&&Counter<6)
	{
		*(Str+Counter++)=Float_part%10+'0';
		Float_part/=10;
		LEN++;
	}
	*(Str+6)='.';
	Counter=0;
	while(Counter<LEN/2)
	{
		uint8 Temp=*(Str+Counter);
		*(Str+Counter)=*(Str+LEN-Counter);
		*(Str+LEN-Counter)=Temp;
		Counter++;
	}
}

Bool LCD_PRINT_4BITS(LCD_4BIT_t *LCD,uint8 row,uint8 col,uint8 *str,...)
{
	Bool E_stat=E_OK;
	Bool flag=0;
	va_list valist;
	va_start(valist, str);
	uint8 Counter=0;
	E_stat=LCD_4BIT_SET_POS(LCD,row,col);
	while((*(str+Counter))&&(Counter<=MAX_CHARS))
	{
		if(Counter==16)
			E_stat=LCD_4BIT_SET_POS(LCD,2,0);
		if(*(str+Counter)!='%')
			E_stat=LCD_4BIT_SEND(LCD,*(str+Counter));
		else if(*(str+Counter)=='%')
		{
			Counter++;
			flag=1;
			uint8 Str[11];
			if(*(str+Counter)=='d')
			{
				int16 Arg=va_arg(valist,int);
				convert_short_to_string(Arg,Str);
				LCD_4BIT_SEND_STRING(LCD,Str);
			}
			else if(*(str+Counter)=='c')
			{
				int8 Arg = va_arg(valist, int);
				LCD_4BIT_SEND(LCD,Arg);
			}
			else if(*(str+Counter)=='l')
			{
				int32 Arg = va_arg(valist, int);
				convert_int_to_string(Arg, Str);
				LCD_4BIT_SEND_STRING(LCD, Str);
			}
			else if(*(str+Counter)=='f')
			{
				Arg = va_arg(valist, double);
				float_string(Arg,Str);
				LCD_4BIT_SEND_STRING(LCD, Str);
			}
		}
		else
			LCD_4BIT_SEND(LCD,*(str+Counter));
		Counter++;
	}
	return E_stat;
}

Bool LCD_4BIT_SEND_STRING_POS(LCD_4BIT_t *LCD,uint8 *str,uint8 row,uint8 col)
{
	Bool ret = E_OK;
	ret=LCD_4BIT_SET_POS(LCD,row,col);
	ret=LCD_4BIT_SEND_STRING(LCD,str);
	return ret;
}

Bool LCD_4BIT_SEND_POS(LCD_4BIT_t *LCD,uint8 data,uint8 row,uint8 col)
{
	Bool ret = E_OK;
	ret=LCD_4BIT_SET_POS(LCD,row,col);
	ret=LCD_4BIT_SEND(LCD,data);
	return ret;
}

Bool LCD_4bit_send_custom_character(LCD_4BIT_t *LCD,uint8 row,uint8 col,const uint8 data[],uint8 mem_pos)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		uint8 Byte_pos=0;
		ret=LCD_4BIT_SEND_CMD(LCD,LCD_SET_CGRAM_ADD+8*(mem_pos));
		for(Byte_pos=0;Byte_pos<=7;Byte_pos++)
			ret=LCD_4BIT_SEND(LCD,data[Byte_pos]);
		ret=LCD_4BIT_SEND_POS(LCD,mem_pos,row,col);
	}
	return ret;
}

Bool LCD_8BIT_INIT(LCD_8BIT_t *LCD)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		uint8 LCD_PINS = 0;
		ret = Mcal_GPIO_INIT(&(LCD->LCD_E));
		ret = Mcal_GPIO_INIT(&(LCD->LCD_RS));
		ret = Mcal_GPIO_INIT(&(LCD->LCD_RW));
		for (LCD_PINS = 0; LCD_PINS <= LCD_4BIT_PINS - 1; LCD_PINS++)
			ret = Mcal_GPIO_INIT(&LCD->LCD_Data[LCD_PINS]);
		for (LCD_PINS = 0; LCD_PINS <= LCD_4BIT_PINS - 1; LCD_PINS++)
			ret = Mcal_GPIO_WRITE(&LCD->LCD_Data[LCD_PINS], GPIO_LOW);
		_delay_ms(20);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_8BIT_TWO_LINE);
		_delay_ms(5);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_8BIT_TWO_LINE);
		_delay_us(150);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_8BIT_TWO_LINE);

		ret = LCD_8BIT_SEND_CMD(LCD, LCD_CLEAR_DISPLAY);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_RETURN_HOME);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_INCREMENT_SHIFT_OFF);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_DISPLAY_ON_CURSOR_OFF);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_8BIT_TWO_LINE);
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_SET_DDRAM_ADD);
	}
	return ret;
}

Bool LCD_8BIT_SEND(LCD_8BIT_t *LCD,uint8 data)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		ret = Mcal_GPIO_WRITE(&(LCD->LCD_RS), GPIO_HIGH);
		ret = Mcal_GPIO_WRITE(&(LCD->LCD_RW), GPIO_LOW);
		ret = LCD_SEND_8BITS(LCD, data);
		ret = LCD_ENABLE_8BITS(LCD);
	}
	return ret;
}

Bool LCD_8BIT_SEND_POS(LCD_8BIT_t *LCD,uint8 data,uint8 row,uint8 col)
{
	Bool ret = E_OK;
	ret = LCD_8BIT_SET_POS(LCD, row, col);
	ret = LCD_8BIT_SEND(LCD, data);
	return ret;
}
Bool LCD_8BIT_SEND_CMD(LCD_8BIT_t *LCD,uint8 Cmd)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		ret = Mcal_GPIO_WRITE(&(LCD->LCD_RS), GPIO_LOW);
		ret = Mcal_GPIO_WRITE(&(LCD->LCD_RW), GPIO_LOW);
		ret = LCD_SEND_8BITS(LCD, Cmd);
		ret = LCD_ENABLE_8BITS(LCD);
	}
	return ret;
}
Bool LCD_8BIT_SEND_STRING(LCD_8BIT_t *LCD,uint8 *str)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		uint8 Char_pos = 0;
		while (*(str + Char_pos) != '\0') {
			ret = LCD_8BIT_SEND(LCD, *(str + Char_pos));
			Char_pos++;
		}
	}
	return ret;
}

Bool LCD_8BIT_SEND_STRING_POS(LCD_8BIT_t *LCD,uint8 *str,uint8 row,uint8 col)
{
	Bool ret = E_OK;
	ret = LCD_8BIT_SET_POS(LCD, row, col);
	ret = LCD_8BIT_SEND_STRING(LCD, str);
	return ret;
}

Bool LCD_8BIT_SET_POS(LCD_8BIT_t *LCD,uint8 row,uint8 col)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		switch (row) {
		case ROW1:
			ret = LCD_8BIT_SEND_CMD(LCD,
					(LCD_SET_DDRAM_ADD | ROW1_ADDRESS) + col);
			break;
		case ROW2:
			ret = LCD_8BIT_SEND_CMD(LCD,
					(LCD_SET_DDRAM_ADD | ROW2_ADDRESS) + col);
			break;
		case ROW3:
			ret = LCD_8BIT_SEND_CMD(LCD,
					(LCD_SET_DDRAM_ADD | ROW3_ADDRESS) + col);
			break;
		case ROW4:
			ret = LCD_8BIT_SEND_CMD(LCD,
					(LCD_SET_DDRAM_ADD | ROW4_ADDRESS) + col);
			break;
		default:
			ret = E_NOK;
		}
	}
	return ret;
}
Bool LCD_8bit_send_custom_character(LCD_8BIT_t *LCD,uint8 row,uint8 col,const uint8 data[],uint8 mem_pos)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		uint8 Byte_pos = 0;
		ret = LCD_8BIT_SEND_CMD(LCD, LCD_SET_CGRAM_ADD + 8 * (mem_pos));
		for (Byte_pos = 0; Byte_pos <= 7; Byte_pos++)
			ret = LCD_8BIT_SEND(LCD, data[Byte_pos]);
		ret = LCD_8BIT_SEND_POS(LCD, mem_pos, row, col);
	}
	return ret;
}

void convert_byte_to_string(uint8 data,uint8 *str)
{
    if(NULL==str)
        return;
    else
    {
        memset(str,'\0',4);
        sprintf(str,"%i",data);
        strncat(str,"     ",4);
        *(str+3)='\0';
    }
}
void convert_short_to_string(uint16 data,uint8 *str)
{
    if(NULL==str)
        return;
    else
    {
        memset(str,'\0',6);
        sprintf(str,"%i",data);
        strncat(str,"  ",2);
        *(str+5)='\0';
    }
}
void convert_int_to_string(uint32 data,uint8 *str)
{
    if(NULL==str)
        return;
    else
    {
        memset(str,'\0',11);
        sprintf(str,"%x",data);
        *(str+10)='\0';
    }
}

