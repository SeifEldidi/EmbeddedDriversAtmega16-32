/*
 * MCPADC.h
 *
 *  Created on: Apr 23, 2023
 *      Author: Seif pc
 */

#ifndef MCPADC_H_
#define MCPADC_H_

/************Includes Section**************/
#include "I2C.h"
/************Define Section******************/
#define MCP4725_ADDRESS 0xC0

#define COMMAND_FAST_MODE 0x00
#define COMMAND_WRIE_DAC_REG 0X02
#define COMMAND_WRITE_DAC_EEPROM 0x03
/*********Software Interfaces*******************/
extern Bool DAC_INIT();
extern Bool DAC_Write_voltage_NEEPROM(uint16 Voltage);
extern Bool DAC_Write_voltage_EEPROM(uint16 Voltage);


#endif /* MCPADC_H_ */
