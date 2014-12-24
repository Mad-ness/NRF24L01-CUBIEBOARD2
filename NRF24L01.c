#include "pyinclude.h"
#include "NRF24L01.h"

/***************************************************************
//ģ�����ƣ�NRF24L01����ģ��
//��    �ܣ������շ�
//            -----------------
//           |                 |
//           |             PD0|--->CE
//           |             PD1|--->CSN
//           |             PD2|--->SCK
//           |             PD3|-->MOSI 
//           |             PD4|<--MISO
//           |             PD5|<--IRQ
//           |                 |
//            -----------------
****************************************************************/

uchar  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
uchar  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ


/*******************************************************************
//�������ƣ�void NRF24L01_IO_set(void)
//�������ܣ�NRF24L01���ų�ʼ��
//��    ������
//�� �� ֵ����
********************************************************************/
void NRF24L01_IO_set(void)
{
	RF24L01_CE;
	RF24L01_CSN;
	RF24L01_SCK;
	RF24L01_MOSI;
	RF24L01_MISO;
	RF24L01_IRQ;

}

/*******************************************************************
//�������ƣ�SPI_RW()
//�������ܣ����GPIOģ��SPI�Ĺ��ܣ�������ֽڣ�MOSI����MSBѭ�����
//          ͬʱ�������ֽڣ�MISO����LSBѭ�����룬�����ض��룬�½���
//          ���������SCK����ʼ��Ϊ�͵�ƽ�����г���
//��    ����byte(�ֽ�)
//�� �� ֵ��byte
********************************************************************/
uchar SPI_RW(uchar data)
{
	uchar i,temp=0;
	for(i=0;i<8;i++) // output 8-bit
	{
		if((data & 0x80)==0x80)
		{
			    RF24L01_MOSI_1;   // output 'uchar', MSB to MOSI
		}
		else
		{
			    RF24L01_MOSI_0; 
		}	
		data = (data << 1);           // shift next bit into MSB..
		temp<<=1;
		RF24L01_SCK_1;                // Set SCK high..
		if((RF24L01_MISO_PIN)==1)temp++;  // capture current MISO bit
		RF24L01_SCK_0;                // ..then set SCK low again
	}
	return(temp);           // return read uchar
}

/*******************************************************************
//�������ƣ�void inerDelay_us(uchar n)
//�������ܣ�n ��ʱ��
//��    ������
//�� �� ֵ����
********************************************************************/
void inerDelay_us(uchar n)
{
	for(;n>0;n--);
}

/****************************
//�������ƣ�SPI_Read()
//�������ܣ���ȡ�Ĵ�����ֵ
//��    ����reg(�Ĵ���)
//�� �� ֵ��reg_val(�Ĵ���ֵ)
*****************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	RF24L01_CSN_0;          // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	RF24L01_CSN_1;          // CSN high, terminate SPI communication
	return(reg_val);        // return register value
}

/******************************************************************
//��������:SPI_RW_Reg()
//�������ܣ��Ĵ������ʺ��������趨��ֵд����Ӧ�ļĴ���������ȡ����ֵ
//��    ����reg(�Ĵ���ѡ��)��value(�趨ֵ)
//�� �� ֵ��status
********************************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status1;
	RF24L01_CSN_0;             // CSN low, init SPI transaction
	status1 = SPI_RW(reg);     // select register
	SPI_RW(value);             // ..and write value to it..
	RF24L01_CSN_1;             // CSN high again
	return(status1);           // return nRF24L01 status uchar
}

/********************************************************
//�������ƣ�uchar SPI_Read_Buf����
//�������ܣ�����ʱ��ȡFIFO��������ֵ
//��    ����reg(�Ĵ���)��*pBuf(���ݴ洢)��bytes(�ֽ���)
//�� �� ֵ��status
*********************************************************/
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar chars)
{
	uchar status2,uchar_ctr;
	RF24L01_CSN_0;                   // Set CSN low, init SPI tranaction
	status2 = SPI_RW(reg);           // Select register to write to and read status uchar
	for(uchar_ctr=0;uchar_ctr<chars;uchar_ctr++)
	{
		pBuf[uchar_ctr] = SPI_RW(0);  
	}
	RF24L01_CSN_1;                           
	return(status2);                 // return nRF24L01 status uchar
}

/***********************************************************
//�������ƣ�SPI_Write_Buf()
//�������ܣ����仺�������ʣ�������������ŵ�FIFO��������
//��    ����reg(�Ĵ���ѡ��)��*pBuf(��д���ݣ�bytes(�ֽ���))
//�� �� ֵ��status NRF24L01״̬λ
************************************************************/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar chars)
{
	uchar status1,uchar_ctr;
	RF24L01_CSN_0;             //SPIʹ��       
	status1 = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<chars; uchar_ctr++) //
	{
		SPI_RW(*pBuf++);
	}
	RF24L01_CSN_1;            //�ر�SPI
	return(status1);          // 
}

