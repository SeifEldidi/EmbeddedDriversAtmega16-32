/*
 * Mcal_I2c.h
 *
 *  Created on: Jun 4, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_I2C_H_
#define MCAL_MCAL_I2C_H_

/******Includes Section***********/
#include "mcal_std_types.h"
#include "Mcal_External_Interrupts.h"
/************Define Section***************/
#define TWPS_PRESCALER 0x00
#define TWPS_PRESCALER_4 0x01
#define TWPS_PRESCALER_16 0x02
#define TWPS_PRESCALER_64 0x03

#define TWSR_START_MSTR          0x08
#define TWSR_REPEATED_START_MSTR 0x10
#define TWSR_MSTR_ADD_ACK_REC   0x18
#define TWSR_MSTR_ADD_NACK_REC  0x20
#define TWSR_MSTR_DATA_ACK_REC  0x28
#define TWSR_MSTR_DATA_NACK_REC 0x30
#define TWSR_ARBIT_LOSS         0x38

#define TW_MSR_SLA_R_ACK 0x40
#define TW_MSR_SLA_R_NACK 0x48
#define TW_MSR_DATA_R_ACK 0x50
#define TW_MSR_DATA_R_NACK 0x58

#define I2C_STATUS TWSR
#define I2C_WRITE_BUFFER TWDR
#define I2C_PORT_DIR DDRC
#define I2C_WRITE 0x00
#define I2C_READ 0x01

#define I2C_STANDARD_MODE 100000UL
#define I2C_FAST_MODE 400000UL
#define I2C_FAST_MODE_PLUS 1000000UL
#define I2C_HIGH_SPEED_MODE 3400000UL
/********Macro Function Section*********/
#define Mcal_I2C_ENABLE() (SET_BIT(TWCR,TWEN))
#define Mcal_I2C_DISABLE() (CLEAR_BIT(TWCR,TWEN))

#define Mcal_I2C_CLEAR_FLAG() (SET_BIT(TWCR,TWINT))

#define Mcal_I2C_ENABLE_ACK() (SET_BIT(TWCR,TWEA))
#define Mcal_I2C_DISABLE_ACK() (CLEAR_BIT(TWCR,TWEA))

#define Mcal_I2C_GEN_START() (TWCR|=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA))
#define Mcal_I2C_Wait() {while(!READ_BIT(TWCR,TWINT));}

#define I2C_READ_ACK() (TWCR = (1<<TWEA)|(1<<TWINT)|(1<<TWEN))
#define I2C_READ_NACK() (TWCR = (1<<TWINT)|(1<<TWEN))

#define I2C_WRITE_ENABLE()   (TWCR = (1<<TWINT) | (1<<TWEN))
#define Mcal_I2C_GEN_STOP() (TWCR|=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO))
#define Mcal_I2C_Wait_Stop() {while(READ_BIT(TWCR,TWINT));}

#define I2C_IS_WRITE_COLL() (READ_BIT(TWCR,TWWC))

#define MCAL_I2C_ENABLE_INT() ()
#define MCAL_I2C_DISABLE_INT() ()
#define Mcal_I2C_FLAG_CLEAR() ()
/*************Defined Data types**************/
typedef struct
{
	uint32 SCL_FREQ;
	uint8 Prescaler_Freq:2;
	uint8 Reserved:6;
#if I2C_RECIEVE_INTERRUPT==FEATURE_ENABLE
	void (*I2C_RX_CALLBACK)(void);
#endif
}I2C_CONFIG_t;
/***********Software Interfaces**********/
uint8 Mcal_I2C_init(I2C_CONFIG_t *I2C);
uint8 Mcal_I2C_write_byte(uint8 Address,uint8 Reg_Address,uint8 byte);
uint8 Mcal_I2C_write_buffer(uint8 Address,uint8 Reg_Address,uint8 *Buffer,uint8 Buffer_len);
uint8 Mcal_I2C_read_byte(uint8 Address,uint8 Reg_Address,uint8 *Byte);
uint8 Mcal_I2C_read_buffer(uint8 Address,uint8 Reg_Address,uint8 *Byte,uint8 Buffer_length);
#endif /* MCAL_MCAL_I2C_H_ */
