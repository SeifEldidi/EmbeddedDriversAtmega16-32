#include "Gpiodriver.h"

uint8 *PORTX_REGS[]={&PORTA,&PORTB,&PORTC,&PORTD};
uint8 *DDRX_REGS[]={&DDRA,&DDRB,&DDRC,&DDRD};
uint8 *PINX_REGS[]={&PINA,&PINB,&PINC,&PIND};

static Bool PIN_CONFIG(GPIO_CONFIG_t *GPIO_PIN);

Bool GPIO_PIN_CONFIG(GPIO_CONFIG_t *GPIO_PIN)
{
	Bool ret=E_OK;
	if(NULL==GPIO_PIN||GPIO_PIN->GPIO_PIN>PIN_MAX-1||GPIO_PIN->GPIO_PORT>PORT_MAX-1)
		ret=E_NOK;
	else
	{
		PIN_CONFIG(GPIO_PIN);
	}
	return ret;
}

Bool GPIO_PIN_READ(GPIO_CONFIG_t *GPIO_PIN,Logic_Level *Level)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN)
		ret = E_NOK;
	else {
		*Level=READ_BIT(*PINX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
	}
	return ret;
}

Bool GPIO_PIN_WRITE(GPIO_CONFIG_t *GPIO_PIN,Logic_Level Level)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN)
		ret = E_NOK;
	else {
		switch(Level)
		{
			case GPIO_HIGH:
				SET_BIT(*PORTX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
				break;
			case GPIO_LOW:
				CLEAR_BIT(*PORTX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
				break;
			default:
				ret=E_NOK;
		}
	}
	return ret;
}

Bool GPIO_PIN_Toggle(GPIO_CONFIG_t *GPIO_PIN)
{
	Bool ret = E_OK;
	if (NULL == GPIO_PIN)
		ret = E_NOK;
	else {
		TOGGLE_BIT(*PORTX_REGS[GPIO_PIN->GPIO_PORT], GPIO_PIN->GPIO_PIN);
	}
	return ret;
}

Bool GPIO_PORT_CONFIG(PORT_INDEX Port,uint8 Config)
{
	Bool ret = E_OK;
	if(Port>PORT_MAX-1)
		ret=E_NOK;
	else
	{
		*DDRX_REGS[Port] = Config;
	}
	return ret;
}

Bool GPIO_PORT_WRITE(PORT_INDEX Port,Logic_Level Level){
	Bool ret = E_OK;
	if (Port>PORT_MAX-1)
		ret = E_NOK;
	else {
		*PORTX_REGS[Port] = Level;
	}
	return ret;
}

Bool GPIO_PORT_READ(PORT_INDEX Port,Logic_Level *Level)
{
	Bool ret = E_OK;
	if (Port>PORT_MAX-1||NULL == Level)
		ret = E_NOK;
	else {
		*Level=*PINX_REGS[Port];
	}
	return ret;
}

Bool GPIO_PORT_Toggle(PORT_INDEX Port)
{
	Bool ret = E_OK;
		if (Port>PORT_MAX-1)
			ret = E_NOK;
		else {
			*PORTX_REGS[Port]=~(*PORTX_REGS[Port]);
		}
		return ret;
}


static Bool PIN_CONFIG(GPIO_CONFIG_t *GPIO_PIN)
{
	Bool ret=E_OK;
	switch(GPIO_PIN->GPIO_STATE)
	{
		case GPIO_INPUT:
			CLEAR_BIT(*DDRX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
			break;
		case GPIO_OUTPUT:
			SET_BIT(*DDRX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
			CLEAR_BIT(*PORTX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
			break;
		case GPIO_INPUT_PULLUP:
			CLEAR_BIT(*DDRX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
			SET_BIT(*PORTX_REGS[GPIO_PIN->GPIO_PORT],GPIO_PIN->GPIO_PIN);
			break;
		default :
			ret=E_NOK;
	}
	return ret;
}
