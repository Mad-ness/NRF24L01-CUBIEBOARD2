#include "pyinclude.h"
#include "NRF24L01.h"

int main(void)
{
	unsigned char snd_buf[8]={0};
	if(SETUP_OK!=sunxi_gpio_init())
	{
	        printf("Failed to initialize GPIO\n");
	        return -1;
	}
	init_NRF24L01();
	
	while(NRF24L01_Check()==1)
	{
		printf("Ini 24L01 Failed !!!\n");
		delay_ms(200);
	}
	RX_Mode();

	unsigned char registers[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0d, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x1c, 0x1d};
	unsigned char i,regvalue;
	for(i=0; i<sizeof(registers); i++)
	{
		regvalue = SPI_Read(registers[i]);
		printf("%x:%x\n", registers[i], regvalue);
	}
	printf("Test NRF24L01 and Ds18b20:\n");
	//-----------------------------------------------------
	
	sunxi_gpio_cleanup();
	return 0;
}

