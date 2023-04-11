/*
 * I2C.c
 *
 *  Created on: Mar 25, 2023
 *      Author: Seif pc
 */
#include "I2C.h"

#if I2C_RECIEVE_INTERRUPT==FEATURE_ENABLE
	void (*I2C_RX_CALLBACK_PTR)(void)=NULL;
#endif

static int32 Pow_4(int8 exp)
{
	int8 Counter=0;
	int32 ret=1;
	for(Counter=0;Counter<=exp-1;Counter++)
		ret*=4;
	return ret;
}

void I2C_WRITE_BYTE(uint8 data)
{
	I2C_WRITE_BUFFER=data;
	I2C_WRITE_ENABLE();
	I2C_WAIT(); //wait for data transmission
}

Bool I2C_INIT(I2C_CONFIG_t *I2C)
{
	Bool ret=E_OK;
	switch(I2C->Prescaler_Freq)
	{
		case TWPS_PRESCALER_1:
			CLEAR_BIT(TWSR,TWPS0);
			CLEAR_BIT(TWSR,TWPS1);
			break;
		case TWPS_PRESCALER_4:
			SET_BIT(TWSR,TWPS0);
			CLEAR_BIT(TWSR,TWPS1);
			break;
		case TWPS_PRESCALER_16:
			CLEAR_BIT(TWSR,TWPS0);
			SET_BIT(TWSR,TWPS1);
			break;
		case TWPS_PRESCALER_64:
			SET_BIT(TWSR,TWPS0);
			SET_BIT(TWSR,TWPS1);
			break;
		default:
			ret=E_NOK;
	}
	if(ret==E_OK)
		TWBR=((uint32)F_CPU/I2C->SCL_FREQ-16)/(2*Pow_4(I2C->Prescaler_Freq));
	else{
	}
#if I2C_RECIEVE_INTERRUPT==FEATURE_ENABLE
	I2C_RX_CALLBACK_PTR=I2C->I2C_RX_CALLBACK;
	SET_BIT(TWCR,TWIE);
#endif
	return ret;
}

Bool I2C_Write_BYTE(uint8 Add,uint8 REG_add,uint8 Data)
{
	Bool ret = E_OK;
	I2C_GEN_START_COND(); //initiate Start Condition on SCL/SDA
	I2C_WAIT();
	switch (I2C_STATUS) {
	case TWSR_START_MSTR:
	case TWSR_REPEATED_START_MSTR:
		break;
	case TWSR_ARBIT_LOSS:
		ret = E_NOK; //Error
	} //start bit sent SDA switched from high to low bus is busy hence send address
	if (ret == E_OK) {
		I2C_WRITE_BYTE(Add | I2C_WRITE);
		switch (I2C_STATUS) {
		case TWSR_MSTR_ADD_ACK_REC:
			break;
		case TWSR_MSTR_ADD_NACK_REC:
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		} //salve+W/R transmitted send data
	}
	if (ret == E_OK) {
		I2C_WRITE_BYTE(REG_add | I2C_WRITE);
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		}
	}
	if (ret == E_OK) {
		I2C_WRITE_BYTE(Data);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		}
	}
	return ret;
}

