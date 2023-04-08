#include "main.h"


SPI_CONFIG_t SPI={
		.SPI_MSTR_SLAVE_set=SPI_MSTR_MODE,
		.SPI_Shift_dir_set=MSB_FIRST_SHIFT,
		.SPI_CLOCK_phase_set=CLOCK_PHASE_SAMPLE_TRAILING,
		.SPI_CLOCK_polarity_set=CLOCK_POLARITY_RSEDG_FEDG,
		.CLK_set=SPI_FOSC_4,
};

DHT11_CONFIG_t DHT11={
		.DHT_PIN.GPIO_PORT=PORTA_INDEX,
		.DHT_PIN.GPIO_PIN=GPIO_PIN3,
		.DHT_PIN.GPIO_STATE=GPIO_OUTPUT,
		.DHT_PIN.GPIO_LOGIC_LEVEL=GPIO_LOW,
};

I2C_CONFIG_t I2C_t={
		.SCL_FREQ=I2C_STANDARD_MODE,
		.Prescaler_Freq=TWPS_PRESCALER_1,
};

uint16 Temp,Humidity;
uint8 Temp_data[5];
uint8 Time[3];
float temp;
int32 Disp;

int main()
{
	Bool ret;
	uint8 Temp_high=0;
	uint8 Temp_Low=0;
	ret=I2C_INIT(&I2C_t);
	RTCDS1307_SET_DATE(SATURDAY,8,4,25);
	RTCDS1307_SET_TIME(MODE_24H,50,12,5);
	ret=SPI_INIT(&SPI);
	ret=MAX7219_INIT_ARR();
	ret=DHT11_INIT(&DHT11);
	while(1)
	{
		ret=DHT11_FETCH_DATA(&DHT11,Temp_data);
		if(ret==E_OK)
		{
			RTCDS1307_GET_TIME(Time);
			Disp=(uint32)Time[2]*10000+Time[1]*100+Time[0];
			Temp_high=Temp_data[2];
			Temp_Low=Temp_data[3];
			temp=Temp_high+((float)Temp_Low/1000);
			MAX7219Disp_float_num(temp,1,Disp);
		}
		else{}
	}
	return 0;
}



