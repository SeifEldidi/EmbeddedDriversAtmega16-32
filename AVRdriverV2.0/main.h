/*
 * main.h
 *
 *  Created on: May 25, 2023
 *      Author: Seif pc
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "ECAL/Ecual_DHT11.h"
#include "ECAL/Ecual_Ultrasonic.h"
#include "MCAL/Mcal_gpio.h"
#include "MCAL\mcal_std_types.h"
#include "MCAL\macl_adc.h"
#include "MCAL\mcal_uart.h"
#include "MCAL\mcal_timer0.h"
#include "ECAL\Ecual_lcd.h"
#include "MCAL\mcal_spi.h"
#include "ECAL\Ecual_max7219.h"
#include "ECAL\Ecual_rtc.h"
#include "Mcal\BitBangedUart.h"

#define Pressed 1
#define NOT_Pressed 0

void main_init();
uint8 StrCmp(char *Str_1,char *Str_2);

#endif /* MAIN_H_ */
