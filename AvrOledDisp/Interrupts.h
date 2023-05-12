/*
 * Interrupts.h
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
/************Define Section******************/
#define EXT_INT0_FEATURE_ENABLE FEATURE_ENABLE
#define EXT_INT1_FEATURE_ENABLE FEATURE_ENABLE
#define EXT_INT2_FEATURE_ENABLE FEATURE_ENABLE
#define ADC_INTERRUPT_ENABLE FEATURE_ENABLE
#define USART_INTERRUPT_ENABLE FEATURE_DISABLE
#define TIMER0_INTERRUPT_ENABLE FEATURE_DISABLE
#define TIMER1_INTERRUPT_OVF_ENABLE FEATURE_ENABLE
#define TIMER1_INTERRUPT_CMPA_ENABLE FEATURE_DISABLE
#define TIMER1_INTERRUPT_CMPB_ENABLE FEATURE_DISABLE
#define TIMER1_INTERRUPT_ICP_ENABLE FEATURE_ENABLE
#define I2C_RECIEVE_INTERRUPT FEATURE_DISABLE

#define GLOBAL_INTERRUPT_ENABLE 7

/***********Macro Like Function**************/
#define GLOBAL_INT_ENABLE()  (SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE))
#define GLOBAL_INT_DISABLE() (SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE))

#define EXT_INT0_MODULE_ENABLE()  (SET_BIT(GICR,INT0))
#define EXT_INT0_MODULE_DISABLE() (CLEAR_BIT(GICR,INT0))

#define EXT_INT1_MODULE_ENABLE()  (SET_BIT(GICR,INT1))
#define EXT_INT1_MODULE_DISABLE() (CLEAR_BIT(GICR,INT1))

#define EXT_INT2_MODULE_ENABLE()  (SET_BIT(GICR,INT2))
#define EXT_INT2_MODULE_DISABLE() (CLEAR_BIT(GICR,INT2))

#define EXT_INT0_FLAG_CLEAR() (SET_BIT(GIFR,INTF0))
#define EXT_INT1_FLAG_CLEAR() (SET_BIT(GIFR,INTF1))
#define EXT_INT2_FLAG_CLEAR() (SET_BIT(GIFR,INTF2))

#define EXT_INT2_RSEDG() (CLEAR_BIT(MCUCSR,ISC2))
#define EXT_INT2_FEDG() (SET_BIT(MCUCSR,ISC2))
/*************Defined Data types**************/

typedef enum
{
	EXT_INT0,
	EXT_INT1,
	EXT_INT2,
}EXT_INT_PIN;

typedef enum
{
	EXT_INT_LOW_LEVEL,
	EXT_INT_LEVEL_CHANGE,
	EXT_INT_RISING_EDGE,
	EXT_INT_FALLING_EDGE,
	EXT_INT2_FALLING_EDGE,
	EXT_INT2_RISING_EDGE,
}EXT_INT_SENSE_CONTROL;

typedef struct
{
	EXT_INT_PIN Pin;
	EXT_INT_SENSE_CONTROL Sense_Control;
#if EXT_INT0_FEATURE_ENABLE==FEATURE_ENABLE
	void(*INT0_Callback)(void);
#endif
#if EXT_INT1_FEATURE_ENABLE==FEATURE_ENABLE
	void(*INT1_Callback)(void);
#endif
#if EXT_INT2_FEATURE_ENABLE==FEATURE_ENABLE
	void(*INT2_Callback)(void);
#endif
}EXT_INT_CONFIG_t;

/*********Software Interfaces*******************/
void ADC_ISR_();
void USART_RX_ISR();
void USART_TX_ISR();
void Timer1_ovf_ISR();
void Timer1_capt_ISR();
Bool EXT_Int_init(EXT_INT_CONFIG_t *Int);
#if I2C_RECIEVE_INTERRUPT==FEATURE_ENABLE
void I2C_ISR_();
#endif

#endif /* INTERRUPTS_H_ */
