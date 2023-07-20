/*
 * mcal_spi.h
 *
 *  Created on: Jul 6, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_SPI_H_
#define MCAL_MCAL_SPI_H_

/*************Include***************/
#include "Mcal_gpio.h"
#include "Mcal_External_Interrupts.h"
/*************Define Section**********/
#define SPI_PORT_DIR DDRB
#define SPI_PORT_CNTRL PORTB
#define SPI_PORT_IN PINB

#define SPI_SOFT 0x00
#define SPI_HARD 0X01

#define SPI_CLK_SPEED_4 0x00
#define SPI_CLK_SPEED_16 0x01
#define SPI_CLK_SPEED_64_1 0x02
#define SPI_CLK_SPEED_128 0x03
#define SPI_CLK_SPEED_2 0x04
#define SPI_CLK_SPEED_8 0x05
#define SPI_CLK_SPEED_32 0x06
#define SPI_CLK_SPEED_64_2 0x07

#define SPI_MSTR_MODE 0x00
#define SPI_SLAVE_MODE 0x01

#define MSB_FIRST 0x00
#define LSB_FIRST 0x01

#define SPI_SAMPLE_RISING_SETUP_FALLING 0x00
#define SPI_SETUP_RISING__SETUP_FALLING 0x01
#define SPI_SAMPLE_FALLING_SETUP_RISING 0X02
#define SPI_SETUP_FALLING_SAMPLE_RISING 0X03

#define MOSI PB5
#define MISO PB6
#define SS 		 PB4
#define SCK   PB7
/*****Macro Function_Section*******/
#define SPI_CLEAR_SS() (CLEAR_BIT(SPI_PORT_CNTRL,SS))
#define SPI_SET_SS() (SET_BIT(SPI_PORT_CNTRL,SS))

#define Mcal_SPI_EN() (SET_BIT(SPCR,SPE) )
#define Mcal_SPI_DIS() (CLEAR_BIT(SPCR,SPE))

#define Mcal_SPI_MSTR() (SET_BIT(SPCR,MSTR))
#define Mcal_SPI_SLAVE() (CLEAR_BIT(SPCR,MSTR))

#define Mcal_SPI_MSB_FIRST() (CLEAR_BIT(SPCR,DORD) )
#define Mcal_SPI_LSB_FIRST() (SET_BIT(SPCR,DORD))

#define Mcal_SPI_INT_EN() (SET_BIT(SPCR,SPIE))
#define Mcal_SPI_INT_DIS() (CLEAR_BIT(SPCR,SPIE))

#define Mcal_SPI_CLK_Polarity_H() (SET_BIT(SPCR,CPOL))//Leading edge =Rising
#define Mcal_SPI_CLK_Polarity_L() (CLEAR_BIT(SPCR,CPOL))//Leading Edge=Falling

#define Mcal_SPI_CLK_phase_H() (SET_BIT(SPCR,CPHA))//Sample on Falling Edge
#define Mcal_SPI_CLK_phase_L() (CLEAR_BIT(SPCR,CPHA))//Sample on Leading Edge
/**********Typedef Section***********/
typedef struct
{
	uint8 SPI_CLK_SPEED:4;
	uint8 SPI_mode:1;
	uint8 SPI_ORD:1;
	uint8 SPI_RES:1;
	uint8 SPI_sample_set;
#if SPI_INT_EN==TRUE
	void (*SPI_callback) (void);
#endif
}spi_cnfg_t;
/***********Software Interfaces*******/
uint8 SPI_CONFIG(spi_cnfg_t *Spi);
void SPI_SEND_BYTE(uint8 Data);
void SPI_SEND_STR(uint8 *str);
void SPI_RX_BYTE(uint8 *RX);
void SPI_RX_string(uint8 *RX);

#endif /* MCAL_MCAL_SPI_H_ */
