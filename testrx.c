#include "pyinclude.h"
#include "NRF24L01.h"
int main(void)
{
    unsigned char tmp_buf[8]={0};
	if(SETUP_OK!=sunxi_gpio_init())
	{
	        printf("Failed to initialize GPIO\n");
	        return -1;
	}
	init_NRF24L01();
	TX_Mode();
	while(NRF24L01_Check()==1)
	{
		printf("Ini 24L01 Failed !!!\n");
		delay_ms(200);
	}

	printf("Test NRF24L01 and Ds18b20:\n");

	//-----------------------------------------------------
	while(1)
	{
		if(nRF24L01_RxPacket(tmp_buf)==1)
		{
			printf("%x   ",tmp_buf[0] );
			printf("%x   ",tmp_buf[1] );
			printf("%x  \n",tmp_buf[2] );
		}
		RX_Mode();
		delay_ms(200);
	}
	sunxi_gpio_cleanup();
	return 0;
}

