/*
 * SPI.c
 *
 *  Created on: Mar 7, 2023
 *      Author: Seif pc
 */

#include "SPI.h"

static Bool SPI_CLK_SET(SPI_CONFIG_t *Spi)
{
	Bool ret=E_OK;
	switch(Spi->CLK_set)
	{
	case SPI_FOSC_4:
		CLEAR_BIT(SPCR, SPR0);
		CLEAR_BIT(SPCR, SPR1);
		CLEAR_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_16:
		SET_BIT(SPCR, SPR0);
		CLEAR_BIT(SPCR, SPR1);
		CLEAR_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_64:
		CLEAR_BIT(SPCR, SPR0);
		SET_BIT(SPCR, SPR1);
		CLEAR_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_128:
		SET_BIT(SPCR, SPR0);
		SET_BIT(SPCR, SPR1);
		CLEAR_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_2:
		CLEAR_BIT(SPCR, SPR0);
		CLEAR_BIT(SPCR, SPR1);
		SET_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_8:
		SET_BIT(SPCR, SPR0);
		CLEAR_BIT(SPCR, SPR1);
		SET_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_32:
		CLEAR_BIT(SPCR, SPR0);
		SET_BIT(SPCR, SPR1);
		SET_BIT(SPSR, SPI2X);
		break;
	case SPI_FOSC_64_D:
		SET_BIT(SPCR, SPR0);
		SET_BIT(SPCR, SPR1);
		SET_BIT(SPSR, SPI2X);
		break;
	default:
		ret=E_NOK;
	}
	return ret;
}
Bool SPI_INIT(SPI_CONFIG_t *spi)
{
	Bool E_state=E_OK;
	if(NULL==spi)
		E_state=E_NOK;
	else
	{
		SPI_DISABLE();
		switch(spi->SPI_MSTR_SLAVE_set)
		{
			case SPI_MSTR_MODE:
				SPI_MSTR_MODE_SET();
				SPI_PORT_DIR|=(1<<MOSI|1<<SCK|1<<SS);
				SPI_PORT_DIR = SPI_PORT_DIR & ~(1<<MISO);
				break;
			case SPI_SLAVE_MODE:
				SPI_SLAVE_MODE_SET();
				SPI_PORT_DIR|=(1<<MISO);
				break;
			default:
				E_state=E_NOK;
		}
		switch(spi->SPI_CLOCK_polarity_set)
		{
			case CLOCK_POLARITY_RSEDG_FEDG:
				SPI_CLOCK_POLARITY_DISABLE();
				break;
			case CLOCK_POLARITY_FEDG_RSEDG:
				SPI_CLOCK_POLARITY_ENABLE();
				break;
			default:
				E_state=E_NOK;
		}
		switch(spi->SPI_CLOCK_phase_set)
		{
			case CLOCK_PHASE_SAMPLE_LEADING:
				SPI_CLOCK_PHASE_ENABLE();
				break;
			case CLOCK_PHASE_SAMPLE_TRAILING:
				SPI_CLOCK_PHASE_DISABLE();
				break;
			default:
				E_state=E_NOK;
		}
		switch(spi->SPI_Shift_dir_set)
		{
			case MSB_FIRST_SHIFT:
				SPI_MSB_FIRST();
				break;
			case LSB_FIRST_SHIFT:
				SPI_LSB_FIRST();
				break;
		}
		E_state=SPI_CLK_SET(spi);
	}
	SPI_ENABLE();
	return E_state;
//	SPI_PORT_DIR|=(1<<MOSI|1<<SCK|1<<SS);
//    SPI_PORT_DIR = SPI_PORT_DIR & ~(1<<MISO);
//    SPSR=(1<<SPI2X);
//	SPCR = ((1<<SPE) | (1<<MSTR));// enable SPI + enable Master + choose SPI clock = Fosc/4
//	return E_state;
}

Bool SPI_SEND_blocking(uint8 data)
{
	Bool E_STAT=E_OK;
	SPDR=data;
	while(BIT_IS_CLEAR(SPSR,SPIF));
	return E_STAT;
}

Bool SPI_SEND_string(uint8 *data)
{
	Bool E_STAT=E_OK;
	while(*data)
	{
		SPI_SEND_blocking(*data);
		data++;
	}
	return E_STAT;
}

Bool SPI_REC_DATA(uint8 *data)
{
	Bool E_STAT = E_OK;
	while(BIT_IS_CLEAR(SPSR,SPIF));
	*data=SPDR;
	return E_STAT;
}
Bool SPI_REC_STRING(uint8 *data)
{
	uint8 Bytes_RX=0;
	Bool E_STAT = E_OK;
	E_STAT=SPI_REC_DATA(data);
	Bytes_RX++;
	while(*(data+Bytes_RX)!='#')
	{
		E_STAT=SPI_REC_DATA(data+Bytes_RX);
		Bytes_RX++;
	}
	return E_STAT;
}
