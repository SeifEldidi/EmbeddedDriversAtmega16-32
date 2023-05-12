#include "main.h"




int main()
{
	uint8 i=0;
	uint8 Time_Buff[15];
	uint8 Date_Buff[15];
	Bool E_stat=E_OK;
	E_stat=OLED_Init();
	E_stat=RTCDS1307_SET_TIME(MODE_PM,25,45,9);
	E_stat=RTCDS1307_SET_DATE(TUESDAY,2,5,23);
	OLED_DISPLAY();
	while(1)
	{
//		E_stat=RTCDS1307_TIME_STR(Time_Buff);
//		E_stat=RTCDS1307_DATE_STR(Date_Buff);
//		E_stat=OLED_DISPLAY_String(Date_Buff,6,1,2);
//		E_stat=OLED_DISPLAY_String(Time_Buff,24,4,2);
	}
	return 0;
}



