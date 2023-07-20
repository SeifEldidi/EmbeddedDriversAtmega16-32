/*
 * mcal_spi.c
 *
 *  Created on: Jul 6, 2023
 *      Author: Seif pc
 */

#include "mcal_spi.h"

uint8 SPI_CONFIG(spi_cnfg_t *Spi)
{
	uint8 Ret=E_OK;
	if(Spi==NULL)
		Ret= E_NOK;
	else{
		/***Disable SPI***/
		Mcal_SPI_DIS();
		Mcal_SPI_INT_DIS();
		/*******Set SPI_DIr******/
		switch(Spi->SPI_mode)
		{
			case SPI_MSTR_MODE:
				 Mcal_SPI_MSTR();
				 SPI_PORT_DIR|=(1<<MOSI|1<<SCK|1<<SS);
				break;
			case SPI_SLAVE_MODE:
				Mcal_SPI_SLAVE();
				SPI_PORT_DIR|=(1<<MISO);
				break;
		}
		/*******Set SPI_Speed******/
		SPCR|=(Spi->SPI_CLK_SPEED&0x03);
		SPSR|=((Spi->SPI_CLK_SPEED&0x04)>>2);
		/*********Set Spi Sampling******/
		switch(Spi->SPI_sample_set)
		{
			case SPI_SAMPLE_RISING_SETUP_FALLING:
				Mcal_SPI_CLK_Polarity_L();
				Mcal_SPI_CLK_phase_L();
				break;
			case SPI_SETUP_RISING__SETUP_FALLING:
				Mcal_SPI_CLK_Polarity_L();
				Mcal_SPI_CLK_phase_H();
				break;
			case SPI_SAMPLE_FALLING_SETUP_RISING:
				Mcal_SPI_CLK_Polarity_H();
				Mcal_SPI_CLK_phase_L();
				break;
			case SPI_SETUP_FALLING_SAMPLE_RISING:
				Mcal_SPI_CLK_Polarity_H();
				Mcal_SPI_CLK_phase_H();
				break;
		}
		/*********Set Spi data sent*******/
		switch(Spi->SPI_ORD)
		{
			case	MSB_FIRST:
				Mcal_SPI_MSB_FIRST();
				break;
			case LSB_FIRST:
				Mcal_SPI_LSB_FIRST();
				break;
		}
		Mcal_SPI_EN();
	}
	return Ret;
}
void SPI_SEND_BYTE(uint8 Data)
{
	SPDR=Data;
	while(!READ_BIT(SPSR,SPIF));
}
void SPI_SEND_STR(uint8 *str)
{
	while(str)
	{
		SPI_SEND_BYTE(str++);
	}
}
void SPI_RX_BYTE(uint8 *RX)
{
	while(!READ_BIT(SPSR,SPIF));
	*RX=SPDR;
}
void SPI_RX_string(uint8 *RX)
{
	int8 i=-1;
	do
	{
		i++;
		SPI_RX_BYTE(RX+i);
	}while(*(RX+i)!='#');
}
