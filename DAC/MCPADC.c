/*
 * MCPADC.c
 *
 *  Created on: Apr 23, 2023
 *      Author: Seif pc
 */
#include "MCPADC.h"


Bool DAC_INIT()
{
	Bool E_Stat=E_OK;
	I2C_CONFIG_t I2C_t={
			.SCL_FREQ=I2C_STANDARD_MODE,
			.Prescaler_Freq=TWPS_PRESCALER_1,
	};
	E_Stat=I2C_INIT(&I2C_t);
	return E_Stat;
}


Bool DAC_Write_voltage_NEEPROM(uint16 Voltage)
{
	Bool E_State=E_OK;
	uint8 I2C_DATA_BUFFER[2];//to write a Data bin

	I2C_DATA_BUFFER[0]=COMMAND_FAST_MODE|((Voltage>>8)&0x0F);
	I2C_DATA_BUFFER[1]=(uint8)Voltage;

	E_State=I2C_Write_NBYTE_(MCP4725_ADDRESS,I2C_DATA_BUFFER,2);
	return E_State;
}

Bool DAC_Write_voltage_EEPROM(uint16 Voltage)
{
	Bool E_State = E_OK;
	uint8 I2C_DATA_BUFFER[3]; //to write a Data bin

	I2C_DATA_BUFFER[0] = COMMAND_WRITE_DAC_EEPROM<<5;
	I2C_DATA_BUFFER[1] = (uint8) (Voltage>>4);
	I2C_DATA_BUFFER[2] =  (Voltage&0x000F)<<4;

	E_State = I2C_Write_NBYTE_(MCP4725_ADDRESS, I2C_DATA_BUFFER, 3);
	return E_State;
}
