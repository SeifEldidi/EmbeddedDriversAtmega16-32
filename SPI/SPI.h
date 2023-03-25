/*
 * SPI.h
 *
 *  Created on: Mar 7, 2023
 *      Author: Seif pc
 */

#ifndef SPI_H_
#define SPI_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define SPI_PORT_DIR DDRB
#define MOSI PB5
#define MISO PB6
#define SCK PB7
#define SS PB4

#define SPI_MSTR_MODE 0x01
#define SPI_SLAVE_MODE 0x00

#define MSB_FIRST_SHIFT 0x00
#define LSB_FIRST_SHIFT 0x01

#define CLOCK_POLARITY_RSEDG_FEDG 0x00
#define CLOCK_POLARITY_FEDG_RSEDG 0x01
#define CLOCK_PHASE_SAMPLE_LEADING 0x01
#define CLOCK_PHASE_SAMPLE_TRAILING 0x00
/***********Macro Like Function**************/
#define SPI_DISABLE()  (CLEAR_BIT(SPCR,SPE))
#define SPI_ENABLE()  (SET_BIT(SPCR,SPE))

#define SPI_MSB_FIRST() (CLEAR_BIT(SPCR,DORD))
#define SPI_LSB_FIRST() (SET_BIT(SPCR,DORD))

#define SPI_MSTR_MODE_SET() (SET_BIT(SPCR,MSTR))
#define SPI_SLAVE_MODE_SET() (CLEAR_BIT(SPCR,MSTR))

#define SPI_DOUBLE_SPEED_ENABLE() (SET_BIT(SPSR,SPI2X))
#define SPI_DOUBLE_SPEED_DISABLE() (CLEAR_BIT(SPSR,SPI2X))

#define SPI_CLOCK_POLARITY_ENABLE() (SET_BIT(SPCR,CPOL))
#define SPI_CLOCK_POLARITY_DISABLE() (CLEAR_BIT(SPCR,CPOL))

#define SPI_CLOCK_PHASE_ENABLE()  (SET_BIT(SPCR,CPHA))
#define SPI_CLOCK_PHASE_DISABLE()  (CLEAR_BIT(SPCR,CPHA))

#define SPI_CLEAR_SS() (CLEAR_BIT(PORTB,SS))
#define SPI_SET_SS() (SET_BIT(PORTB,SS))

#define SPI_CLEAR_FLAG() (SET_BIT(SPSR,SPIF))
/*************Defined Data types**************/
typedef enum
{
	SPI_FOSC_4,
	SPI_FOSC_16,
	SPI_FOSC_64,
	SPI_FOSC_128,
	SPI_FOSC_2,
	SPI_FOSC_8,
	SPI_FOSC_32,
	SPI_FOSC_64_D,
}SPI_CLOCK_PRESCALER;

typedef struct
{
	uint8 SPI_MSTR_SLAVE_set:1;
	uint8 SPI_Shift_dir_set:1;
	uint8 SPI_CLOCK_phase_set:1;
	uint8 SPI_CLOCK_polarity_set:1;
	SPI_CLOCK_PRESCALER CLK_set;
}SPI_CONFIG_t;

/*********Software Interfaces*******************/
Bool SPI_INIT(SPI_CONFIG_t *spi);
Bool SPI_SEND_blocking(uint8 data);
Bool SPI_SEND_string(uint8 *data);
Bool SPI_REC_DATA(uint8 *data);
Bool SPI_REC_STRING(uint8 *data);

#endif /* SPI_H_ */
