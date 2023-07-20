/*
 * MCAL_MACROS.h
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_MACROS_H_
#define MCAL_MCAL_MACROS_H_
/****************Define Section********************/
#define BIT_MASK 0x01
/***********Macro LikeFunction***********/
#define CLEAR_BIT(REG,BIT_POS) ((REG)&=~(1<<BIT_POS))
#define SET_BIT(REG,BIT_POS)   ((REG)|=(1<<BIT_POS))
#define TOGGLE_BIT(REG,BIT_POS) ((REG)^=(1<<BIT_POS))
#define READ_BIT(REG,BIT_POS)   ((REG>>BIT_POS)&(BIT_MASK))
#define SHL_REG(REG,SH_AM)    (REG<<SH_AM)
#define SHR_REG(REG,SH_AM)    (REG>>SH_AM)
#define ROR(REG,ROT_AM)       (REG=(REG>>ROT_AM)|(REG<<(sizeof(REG)-ROT_AM)))
#define ROL(REG,ROT_AM)       (REG=(REG<<ROT_AM)|(REG>>(sizeof(REG)-ROT_AM)))
#define WRITE_BIT(REG,BIT_POS,BIT_VAL) {\
	if(BIT_VAL==1)\
	  ((REG)|=(1<<BIT_POS));\
	else if(BIT_VAL==0)\
	  ((REG)&=~(1<<BIT_POS));\
}
#endif /* MCAL_MCAL_MACROS_H_ */