Bool I2C_WRITE_NBYTES(uint8 Add,uint8 REG_add,uint8 *Data,uint8 No_bytes)
{
	Bool ret = E_OK;
	I2C_GEN_START_COND();//initiate Start Condition on SCL/SDA
	I2C_WAIT();
	switch(I2C_STATUS)
	{
		case TWSR_START_MSTR:
		case TWSR_REPEATED_START_MSTR:
			break;
		case TWSR_ARBIT_LOSS:
			ret=E_NOK;//Error
	}//start bit sent SDA switched from high to low bus is busy hence send address
	if(ret==E_OK)
	{
		I2C_WRITE_BYTE(Add|I2C_WRITE);
		switch (I2C_STATUS) {
			case TWSR_MSTR_ADD_ACK_REC:
				break;
			case TWSR_MSTR_ADD_NACK_REC:
			case TWSR_ARBIT_LOSS:
				ret = E_NOK;
				break;
			default:
				ret = E_NOK;
		} //salve+W/R transmitted send data
	}
	if(ret==E_OK)
	{
		I2C_WRITE_BYTE(REG_add|I2C_WRITE);
		switch(I2C_STATUS)
		{
			case TWSR_MSTR_DATA_ACK_REC:
				break;
			case TWSR_MSTR_DATA_NACK_REC:
			case TWSR_ARBIT_LOSS:
				ret=E_NOK;
				break;
			default:
				ret=E_NOK;
		}
	}
	if(ret==E_OK)
	{
		uint8 Bytes_sent=0;
		for(Bytes_sent=0;Bytes_sent<=No_bytes-1;Bytes_sent++)
		{
			I2C_WRITE_BYTE(*(Data+Bytes_sent));
			I2C_WAIT();
			switch(I2C_STATUS)
			{
				case TWSR_MSTR_DATA_ACK_REC:
					break;
				case TWSR_MSTR_DATA_NACK_REC:
				case TWSR_ARBIT_LOSS:
					ret = E_NOK;
					break;
				default:
					ret = E_NOK;
			}
			if(ret==E_NOK)
				break;
		}
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return ret;
}

Bool I2C_READ_BYTE(uint8 Add,uint8 REG_add,uint8 *Data)
{
	Bool ret = E_OK;
	I2C_GEN_START_COND(); //initiate Start Condition on SCL/SDA
	I2C_WAIT();
	switch (I2C_STATUS) {
	case TWSR_START_MSTR:
	case TWSR_REPEATED_START_MSTR:
		break;
	case TWSR_ARBIT_LOSS:
		ret = E_NOK; //Error
	} //start bit sent SDA switched from high to low bus is busy hence send address
	if (ret == E_OK) {
		I2C_WRITE_BYTE(Add | I2C_WRITE);
		switch (I2C_STATUS) {
		case TWSR_MSTR_ADD_ACK_REC:
			break;
		case TWSR_MSTR_ADD_NACK_REC:
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		} //salve+W/R transmitted send data
	}
	if (ret == E_OK) {
		I2C_WRITE_BYTE(REG_add | I2C_WRITE);
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		}
	}
	if (ret == E_OK) {
		I2C_GEN_START_COND(); //initiate Repeated Start Condition on SCL/SDA
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_REPEATED_START_MSTR:
		case TWSR_START_MSTR:
			break;
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		}
	}
	if (ret == E_OK) //send data
	{
		I2C_WRITE_BYTE(Add | I2C_READ);
		switch (I2C_STATUS) {
		case TW_MSR_SLA_R_ACK:
			break;
		case TW_MSR_SLA_R_NACK:
		case TWSR_ARBIT_LOSS:
			ret = E_NOK;
			break;
		default:
			ret = E_NOK;
		}
	}
	if (ret == E_OK) {
		I2C_READ_NACK();
		I2C_WAIT();
		*Data = I2C_WRITE_BUFFER;
		if (I2C_STATUS != TW_MSR_DATA_R_NACK)
			ret = E_NOK;
	}
	return ret;
}

Bool I2C_READ_NBYTES(uint8 Add,uint8 REG_add,uint8 *Data,int8 No_bytes)
{
	Bool ret = E_OK;
	I2C_GEN_START_COND(); //initiate Start Condition on SCL/SDA
	I2C_WAIT();
	switch (I2C_STATUS) {
		case TWSR_START_MSTR:
		case TWSR_REPEATED_START_MSTR:
			break;
		case TWSR_ARBIT_LOSS:
			ret = E_NOK; //Error
	} //start bit sent SDA switched from high to low bus is busy hence send address
	if (ret == E_OK) {
		I2C_WRITE_BYTE(Add | I2C_WRITE);
		switch (I2C_STATUS) {
			case TWSR_MSTR_ADD_ACK_REC:
				break;
			case TWSR_MSTR_ADD_NACK_REC:
			case TWSR_ARBIT_LOSS:
				ret = E_NOK;
				break;
			default:
				ret = E_NOK;
		} //salve+W/R transmitted send data
	}
	if (ret == E_OK) {
		I2C_WRITE_BYTE(REG_add | I2C_WRITE);
		switch (I2C_STATUS) {
			case TWSR_MSTR_DATA_ACK_REC:
				break;
			case TWSR_MSTR_DATA_NACK_REC:
			case TWSR_ARBIT_LOSS:
				ret = E_NOK;
				break;
			default:
				ret = E_NOK;
			}
	}
	if (ret == E_OK) {
		I2C_GEN_START_COND(); //initiate Repeated Start Condition on SCL/SDA
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TWSR_REPEATED_START_MSTR:
			case TWSR_START_MSTR:
				break;
			case TWSR_ARBIT_LOSS:
				ret = E_NOK;
				break;
			default:
				ret = E_NOK;
		}
	}
	if(ret==E_OK)//send data
	{
		I2C_WRITE_BYTE(Add | I2C_READ);
		switch (I2C_STATUS)
		{
			case TW_MSR_SLA_R_ACK:
				break;
			case TW_MSR_SLA_R_NACK:
			case TWSR_ARBIT_LOSS:
				ret = E_NOK;
				break;
			default:
				ret = E_NOK;
		}
	}
	if (ret == E_OK) {
		for(;No_bytes>0;No_bytes--)
		{
				if(No_bytes==1)
				{
					I2C_READ_NACK();
					I2C_WAIT();
					*Data++=I2C_WRITE_BUFFER;
					if(I2C_STATUS!=TW_MSR_DATA_R_NACK)
							ret=E_NOK;
				}
				else
				{
					I2C_READ_ACK();
					I2C_WAIT();
					*Data++ = I2C_WRITE_BUFFER;
					if(I2C_STATUS!=TW_MSR_DATA_R_ACK)
					{
						ret=E_NOK;
						break;
					}
				}
		}
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return ret;
}

#if I2C_RECIEVE_INTERRUPT==FEATURE_ENABLE
void I2C_ISR_()
{
	I2C_CLEAR_FLAG();
	if(I2C_RX_CALLBACK_PTR)
		I2C_RX_CALLBACK_PTR();
}
#endif
