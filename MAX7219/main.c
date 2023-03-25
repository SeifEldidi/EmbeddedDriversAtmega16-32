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


uint16 Temp,Humidity;
uint8 Temp_data[5];
float temp;

int main()
{
	Bool ret;
	uint8 Temp_high=0;
	uint8 Temp_Low=0;
	ret=SPI_INIT(&SPI);
	ret=MAX7219_INIT();
	ret=DHT11_INIT(&DHT11);
	while(1)
	{
		ret=DHT11_FETCH_DATA(&DHT11,Temp_data);
		if(ret==E_OK)
		{
			Temp_high=Temp_data[2];
			Temp_Low=Temp_data[3];
			uint32 Temp=Temp_high;
			temp=Temp_high+((float)Temp_Low/1000);
			MAX7219Disp_Number(Temp);
		}
		else{}
	}
	return 0;
}



