/*
 * dht11.h
 *
 *  Created on: Mar 22, 2023
 *      Author: Seif pc
 */

#ifndef DHT11_H_
#define DHT11_H_

/************Includes Section**************/
#include "mcalStdtypes.h"
#include "Gpiodriver.h"
#include "Interrupts.h"
#include "spi.h"
#include "Timer2.h"
/************Define Section******************/
#define Word_size 16
#define BYTE_SIZE 8

#define DHT11_PORT_DIR DDRA
#define DHT11_PORT_WRITE PORTA
#define DHT11_PORT PINA
#define DHT11_PIN PA3

/*************Defined Data types**************/
typedef struct
{
	GPIO_CONFIG_t DHT_PIN;
}DHT11_CONFIG_t;

/*********Software Interfaces*******************/
Bool DHT11_INIT(DHT11_CONFIG_t *DHT11);
Bool DHT11_SEND_START();
Bool DHT11_RX_RESP(DHT11_CONFIG_t *DHT11);
Bool DHT11_FETCH_DATA(DHT11_CONFIG_t *DHT11,uint8 *Arr);



#endif /* DHT11_H_ */
