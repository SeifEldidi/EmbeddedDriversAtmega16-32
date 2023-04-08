/*
 * I2C.h
 *
 *  Created on: Mar 25, 2023
 *      Author: Seif pc
 */

#ifndef I2C_H_
#define I2C_H_

#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
/************Define Section******************/
#define TWPS_PRESCALER_1   (uint8)0x00
#define TWPS_PRESCALER_4   (uint8)0x01
#define TWPS_PRESCALER_16  (uint8)0x02
#define TWPS_PRESCALER_64  (uint8)0x03

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
#define I2C_WRITE 0x00
#define I2C_READ 0x01

#define I2C_STANDARD_MODE 100000UL
#define I2C_FAST_MODE 400000UL
#define I2C_FAST_MODE_PLUS 1000000UL
#define I2C_HIGH_SPEED_MODE 3400000UL
/***********Macro Like Function**************/
#define I2C_ENABLE()    (SET_BIT(TWCR,TWEN))
#define I2C_DIABLE() (CLEAR_BIT(TWCR,TWEN))

#define I2C_WAIT() {while(BIT_IS_CLEAR(TWCR,TWINT));}
#define I2C_WAIT_STOP() {while(BIT_IS_SET(TWCR,TWSTO));}

#define I2C_CLEAR_FLAG() (SET_BIT(TWCR,TWINT))

#define I2C_ENABLE_ACK()  (SET_BIT(TWCR,TWEA))
#define I2C_DISABLE_ACK() (CLEAR_BIT(TWCR,TWEA))

#define I2C_READ_ACK() (TWCR = (1<<TWEA)|(1<<TWINT)|(1<<TWEN))
#define I2C_READ_NACK() (TWCR = (1<<TWINT)|(1<<TWEN))

#define I2C_GEN_START_COND()		(TWCR = (1<<TWSTA)|(1<<TWINT)|(1<<TWEN))
#define I2C_GEN_STOP_COND()		(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO))
#define I2C_WRITE_ENABLE()   (TWCR = (1<<TWINT) | (1<<TWEN))

#define I2C_IS_WRITE_COLL() (READ_BIT(TWCR,TWWC))
/*************Defined Data types**************/
typedef struct
{
	uint32 SCL_FREQ;
	uint8 Prescaler_Freq:2;
	uint8 Reserved:6;
}I2C_CONFIG_t;

/*********Software Interfaces*******************/
Bool I2C_INIT(I2C_CONFIG_t *I2C);
Bool I2C_WRITE_NBYTES(uint8 Add,uint8 H_add,uint8 *Data,uint8 No_bytes);
Bool I2C_READ_NBYTES(uint8 Add,uint8 H_add,uint8 *Data,int8 No_bytes);


#endif /* I2C_H_ */
