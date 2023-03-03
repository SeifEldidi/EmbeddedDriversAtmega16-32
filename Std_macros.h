/*
 * Std_macros.h
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#ifndef STD_MACROS_H_
#define STD_MACROS_H_

#define BIT_MASK 0x01

#define SET_BIT(REG,BIT_POS) ((REG)|=(BIT_MASK<<BIT_POS))
#define CLEAR_BIT(REG,BIT_POS) ((REG)&=~(BIT_MASK<<BIT_POS))
#define TOGGLE_BIT(REG,BIT_POS) ((REG)^=(BIT_MASK<<BIT_POS))
#define BIT_IS_SET(REG,BIT_POS)    ((REG>>BIT_POS)&(BIT_MASK))
#define BIT_IS_CLEAR(REG,BIT_POS)    (!((REG>>BIT_POS)&(BIT_MASK)))
#define READ_BIT(REG,BIT_POS) ((REG>>BIT_POS)&(BIT_MASK))

#endif /* STD_MACROS_H_ */
