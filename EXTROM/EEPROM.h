/*
 * EEPROM.h
 *
 *  Created on: Apr 11, 2023
 *      Author: Seif pc
 */

#ifndef EEPROM_H_
#define EEPROM_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "stdlibraries.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
#include "I2C.h"
/************Define Section******************/
#define EEPROM_CONTROL_ADD 0xA0
/*************Defined Data types**************/

/*********Software Interfaces*******************/
Bool EPPROM_WRITE_BYTE(uint16 EEPROM_Address,uint8 Data);
Bool EEPROM_READ_BYTE(uint16 EEPROM_Address,uint8 *Data);
Bool EEPROM_WRTIE_PAGE(uint16 EEPROM_Address,uint8 *Data,uint8 Len);
Bool EEPROM_READ_PAGE(uint16 EEPROM_Address,uint8 *Data,uint8 len);

#endif /* EEPROM_H_ */
