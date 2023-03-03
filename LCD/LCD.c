/*
 * LCD.c
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */
#include "LCD.h"
#include <string.h>

static Bool LCD_SEND_4BITS(LCD_4BIT_t *LCD,uint8 CMD)
{
	Bool ret=E_OK;
	if(NULL==LCD)
		ret=E_NOK;
	else
	{
		uint8 LCD_PINS=0;
		for(LCD_PINS=0;LCD_PINS<=LCD_4BIT_PINS-1;LCD_PINS++)
			ret=GPIO_PIN_WRITE(&LCD->LCD_Data[LCD_PINS],((CMD>>LCD_PINS)&BIT_MASK));
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
			ret = GPIO_PIN_WRITE(&LCD->LCD_Data[LCD_PINS],
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
		ret=GPIO_PIN_WRITE(&(LCD->LCD_E),GPIO_HIGH);
		_delay_us(5);
		ret=GPIO_PIN_WRITE(&(LCD->LCD_E),GPIO_LOW);
	}
	return ret;
}

static Bool LCD_ENABLE_8BITS(LCD_8BIT_t *LCD)
{
	Bool ret = E_OK;
	if (NULL == LCD)
		ret = E_NOK;
	else {
		ret=GPIO_PIN_WRITE(&(LCD->LCD_E), GPIO_HIGH);
		_delay_us(5);
		ret=GPIO_PIN_WRITE(&(LCD->LCD_E), GPIO_LOW);
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
		ret=GPIO_PIN_CONFIG(&(LCD->LCD_E));
		ret=GPIO_PIN_CONFIG(&(LCD->LCD_RS));
		ret=GPIO_PIN_CONFIG(&(LCD->LCD_RW));
		for(LCD_PINS=0;LCD_PINS<=LCD_4BIT_PINS-1;LCD_PINS++)
			ret=GPIO_PIN_CONFIG(&LCD->LCD_Data[LCD_PINS]);
		for(LCD_PINS=0;LCD_PINS<=LCD_4BIT_PINS-1;LCD_PINS++)
			ret=GPIO_PIN_WRITE(&LCD->LCD_Data[LCD_PINS],GPIO_LOW);
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
		ret=GPIO_PIN_WRITE(&(LCD->LCD_RS),GPIO_HIGH);
		ret=GPIO_PIN_WRITE(&(LCD->LCD_RW),GPIO_LOW);
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
		ret=GPIO_PIN_WRITE(&(LCD->LCD_RS),GPIO_LOW);
		ret=GPIO_PIN_WRITE(&(LCD->LCD_RW),GPIO_LOW);
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
		ret = GPIO_PIN_CONFIG(&(LCD->LCD_E));
		ret = GPIO_PIN_CONFIG(&(LCD->LCD_RS));
		ret = GPIO_PIN_CONFIG(&(LCD->LCD_RW));
		for (LCD_PINS = 0; LCD_PINS <= LCD_4BIT_PINS - 1; LCD_PINS++)
			ret = GPIO_PIN_CONFIG(&LCD->LCD_Data[LCD_PINS]);
		for (LCD_PINS = 0; LCD_PINS <= LCD_4BIT_PINS - 1; LCD_PINS++)
			ret = GPIO_PIN_WRITE(&LCD->LCD_Data[LCD_PINS], GPIO_LOW);
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
		ret = GPIO_PIN_WRITE(&(LCD->LCD_RS), GPIO_HIGH);
		ret = GPIO_PIN_WRITE(&(LCD->LCD_RW), GPIO_LOW);
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
		ret = GPIO_PIN_WRITE(&(LCD->LCD_RS), GPIO_LOW);
		ret = GPIO_PIN_WRITE(&(LCD->LCD_RW), GPIO_LOW);
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
        sprintf(str,"%i",data);
        *(str+10)='\0';
    }
}

