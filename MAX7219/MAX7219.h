/*
 * MAX7219.h
 *
 *  Created on: Mar 21, 2023
 *      Author: Seif pc
 */

#ifndef MAX7219_H_
#define MAX7219_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
#include "spi.h"
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
#define NO_DIGITS 6
#define POWER_ON 0x01
#define NORMAL_OP 0x00
#define EXTERNAL_DECODER 0x00
#define INTENSITY_LEVEL7 0x07

/*********Software Interfaces*******************/
Bool MAX7219_INIT();
Bool MAX7219_SEND_Data(uint8 Address,uint8 Data);
void MAX7219Disp_Number(int32_t data);


#endif /* MAX7219_H_ */
