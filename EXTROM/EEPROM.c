/*
 * EEPROM.c
 *
 *  Created on: Apr 11, 2023
 *      Author: Seif pc
 */
#include "EEPROM.h"

Bool EPPROM_WRITE_BYTE(uint16 EEPROM_Address,uint8 Data)
{
	Bool E_stat=E_OK;
	I2C_GEN_START_COND();
	I2C_WAIT();
	switch (I2C_STATUS) {
		case TWSR_START_MSTR:
		case TWSR_REPEATED_START_MSTR:
			break;
		case TWSR_ARBIT_LOSS://arbitration lost
			E_stat = E_NOK; //Error
	}
	if(E_stat==E_OK){
		I2C_WRITE_BYTE(EEPROM_CONTROL_ADD| I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TWSR_MSTR_ADD_ACK_REC:
				break;
			case TWSR_MSTR_ADD_NACK_REC:
			case TWSR_ARBIT_LOSS:
				E_stat = E_NOK;
				break;
			default:
				E_stat = E_NOK;
		}
	}
	if(E_stat==E_OK){
		I2C_WRITE_BYTE(((EEPROM_Address>>8)&0x0F)| I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TWSR_MSTR_DATA_ACK_REC:
				break;
			case TWSR_MSTR_DATA_NACK_REC:
			case TWSR_ARBIT_LOSS:
				E_stat = E_NOK;
				break;
			default:
				E_stat = E_NOK;
		}
	}
	if(E_stat==E_OK){
		I2C_WRITE_BYTE((uint8)(EEPROM_Address) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TWSR_MSTR_DATA_ACK_REC:
				break;
			case TWSR_MSTR_DATA_NACK_REC:
			case TWSR_ARBIT_LOSS:
				E_stat = E_NOK;
				break;
			default:
				E_stat = E_NOK;
		}
	}
	if(E_stat==E_OK){
		I2C_WRITE_BYTE(Data);
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TWSR_MSTR_DATA_ACK_REC:
				break;
			case TWSR_MSTR_DATA_NACK_REC:
			case TWSR_ARBIT_LOSS:
				E_stat = E_NOK;
				break;
			default:
				E_stat = E_NOK;
		}
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return E_stat;
}

Bool EEPROM_READ_BYTE(uint16 EEPROM_Address,uint8 *Data)
{
	Bool E_stat = E_OK;
	I2C_GEN_START_COND();
	I2C_WAIT();
	switch (I2C_STATUS) {
	case TWSR_START_MSTR:
	case TWSR_REPEATED_START_MSTR:
		break;
	case TWSR_ARBIT_LOSS: //arbitration lost
		E_stat = E_NOK; //Error
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(EEPROM_CONTROL_ADD | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_ADD_ACK_REC:
			break;
		case TWSR_MSTR_ADD_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(((EEPROM_Address >> 8) & 0x0F) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE((uint8) (EEPROM_Address) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	I2C_GEN_START_COND();
	I2C_WAIT();
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(EEPROM_CONTROL_ADD | I2C_READ);
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TW_MSR_SLA_R_ACK:
				break;
			case TW_MSR_SLA_R_NACK:
			case TWSR_ARBIT_LOSS:
				E_stat = E_NOK;
				break;
			default:
				E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_READ_NACK();
		I2C_WAIT();
		*Data = I2C_WRITE_BUFFER;
		if (I2C_STATUS != TW_MSR_DATA_R_NACK)
			E_stat = E_NOK;
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return E_stat;
}

Bool EEPROM_WRTIE_PAGE(uint16 EEPROM_Address,uint8 *Data,uint8 Len)
{
	Bool E_stat = E_OK;
	I2C_GEN_START_COND();
	I2C_WAIT();
	switch (I2C_STATUS) {
	case TWSR_START_MSTR:
	case TWSR_REPEATED_START_MSTR:
		break;
	case TWSR_ARBIT_LOSS: //arbitration lost
		E_stat = E_NOK; //Error
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(EEPROM_CONTROL_ADD | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_ADD_ACK_REC:
			break;
		case TWSR_MSTR_ADD_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(((EEPROM_Address >> 8) & 0x0F) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE((uint8) (EEPROM_Address) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		uint8 Bytes_Written=0;
		for(Bytes_Written=0;Bytes_Written<=Len-1;Bytes_Written++)
		{
			I2C_WRITE_BYTE(Data[Bytes_Written]);
			I2C_WAIT();
			switch (I2C_STATUS) {
				case TWSR_MSTR_DATA_ACK_REC:
					break;
				case TWSR_MSTR_DATA_NACK_REC:
				case TWSR_ARBIT_LOSS:
					E_stat = E_NOK;
					break;
				default:
					E_stat = E_NOK;
			}
			if(E_stat==E_NOK)
				break;
		}
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return E_stat;
}

Bool EEPROM_READ_PAGE(uint16 EEPROM_Address,uint8 *Data,uint8 len)
{
	Bool E_stat = E_OK;
	I2C_GEN_START_COND();
	I2C_WAIT();
	switch (I2C_STATUS) {
	case TWSR_START_MSTR:
	case TWSR_REPEATED_START_MSTR:
		break;
	case TWSR_ARBIT_LOSS: //arbitration lost
		E_stat = E_NOK; //Error
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(EEPROM_CONTROL_ADD | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_ADD_ACK_REC:
			break;
		case TWSR_MSTR_ADD_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(((EEPROM_Address >> 8) & 0x0F) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE((uint8) (EEPROM_Address) | I2C_WRITE);
		I2C_WAIT();
		switch (I2C_STATUS) {
		case TWSR_MSTR_DATA_ACK_REC:
			break;
		case TWSR_MSTR_DATA_NACK_REC:
		case TWSR_ARBIT_LOSS:
			E_stat = E_NOK;
			break;
		default:
			E_stat = E_NOK;
		}
	}
	I2C_GEN_START_COND();
	I2C_WAIT();
	if (E_stat == E_OK) {
		I2C_WRITE_BYTE(EEPROM_CONTROL_ADD | I2C_READ);
		I2C_WAIT();
		switch (I2C_STATUS) {
			case TW_MSR_SLA_R_ACK:
				break;
			case TW_MSR_SLA_R_NACK:
			case TWSR_ARBIT_LOSS:
				E_stat = E_NOK;
				break;
			default:
				E_stat = E_NOK;
		}
	}
	if (E_stat == E_OK) {
		for (; len > 0; len--) {
			if (len == 1) {
				I2C_READ_NACK();
				I2C_WAIT();
				*Data++ = I2C_WRITE_BUFFER;
				if (I2C_STATUS != TW_MSR_DATA_R_NACK)
					E_stat = E_NOK;
			} else {
				I2C_READ_ACK();
				I2C_WAIT();
				*Data++ = I2C_WRITE_BUFFER;
				if (I2C_STATUS != TW_MSR_DATA_R_ACK) {
					E_stat = E_NOK;
					break;
				}
			}
		}
	}
	I2C_GEN_STOP_COND();
	I2C_WAIT_STOP();
	return E_stat;
}
