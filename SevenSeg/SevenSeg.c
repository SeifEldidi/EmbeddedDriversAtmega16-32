/*
 * SevenSeg.c
 *
 *  Created on: Mar 12, 2023
 *      Author: Seif pc
 */

#include "SevenSeg.h"

Bool SEVEN_SEG_init(SEVEN_SEG_7 *Seg)
{
	Bool ret=E_OK;
	uint8 Iter=0;
	for(Iter=0;Iter<=DIGS-1;Iter++)
		GPIO_PIN_CONFIG(&Seg->dig[Iter]);
	for(Iter=0;Iter<=NO_SEGS-1;Iter++)
			GPIO_PIN_CONFIG(&Seg->Control[Iter]);
	return ret;
}

Bool SEVEN_SEG_display(SEVEN_SEG_7 *Seg,uint16 val,uint8 *Timerflag)
{
	Bool ret=E_OK;
	uint8 Segs_i=0;
	uint8 Segs=0;
	static uint8 no_written=0;
//	if (*Timerflag) {
//		Segs=Segs==4?Segs=0:Segs+1;
//	}
	for(Segs=0;Segs<=NO_SEGS-1;Segs++)
	{
	for (Segs_i = 0; Segs_i <= NO_SEGS - 1; Segs_i++)
		GPIO_PIN_WRITE(&Seg->Control[Segs_i], GPIO_LOW);
	GPIO_PIN_WRITE(&Seg->Control[Segs], GPIO_HIGH);
//	if (!no_written || *Timerflag) {
		for (Segs_i = 0; Segs_i <= DIGS - 1; Segs_i++)
			GPIO_PIN_WRITE(&Seg->dig[Segs_i], GPIO_LOW);
		if (Segs == 3) {
			uint8 Write = (val % 100) % 10;
			GPIO_PIN_WRITE(&Seg->dig[0], (Write >> 0 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[1], (Write >> 1 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[2], (Write >> 2 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[3], (Write >> 3 & BIT_MASK));
		} else if (Segs == 2) {
			uint8 Write = (val % 100) / 10;
			GPIO_PIN_WRITE(&Seg->dig[0], (Write >> 0 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[1], (Write >> 1 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[2], (Write >> 2 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[3], (Write >> 3 & BIT_MASK));
		} else if (Segs == 1) {
			uint8 Write = (val / 100) % 10;
			GPIO_PIN_WRITE(&Seg->dig[0], (Write >> 0 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[1], (Write >> 1 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[2], (Write >> 2 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[3], (Write >> 3 & BIT_MASK));
		} else if (Segs == 0) {
			uint8 Write = (val / 1000);
			GPIO_PIN_WRITE(&Seg->dig[0], (Write >> 0 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[1], (Write >> 1 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[2], (Write >> 2 & BIT_MASK));
			GPIO_PIN_WRITE(&Seg->dig[3], (Write >> 3 & BIT_MASK));
//		}
//		no_written = 1;
//		*Timerflag = 0;
	}
	_delay_ms(5);
	}
	return ret;
}
