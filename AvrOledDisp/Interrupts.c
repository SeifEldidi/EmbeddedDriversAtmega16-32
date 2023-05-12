/*
 * Interrupts.c
 *
 *  Created on: Feb 26, 2023
 *      Author: Seif pc
 */
#include "Interrupts.h"

#if EXT_INT0_FEATURE_ENABLE==FEATURE_ENABLE
	static void (*INT0_Callback_Ptr)(void);
#endif
#if EXT_INT1_FEATURE_ENABLE==FEATURE_ENABLE
	static void (*INT1_Callback_Ptr)(void);
#endif
#if EXT_INT2_FEATURE_ENABLE==FEATURE_ENABLE
	static void (*INT2_Callback_Ptr)(void);
#endif

static inline void Ext_INT_Disable()
{
	GLOBAL_INT_DISABLE();
	EXT_INT0_MODULE_DISABLE();
	EXT_INT1_MODULE_DISABLE();
	EXT_INT2_MODULE_DISABLE();
}

static Bool Ext_INT1_init(EXT_INT_CONFIG_t *Int)
{
	Bool ret = E_OK;
#if EXT_INT1_FEATURE_ENABLE==FEATURE_ENABLE
	switch (Int->Sense_Control) {
	case EXT_INT_LOW_LEVEL:
			CLEAR_BIT(MCUCR, ISC10);
			CLEAR_BIT(MCUCR, ISC11);
			break;
	case EXT_INT_LEVEL_CHANGE:
			SET_BIT(MCUCR, ISC10);
			CLEAR_BIT(MCUCR, ISC11);
			break;
	case EXT_INT_FALLING_EDGE:
			CLEAR_BIT(MCUCR, ISC10);
			SET_BIT(MCUCR, ISC11);
		    break;
	case EXT_INT_RISING_EDGE:
			SET_BIT(MCUCR, ISC10);
			SET_BIT(MCUCR, ISC11);
			break;
	default:
		ret = E_NOK;
	}
	INT1_Callback_Ptr=Int->INT1_Callback;
#endif
	return ret;
}

static Bool Ext_INT0_init(EXT_INT_CONFIG_t *Int)
{
	Bool ret=E_OK;
#if EXT_INT0_FEATURE_ENABLE==FEATURE_ENABLE
	switch(Int->Sense_Control)
	{
		case EXT_INT_LOW_LEVEL:
					CLEAR_BIT(MCUCR,ISC00);
					CLEAR_BIT(MCUCR,ISC01);
					break;
		case EXT_INT_LEVEL_CHANGE:
				   SET_BIT(MCUCR,ISC00);
				   CLEAR_BIT(MCUCR,ISC01);
				   break;
		case EXT_INT_FALLING_EDGE:
		     	   CLEAR_BIT(MCUCR,ISC00);
				   SET_BIT(MCUCR,ISC01);
				   break;
		case EXT_INT_RISING_EDGE:
				   SET_BIT(MCUCR,ISC00);
				   SET_BIT(MCUCR,ISC01);
				   break;
		default:
			ret=E_NOK;
	}
	INT0_Callback_Ptr=Int->INT0_Callback;
#endif
	return ret;
}

static Bool Ext_INT2_init(EXT_INT_CONFIG_t *Int)
{
	Bool ret=E_OK;
#if EXT_INT2_FEATURE_ENABLE==FEATURE_ENABLE
	switch(Int->Sense_Control)
	{
		case EXT_INT2_FALLING_EDGE:
			EXT_INT2_FEDG();
			break;
		case EXT_INT2_RISING_EDGE:
			EXT_INT2_RSEDG();
			break;
		default:
			ret=E_NOK;
	}
	INT2_Callback_Ptr=Int->INT2_Callback;
#endif
	return ret;
}

Bool EXT_Int_init(EXT_INT_CONFIG_t *Int)
{
	Bool ret=E_OK;
	if(NULL==Int)
		ret=E_NOK;
	else
	{
		Ext_INT_Disable();
		switch(Int->Pin)
		{
			case EXT_INT0:
				GLOBAL_INT_ENABLE();
				Ext_INT0_init(Int);
				EXT_INT0_FLAG_CLEAR();
				EXT_INT0_MODULE_ENABLE();
				break;
			case EXT_INT1:
				GLOBAL_INT_ENABLE();
				Ext_INT1_init(Int);
				EXT_INT1_FLAG_CLEAR();
				EXT_INT1_MODULE_ENABLE();
				break;
			case  EXT_INT2:
				GLOBAL_INT_ENABLE();
				Ext_INT2_init(Int);
				EXT_INT2_FLAG_CLEAR();
				EXT_INT2_MODULE_ENABLE();
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

void INT0_ISR_()
{
	EXT_INT0_FLAG_CLEAR();
	if(INT0_Callback_Ptr)
		INT0_Callback_Ptr();
}

void INT1_ISR_()
{
	EXT_INT1_FLAG_CLEAR();
	if(INT1_Callback_Ptr)
		INT1_Callback_Ptr();
}

void INT2_ISR_()
{
	EXT_INT2_FLAG_CLEAR();
	if(INT2_Callback_Ptr)
		INT2_Callback_Ptr();
}

ISR(INT0_vect)
{
	INT0_ISR_();
}

ISR(INT1_vect)
{
	INT1_ISR_();
}

ISR(INT2_vect)
{
	INT2_ISR_();
}



