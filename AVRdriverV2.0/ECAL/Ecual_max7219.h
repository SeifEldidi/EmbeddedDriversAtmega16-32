/*
 * Ecual_max7219.h
 *
 *  Created on: Jul 6, 2023
 *      Author: Seif pc
 */

#ifndef ECAL_ECUAL_MAX7219_H_
#define ECAL_ECUAL_MAX7219_H_
/*Includes*/
#include "../MCAL/Mcal_gpio.h"
#include "../MCAL/Mcal_spi.h"
#include "../MCAL/mcal_std_types.h"
#include "../MCAL/mcal_timer0.h"
/************Define Section******************/
#define DIGIT_0  0x01
#define DIGIT_1  0x02
#define DIGIT_2  0x03
#define DIGIT_3  0x04
#define DIGIT_4  0x05
#define DIGIT_5  0x06
#define DIGIT_6  0x07
#define DIGIT_7  0x08
#define DECODE_MODE  0x09
#define INTENSITY_REG  0x0A
#define SCAN_LIM_REG  0x0B
#define SHUTDOWN_REG  0x0C
#define DISPLAY_TEST_REG  0x0F
#define NO_DIGITS 8
#define POWER_ON 0x01
#define NORMAL_OP 0x00
#define EXTERNAL_DECODER 0x00
#define INTENSITY_LEVEL7 0x07


Bool MAX7219_INIT();
Bool MAX7219_SEND_Data(uint8 Address,uint8 Data);
void MAX7219Disp_Number(int32_t data);
void MAX7219Disp_string(char *str);
void MAX7219Disp_float(float Val,Bool Temp_flag);
Bool Max7219_Send_DArr_Ddata(uint8 Address,uint8 Data[],uint8 size,Bool *temp_flag,Bool *temp_flag_2);
void MAX7219Disp_float_num(float Val,Bool Temp_flag,uint32 Num);
Bool MAX7219_INIT_ARR();

#endif /* ECAL_ECUAL_MAX7219_H_ */
