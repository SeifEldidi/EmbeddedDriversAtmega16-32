/*
 * keypad.c
 *
 *  Created on: Mar 12, 2023
 *      Author: Seif pc
 */

#include "keypad.h"

#if NO_ROWS==4&&NO_COLS==3
static uint8 KEYPAD_4_3[NO_ROWS][NO_COLS]={
		{'1','2','3'},
	    {'4','5','6'},
	    {'7','8','9'},
	    {'*','0','#'}
};
#endif

Bool keypad_init(Keypad_CONFIG_t_4_3 *keypad)
{
	Bool ret=E_OK;
	if(NULL==keypad)
		ret=E_NOK;
	else
	{
		uint8 Iterator=0;
		for(Iterator=0;Iterator<=NO_COLS-1;Iterator++)
			GPIO_PIN_CONFIG(&keypad->COLS[Iterator]);
		for(Iterator=0;Iterator<=NO_ROWS-1;Iterator++)
			GPIO_PIN_CONFIG(&keypad->ROWS[Iterator]);
	}
	return ret;
}
Bool keypad_pressed(Keypad_CONFIG_t_4_3 *keypad,uint8 *val)
{
	Bool ret = E_OK;
	Bool state;
	uint8 NO_pressed=0;
	if (NULL == keypad)
		ret = E_NOK;
	else
	{
		uint8 o_rows=0,i_cols=0,i_rows;
		for(i_rows=0;i_rows<=NO_ROWS-1;i_rows++)
		{
			for(o_rows=0;o_rows<=NO_ROWS-1;o_rows++)
				GPIO_PIN_WRITE(&keypad->ROWS[o_rows],GPIO_LOW);
			GPIO_PIN_WRITE(&keypad->ROWS[i_rows],GPIO_HIGH);
			for(i_cols=0;i_cols<=NO_COLS-1;i_cols++)
			{
				ret=GPIO_PIN_READ(&keypad->COLS[i_cols],&state);
				if(state==GPIO_HIGH)
				{
					NO_pressed++;
					*val=KEYPAD_4_3[i_rows][i_cols];
				}
			}
		}
	}
	if(!NO_pressed)
		*val=NOT_PRESSED;
	return ret;
}