void power_off(void)
{
    RF24L01_CE_0;
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0D); 
    RF24L01_CE_1;
    inerDelay_us(20);
}

/**************************
//�������ƣ�RX_Mode()
//�������ܣ��趨Ϊ����ģʽ
//��    ������
//�� �� ֵ����
***************************/
void RX_Mode(void)
{
	RF24L01_CE_0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH);
	
	SPI_RW_Reg(WRITE_REG + EN_AA,0X01);     //Enable Auto.ACK:Pipe0
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0X01); //Enable Pipe0
	SPI_RW_Reg(WRITE_REG + RF_CH,40);       //Selsct RF channl 40
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,TX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0X26);
	SPI_RW_Reg(WRITE_REG + CONFIG,0X0f);   //Set PWR_UP bit ,enable CRC(2bytes)&Prim:RX.RX_DR enabled
	RF24L01_CE_1;                                  //Set CE pin high to enable RX device  
}

/*****************************
//�������ƣ�TX_Mode()
//�������ܣ��趨24L01���ͷ�ʽ
//��    ������
//�� �� ֵ����
******************************/
void TX_Mode(void)
{
	RF24L01_CE_0;
	SPI_Write_Buf(WRITE_REG + TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH);
	
	SPI_RW_Reg(WRITE_REG + EN_AA,0X01);         //Enable Auto.Ack:Pipe0
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0X01);     //Enable Pipe0
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0X1a);    //500us + 80us,10 retrans
	SPI_RW_Reg(WRITE_REG + RF_CH,40);           //Select RF channal 40
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0X26);      //TX_PWR:0dBm,Datarate:2Mbps��LNA:HCURR
	SPI_RW_Reg(WRITE_REG +CONFIG,0X0e);         //Set PWR_UP bit,enable CRC(2bytes)&Prim:TX.MAX_RT&TX_DS enabled
	RF24L01_CE_1;
}

//****************************************************************************************************/
//������void SetRX_Mode(void)
//���ܣ����ݽ������� 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);  // IRQ�շ�����ж���Ӧ��16λCRC	��������
	RF24L01_CE_1; 
	inerDelay_us(130);    //ע�ⲻ��̫С
}

/*******************************************************************
//�������ƣ�unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//�������ܣ����ݶ�ȡ�����rx_buf���ջ�������
//��    ����*rx_buf(��������)
//�� �� ֵ��revale (��ɱ�־)
********************************************************************/
uchar nRF24L01_RxPacket(uchar* rx_buf)
{
	uchar revale=0;
	uchar  sta;
	sta=SPI_Read(STATUS);	 // ��ȡ״̬�Ĵ������ж����ݽ���״��		
	if(sta&0x40)                 // �ж��Ƿ���յ�����
	{
		RF24L01_CE_0 ; 			                 //SPIʹ��
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH); // read receive payload from RX_FIFO buffer
		revale =1;	                 //��ȡ������ɱ�־
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);    //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}

/*************************************************
//�������ƣ�void nRF24L01_TxPacket(char * tx_buf)
//�������ܣ����� tx_buf������
//��    ����*tx_buf(��������)
//�� �� ֵ����
***************************************************/
void nRF24L01_TxPacket(uchar * tx_buf)
{
	RF24L01_CE_0 ;			//StandBy Iģʽ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);   // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 	             // װ������	
	//SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	RF24L01_CE_1;		 //�ø�CE���������ݷ���
	inerDelay_us(10);
}
/********************************
//�������ƣ�init_NRF24L01()
//�������ܣ�NRF24L01��ʼ��
//��    ������
//�� �� ֵ����
*********************************/
void init_NRF24L01(void)
{
	//inerDelay_us(100);
	NRF24L01_IO_set();
	RF24L01_CE_0 ;           // chip enable
	RF24L01_CSN_1;           // Spi disable 
	RF24L01_SCK_0;           // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 40);         //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0f);   //���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0E);     // IRQ�շ�����ж���Ӧ��16λCRC	��������
} 
/********************************
//�������ƣ�uchar NRF24L01_Check(void)
//�������ܣ����NRF24L01�Ƿ�����
//��    ������
//�� �� ֵ��0���ӳɹ���1����ʧ��
*********************************/
uchar NRF24L01_Check(void)
{
	uchar buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uchar i;   	 
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	SPI_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;  //���24L01����	
	return 0;		  //��⵽24L01
}


