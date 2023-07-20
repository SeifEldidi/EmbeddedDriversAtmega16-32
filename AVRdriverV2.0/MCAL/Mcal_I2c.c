/*
 * Mcal_I2c.c
 *
 *  Created on: Jun 4, 2023
 *      Author: Seif pc
 */

#include "Mcal_I2C.h"

static int32 Pow_4(int8 exp)
{
	int8 Counter=0;
	int32 ret=1;
	for(Counter=0;Counter<=exp-1;Counter++)
		ret*=4;
	return ret;
}


uint8 Mcal_I2C_init(I2C_CONFIG_t *I2C)
{
	Bool ret = E_OK;
	switch (I2C->Prescaler_Freq) {
	case TWPS_PRESCALER:
		CLEAR_BIT(TWSR, TWPS0);
		CLEAR_BIT(TWSR, TWPS1);
		break;
	case TWPS_PRESCALER_4:
		SET_BIT(TWSR, TWPS0);
		CLEAR_BIT(TWSR, TWPS1);
		break;
	case TWPS_PRESCALER_16:
		CLEAR_BIT(TWSR, TWPS0);
		SET_BIT(TWSR, TWPS1);
		break;
	case TWPS_PRESCALER_64:
		SET_BIT(TWSR, TWPS0);
		SET_BIT(TWSR, TWPS1);
		break;
	default:
		ret = E_NOK;
	}
	if (ret == E_OK)
		TWBR = ((uint32) F_CPU / I2C->SCL_FREQ - 16)
				/ (2 * Pow_4(I2C->Prescaler_Freq));
	else {
	}
#if I2C_RECIEVE_INTERRUPT==TRUE
	I2C_RX_CALLBACK_PTR = I2C->I2C_RX_CALLBACK;
	SET_BIT(TWCR, TWIE);
#endif
	I2C_PORT_DIR|=((1<<PC0)|(1<<PC1));
	return ret;
}

uint8 Mcal_I2C_write_byte(uint8 Address,uint8 Reg_Address,uint8 byte)
{
	uint8 Err_Status=E_OK;
	/*******Gen start Bit*****/
	Mcal_I2C_GEN_START();
	Mcal_I2C_Wait();
	/*****Check if start and bus is not busy*/
	if((I2C_STATUS&0xF8)==TWSR_START_MSTR)
	{
		/**Send Address of I2C device with Ack****/
		I2C_WRITE_BUFFER=Address|I2C_WRITE;
		I2C_WRITE_ENABLE();
		Mcal_I2C_Wait();
		/******Check arbitration*****/
		if((I2C_STATUS&0xF8)==TWSR_MSTR_ADD_ACK_REC)
		{
			/*******Send address of register in I2C device*****/
			I2C_WRITE_BUFFER = Reg_Address|I2C_WRITE;
			I2C_WRITE_ENABLE();
			Mcal_I2C_Wait();
			/*******Check arbitration*********/
			if((I2C_STATUS&0xF8)==TWSR_MSTR_DATA_ACK_REC)
			{
				/*********Send Data byte********/
				I2C_WRITE_BUFFER = byte;
				I2C_WRITE_ENABLE();
				Mcal_I2C_Wait();
				/******Send Stop bit to terminate bus control*/
				if((I2C_STATUS&0xF8)==TWSR_MSTR_DATA_ACK_REC)
				{
					Mcal_I2C_GEN_STOP();
					Mcal_I2C_Wait_Stop();
				}else{
					Err_Status=E_NOK;
				}
			}else{
				Err_Status=E_NOK;
			}
		}
		else{
			Err_Status=E_NOK;
		}
	}else{
		Err_Status=E_NOK;
	}
	return Err_Status;
}

uint8 Mcal_I2C_write_buffer(uint8 Address,uint8 Reg_Address,uint8 *Buffer,uint8 Buffer_len)
{
	uint8 Err_Status = E_OK;
	/*******Gen start Bit*****/
	Mcal_I2C_GEN_START();
	Mcal_I2C_Wait();
	/*****Check if start and bus is not busy*/
	if ((I2C_STATUS&0xF8) == TWSR_START_MSTR) {
		/**Send Address of I2C device with Ack****/
		I2C_WRITE_BUFFER = Address|I2C_WRITE;
		I2C_WRITE_ENABLE();
		Mcal_I2C_Wait();
		/******Check arbitration*****/
		if ((I2C_STATUS&0xF8) == TWSR_MSTR_ADD_ACK_REC) {
			/*******Send address of register in I2C device*****/
			I2C_WRITE_BUFFER = Reg_Address|I2C_WRITE;
			I2C_WRITE_ENABLE();
			Mcal_I2C_Wait();
			/*******Check arbitration*********/
			if ((I2C_STATUS&0xF8) == TWSR_MSTR_DATA_ACK_REC) {
				/*********Send Data buffer********/
				uint8 Byte_Cnt=0;
				while(Byte_Cnt<=Buffer_len-1)
				{
						I2C_WRITE_BUFFER = *(Buffer+Byte_Cnt);
						I2C_WRITE_ENABLE();
						Mcal_I2C_Wait();
						/******Check status*/
						if ((I2C_STATUS&0xF8)  != TWSR_MSTR_DATA_ACK_REC) {
							Err_Status = E_NOK;
							break;
						} else {
						}
						Byte_Cnt++;
				}
				/******Send Stop bit to terminate bus control*/
				Mcal_I2C_GEN_STOP();
				Mcal_I2C_Wait_Stop();
			} else {
				Err_Status = E_NOK;
			}
		} else {
			Err_Status = E_NOK;
		}
	} else {
		Err_Status = E_NOK;
	}
	return Err_Status;
}

