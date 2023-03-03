/*
 * Leddriver.c
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#include "LedDriver.h"

extern uint8 *PORTX_REGS[];
extern uint8 *DDRX_REGS[];

Bool LED_CONFIG(LED_t *GPIO_PIN)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN || GPIO_PIN->LED_PIN > PIN_MAX - 1
			|| GPIO_PIN->LED_PORT > PORT_MAX - 1)
		ret = E_NOK;
	else {
		SET_BIT(*DDRX_REGS[GPIO_PIN->LED_PORT],GPIO_PIN->LED_PIN);
	}
	return ret;
}
Bool LED_on(LED_t *GPIO_PIN)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN || GPIO_PIN->LED_PIN > PIN_MAX - 1
			|| GPIO_PIN->LED_PORT > PORT_MAX - 1)
		ret = E_NOK;
	else {
		SET_BIT(*PORTX_REGS[GPIO_PIN->LED_PORT],GPIO_PIN->LED_PIN);
	}
	return ret;
}
Bool LED_of(LED_t *GPIO_PIN)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN || GPIO_PIN->LED_PIN > PIN_MAX - 1
			|| GPIO_PIN->LED_PORT > PORT_MAX - 1)
		ret = E_NOK;
	else {
		CLEAR_BIT(*PORTX_REGS[GPIO_PIN->LED_PORT], GPIO_PIN->LED_PIN);
	}
	return ret;
}
Bool LED_TOGGLE(LED_t *GPIO_PIN)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN || GPIO_PIN->LED_PIN > PIN_MAX - 1
			|| GPIO_PIN->LED_PORT > PORT_MAX - 1)
		ret = E_NOK;
	else {
		TOGGLE_BIT(*PORTX_REGS[GPIO_PIN->LED_PORT], GPIO_PIN->LED_PIN);
	}
	return ret;
}

