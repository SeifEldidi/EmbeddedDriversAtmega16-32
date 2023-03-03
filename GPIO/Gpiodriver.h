
#ifndef _GPIO_DRIVER_H
#define _GPIO_DRIVER_H

/************Includes Section**************/
#include "mcalStdtypes.h"

/************Define Section******************/
#define PORT_MAX 4
#define PIN_MAX 8
#define GPIO_LOW 0
#define GPIO_HIGH 1
/***********Macro Like Function**************/
#define SHL(REG,BIT_POS) ((REG)<<BIT_POS)
#define SHR(REG,BIT_POS) ((REG)>>BIT_POS)
/*************Defined Data types**************/
typedef uint8 Logic_Level;

typedef enum
{
	PORTA_INDEX=0,
	PORTB_INDEX,
	PORTC_INDEX,
	PORTD_INDEX,
}PORT_INDEX;

typedef enum
{
	GPIO_PIN0,
	GPIO_PIN1,
	GPIO_PIN2,
	GPIO_PIN3,
	GPIO_PIN4,
	GPIO_PIN5,
	GPIO_PIN6,
	GPIO_PIN7,
}GPIO_PIN_ORDER;

typedef enum
{
	GPIO_INPUT,
	GPIO_OUTPUT,
	GPIO_INPUT_PULLUP,
}GPIO_STATE;

typedef struct
{
	uint8 GPIO_PORT:2;
	uint8 GPIO_PIN:3;
	uint8 GPIO_STATE:2;
	uint8 GPIO_LOGIC_LEVEL:1;
}GPIO_CONFIG_t;

/*********Software Interfaces*******************/
Bool GPIO_PIN_CONFIG(GPIO_CONFIG_t *GPIO_PIN);
Bool GPIO_PIN_READ(GPIO_CONFIG_t *GPIO_PIN,Logic_Level *Level);
Bool GPIO_PIN_WRITE(GPIO_CONFIG_t *GPIO_PIN,Logic_Level Level);
Bool GPIO_PIN_Toggle(GPIO_CONFIG_t *GPIO_PIN);

Bool GPIO_PORT_CONFIG(PORT_INDEX Port,uint8 Config);
Bool GPIO_PORT_WRITE(PORT_INDEX Port,Logic_Level Level);
Bool GPIO_PORT_READ(PORT_INDEX Port,Logic_Level *Level);
Bool GPIO_PORT_Toggle(PORT_INDEX Port);

#endif
