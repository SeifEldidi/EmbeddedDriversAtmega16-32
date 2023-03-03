/*
 * mcalstdtypes.h
 *
 *  Created on: Feb 25, 2023
 *      Author: Seif pc
 */

#ifndef _MCAL_STDTYPES_H_
#define _MCAL_STDTYPES_H_

#include "Std_macros.h"
#include "stdlibraries.h"

#define FEATURE_ENABLE 1
#define FEATURE_DISABLE 0
#define E_OK 1
#define E_NOK 0
#define NULL ((void *)0)

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned long uint32;
typedef signed long int32;
typedef unsigned long long uint64;
typedef signed long long  int64;

typedef float float32;
typedef double float64;

typedef unsigned char Bool;

#endif