uint8 Mcal_I2C_read_byte(uint8 Address,uint8 Reg_Address,uint8 *Byte)
{
	uint8 Err_status = E_OK;
		/*******Gen start Bit*****/
		Mcal_I2C_GEN_START();
		Mcal_I2C_Wait();
		/*****Check if start and bus is not busy*/
		if ((I2C_STATUS&0xF8) == TWSR_START_MSTR) {
			/**Send Address of I2C device with Ack****/
			I2C_WRITE_BUFFER = Address | I2C_WRITE;
			I2C_WRITE_ENABLE();
			 Mcal_I2C_Wait();
			/******Check arbitration*****/
			if ((I2C_STATUS&0xF8) == TWSR_MSTR_ADD_ACK_REC) {
				/*******Send address of register in I2C device*****/
				I2C_WRITE_BUFFER = Reg_Address | I2C_WRITE;
				I2C_WRITE_ENABLE();
				Mcal_I2C_Wait();
				/*******Check arbitration*********/
				if ((I2C_STATUS&0xF8) == TWSR_MSTR_DATA_ACK_REC) {
					/*********REPEATED Start********/
					Mcal_I2C_GEN_START();
					Mcal_I2C_Wait();
					if ((I2C_STATUS & 0xF8) == TWSR_REPEATED_START_MSTR) {
							I2C_WRITE_BUFFER = Address | I2C_READ;
							I2C_WRITE_ENABLE();
							Mcal_I2C_Wait();
							if((I2C_STATUS & 0xF8) == TW_MSR_DATA_R_ACK){
								I2C_READ_NACK();
								Mcal_I2C_Wait();
								*Byte = I2C_WRITE_BUFFER;
								if ((I2C_STATUS & 0xF8) != TW_MSR_DATA_R_NACK) {
										Err_status = E_NOK;
									}
								}
							/******Send Stop bit to terminate bus control*/
								Mcal_I2C_GEN_STOP();
								Mcal_I2C_Wait_Stop();
							}
				} else {
					Err_status = E_NOK;
				}
			} else {
				Err_status = E_NOK;
			}
		} else {
			Err_status = E_NOK;
		}
		return Err_status;
}

uint8 Mcal_I2C_read_buffer(uint8 Address,uint8 Reg_Address,uint8 *Byte,uint8 Buffer_length)
{
	uint8 Err_status = E_OK;
	/*******Gen start Bit*****/
	Mcal_I2C_GEN_START();
	Mcal_I2C_Wait();
	/*****Check if start and bus is not busy*/
	if ((I2C_STATUS&0xF8) == TWSR_START_MSTR) {
		/**Send Address of I2C device with Ack****/
		I2C_WRITE_BUFFER = Address | I2C_WRITE;
		I2C_WRITE_ENABLE();
		 Mcal_I2C_Wait();
		/******Check arbitration*****/
		if ((I2C_STATUS&0xF8) == TWSR_MSTR_ADD_ACK_REC) {
			/*******Send address of register in I2C device*****/
			I2C_WRITE_BUFFER = Reg_Address | I2C_WRITE;
			I2C_WRITE_ENABLE();
			Mcal_I2C_Wait();
			/*******Check arbitration*********/
			if ((I2C_STATUS&0xF8) == TWSR_MSTR_DATA_ACK_REC) {
				/*********REPEATED Start********/
				Mcal_I2C_GEN_START();
				Mcal_I2C_Wait();
				if ((I2C_STATUS & 0xF8) == TWSR_REPEATED_START_MSTR) {
						I2C_WRITE_BUFFER = Address | I2C_READ;
						I2C_WRITE_ENABLE();
						Mcal_I2C_Wait();
						if((I2C_STATUS & 0xF8) == TW_MSR_SLA_R_ACK){
						uint8 BYTE_RX = 0;
						while (BYTE_RX <= Buffer_length - 1) {
							if (BYTE_RX < Buffer_length - 1) {
								I2C_READ_ACK();
								Mcal_I2C_Wait();
								*(Byte+BYTE_RX) = I2C_WRITE_BUFFER;
								if ((I2C_STATUS & 0xF8) != TW_MSR_DATA_R_ACK) {
									Err_status = E_NOK;
									break;
								}
							} else if (BYTE_RX == Buffer_length - 1) {
								I2C_READ_NACK();
								Mcal_I2C_Wait();
								*(Byte+BYTE_RX) = I2C_WRITE_BUFFER;
								if ((I2C_STATUS & 0xF8) != TW_MSR_DATA_R_NACK) {
									Err_status = E_NOK;
									break;
								}
							}
							BYTE_RX++;
						}
						/******Send Stop bit to terminate bus control*/
						Mcal_I2C_GEN_STOP();
						Mcal_I2C_Wait_Stop();
					}
				}
			} else {
				Err_status = E_NOK;
			}
		} else {
			Err_status = E_NOK;
		}
	} else {
		Err_status = E_NOK;
	}
	return Err_status;
}
