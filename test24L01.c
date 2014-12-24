#include "pyinclude.h"
#include "NRF24L01.h"

int main(int argc,char*argv[])
{
	unsigned char tmp_buf[8]={0};
	unsigned char snd_buf[8]={0};
	unsigned char sta;
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

	printf("Test NRF24L01 and Ds18b20:\n");
	RF24L01_IRQ_1;
        SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	//-----------------------------------------------------
	TX_Mode();
	delay_ms(200);
	int on = atoi(argv[1]);
	if(on == 1)
	{
		printf(" on \n");
		snd_buf[0] = 0xaa;
	}else{
		printf(" off \n");
		snd_buf[0] = 0x55;
	}
	nRF24L01_TxPacket(snd_buf);
	while(RF24L01_IRQ_R);
	sta = SPI_Read(STATUS);
	SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	delay_ms(200);
	
	RX_Mode();
	RF24L01_IRQ_1;
	delay_ms(200);
		
	sunxi_gpio_cleanup();
	return 0;
}

