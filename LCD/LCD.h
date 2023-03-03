/*
 * LCD.h
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#ifndef LCD_H_
#define LCD_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
/************Define Section******************/
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_INCREMENT_SHIFT_LEFT  0x07
#define LCD_DECREMENT_SHIFT_RIGHT 0x05
#define LCD_INCREMENT_SHIFT_OFF   0x06
#define LCD_DECREMENT_SHIFT_OFF   0x04
#define LCD_DISPLAY_OFF                    0x08
#define LCD_DISPLAY_ON_CURSOR_OFF          0x0C
#define LCD_DISPLAY_ON_CURSOR_ON_BLINK_OFF 0x0E
#define LCD_DISPLAY_ON_CURSOR_ON_BLINK_ON  0x0F
#define LCD_CURSOR_MOVE_RIGHT     0x14
#define LCD_CURSOR_MOVE_LEFT      0x10
#define LCD_SHIFT_LEFT            0x18
#define LCD_SHIFT_RIGHT           0x1C
#define LCD_4BIT_ONE_LINE         0x20
#define LCD_4BIT_TWO_LINE         0x28
#define LCD_8BIT_ONE_LINE         0x30
#define LCD_8BIT_TWO_LINE         0x38
#define LCD_SET_CGRAM_ADD         0x40
#define LCD_SET_DDRAM_ADD         0x80
#define LCD_4BIT_PINS 4
#define LCD_8BIT_PINS 8

#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4

#define ROW1_ADDRESS 0x00
#define ROW2_ADDRESS 0x40
#define ROW3_ADDRESS 0x14
#define ROW4_ADDRESS 0x54
/***********Macro Like Function**************/

/*************Defined Data types**************/
typedef struct
{
	GPIO_CONFIG_t LCD_RS;
	GPIO_CONFIG_t LCD_E;
	GPIO_CONFIG_t LCD_RW;
	GPIO_CONFIG_t LCD_Data[4];
}LCD_4BIT_t;

typedef struct
{
	GPIO_CONFIG_t LCD_RS;
	GPIO_CONFIG_t LCD_E;
	GPIO_CONFIG_t LCD_RW;
	GPIO_CONFIG_t LCD_Data[8];
}LCD_8BIT_t;

/*********Software Interfaces*******************/
Bool LCD_4BIT_INIT(LCD_4BIT_t *LCD);
Bool LCD_4BIT_SEND(LCD_4BIT_t *LCD,uint8 data);
Bool LCD_4BIT_SEND_POS(LCD_4BIT_t *LCD,uint8 data,uint8 row,uint8 col);
Bool LCD_4BIT_SEND_CMD(LCD_4BIT_t *LCD,uint8 Cmd);
Bool LCD_4BIT_SEND_STRING(LCD_4BIT_t *LCD,uint8 *str);
Bool LCD_4BIT_SEND_STRING_POS(LCD_4BIT_t *LCD,uint8 *str,uint8 row,uint8 col);
Bool LCD_4BIT_SET_POS(LCD_4BIT_t *LCD,uint8 row,uint8 col);
Bool LCD_4bit_send_custom_character(LCD_4BIT_t *LCD,uint8 row,uint8 col,const uint8 data[],uint8 mem_pos);

Bool LCD_8BIT_INIT(LCD_8BIT_t *LCD);
Bool LCD_8BIT_SEND(LCD_8BIT_t *LCD,uint8 data);
Bool LCD_8BIT_SEND_POS(LCD_8BIT_t *LCD,uint8 data,uint8 row,uint8 col);
Bool LCD_8BIT_SEND_CMD(LCD_8BIT_t *LCD,uint8 Cmd);
Bool LCD_8BIT_SEND_STRING(LCD_8BIT_t *LCD,uint8 *str);
Bool LCD_8BIT_SEND_STRING_POS(LCD_8BIT_t *LCD,uint8 *str,uint8 row,uint8 col);
Bool LCD_8BIT_SET_POS(LCD_8BIT_t *LCD,uint8 row,uint8 col);
Bool LCD_8bit_send_custom_character(LCD_8BIT_t *LCD,uint8 row,uint8 col,const uint8 data[],uint8 mem_pos);

void convert_byte_to_string(uint8 data,uint8 *str);
void convert_short_to_string(uint16 data,uint8 *str);
void convert_int_to_string(uint32 data,uint8 *str);


#endif /* LCD_H_ */
