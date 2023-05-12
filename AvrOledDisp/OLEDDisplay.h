/*
 * OLEDDisplay.h
 *
 *  Created on: May 2, 2023
 *      Author: Seif pc
 */

#ifndef OLEDDISPLAY_H_
#define OLEDDISPLAY_H_

extern unsigned char Image[1024];
/*********Includes Section********/
#include "I2C.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
/*********Define Section*********/
#define zero '0'

#define OLEDSSD_DISPLAY_ADD 0x7A
#define OLEDSSD_COMMAND 0x00
#define OLEDSSD_DATA 0x40

#define OLED_SET_CONTRAST_CONTROL 0x81
#define OLED_DISPLAY_ON_RESUME 0xA4
#define OLED_DISPLAY_ON_NRESUME 0xA5
#define OLED_NORMAL_DISPLAY 0xA6
#define OLED_INVERSE_DISPLAY 0xA7
#define OLED_DISPLAY_ON  0xAF
#define OLED_DISPLAY_OFF_SLEEP 0xAE

#define OLED_HORIZONTAL_SCROLL_RIGHT 0x26
#define OLED_HORIZONTAL_SCROLL_LEFT  0x27
#define OLED_HORIZONTAL_SCROLL_VERTICAL_RIGHT 0x29
#define OLED_HORIZONTAL_SCROLL_VERTICAL_LEFT  0x2A
#define OLED_DEACTIVATE_SCROLL 0x2E
#define OLED_ACTIVATE_SCROLL 0x2F
#define OLED_SET_VERTICAL_SCROLL_AREA 0xA3

#define OLED_SET_LOWER_COLUMN  0x00
#define OLED_SET_HIGHER_COLUMN 0x10
#define OLED_MEMORY_ADDR_MODE  0x20
#define OLED_SET_COLUMN_ADDR   0x21
#define OLED_SET_PAGE_ADDR     0x22

#define OLED_SET_DIPLAY_START_LINE 0x40
#define OLED_SET_SEG_REMAP 		   0xA0
#define OLED_SET_MULTIPLEX_RATIO   0xA8
#define OLED_SET_COM_OUTPUT_SCAN_DIRECTION 0xC0
#define OLED_COM_SCAN_DIR_DEC 0xC8
#define OLED_SET_DISPLAY_OFFSET    0xD3
#define OLED_SET_COM_PIN           0xDA

#define OLED_CHARGE_PUMP_SET 0x8D
#define OLED_SET_DISPLAY_CLK_DIVIDE 0xD5
#define OLED_SET_PRE_CHARGE_PER     0xD9
#define OLED_SET_VCOMH_DES_LEV      0xDB
#define OLED_NOP                    0x53
#define OLED_COL_SIZE 128
#define OLED_PAGE_SIZE 7
/*******Macro Function Section****/
#define OLED_SWAP(X,Y) { int16_t t = X; X = Y; Y = t; }

/*****Defined Data types******/

/*****Software Interfaces****/
Bool OLED_Init();
Bool OLED_DISPLAY_CHAR(char C);
Bool OLED_SET_CURSOR(uint8 X,uint8 Y);
void OLDE_ScrollRight(uint8_t Pag_start, uint8_t Pag_End);
void OLDE_ScrollLeft(uint8_t Pag_start, uint8_t Pag_End);
void OLED_DEACTIVATE_scroll();
void OLED_DISPLAY();
void OLED_Draw_Line(uint8 x_0,uint8 y_0,uint8 x_1,uint8 y_1);
void OLED_DRAW_Vertical_LINE(uint8 Center_x,uint8 Center_y,uint8 height);
void OLED_DRAW_HORZ_LINE(uint8 Center_x,uint8 Center_y,uint8 Width);
void OLED_DRAW_RECTANGLE(uint8 x,uint8 y,uint8 Width,uint8 Height);
Bool OLED_DISPLAY_String(char *str,uint8 X,uint8 Y,uint8 Font);
Bool OLED_display_Integer(int32 Num,uint8 Col_C,uint8 Row_C,uint8 Font);
Bool OLED_display_Float(float32 Num,uint8 Col_C,uint8 Row_C,uint8 Font);
Bool OLED_display_Image(unsigned char *Bitmap);
Bool OLED_SET_PIXEL(uint8 Page,uint8 Col);

#endif /* OLEDDISPLAY_H_ */
