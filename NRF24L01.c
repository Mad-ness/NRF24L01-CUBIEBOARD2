#include "pyinclude.h"
#include "NRF24L01.h"

/***************************************************************
//模块名称：NRF24L01无线模块
//功    能：数据收发
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

uchar  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
uchar  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址


/*******************************************************************
//函数名称：void NRF24L01_IO_set(void)
//函数功能：NRF24L01引脚初始化
//参    数：无
//返 回 值：无
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
//函数名称：SPI_RW()
//函数功能：完成GPIO模拟SPI的功能，将输出字节（MOSI）从MSB循环输出
//          同时将输入字节（MISO）从LSB循环移入，上升沿读入，下降沿
//          输出。（从SCK被初始化为低电平可以判出）
//参    数：byte(字节)
//返 回 值：byte
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
//函数名称：void inerDelay_us(uchar n)
//函数功能：n 延时数
//参    数：无
//返 回 值：无
********************************************************************/
void inerDelay_us(uchar n)
{
	for(;n>0;n--);
}

/****************************
//函数名称：SPI_Read()
//函数功能：读取寄存器的值
//参    数：reg(寄存器)
//返 回 值：reg_val(寄存器值)
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
//函数名称:SPI_RW_Reg()
//函数功能：寄存器访问函数，把设定的值写进相应的寄存器，并读取返回值
//参    数：reg(寄存器选择)，value(设定值)
//返 回 值：status
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
//函数名称：uchar SPI_Read_Buf（）
//函数功能：接收时读取FIFO缓冲区的值
//参    数：reg(寄存器)，*pBuf(数据存储)，bytes(字节数)
//返 回 值：status
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
//函数名称：SPI_Write_Buf()
//函数功能：发射缓冲区访问，把数组里的数放到FIFO缓冲区中
//参    数：reg(寄存器选择)，*pBuf(待写数据，bytes(字节数))
//返 回 值：status NRF24L01状态位
************************************************************/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar chars)
{
	uchar status1,uchar_ctr;
	RF24L01_CSN_0;             //SPI使能       
	status1 = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<chars; uchar_ctr++) //
	{
		SPI_RW(*pBuf++);
	}
	RF24L01_CSN_1;            //关闭SPI
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
//函数名称：RX_Mode()
//函数功能：设定为接收模式
//参    数：无
//返 回 值：无
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
//函数名称：TX_Mode()
//函数功能：设定24L01发送方式
//参    数：无
//返 回 值：无
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
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0X26);      //TX_PWR:0dBm,Datarate:2Mbps，LNA:HCURR
	SPI_RW_Reg(WRITE_REG +CONFIG,0X0e);         //Set PWR_UP bit,enable CRC(2bytes)&Prim:TX.MAX_RT&TX_DS enabled
	RF24L01_CE_1;
}

//****************************************************************************************************/
//函数：void SetRX_Mode(void)
//功能：数据接收配置 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);  // IRQ收发完成中断响应，16位CRC	，主接收
	RF24L01_CE_1; 
	inerDelay_us(130);    //注意不能太小
}

/*******************************************************************
//函数名称：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//函数功能：数据读取后放入rx_buf接收缓冲区中
//参    数：*rx_buf(待收数据)
//返 回 值：revale (完成标志)
********************************************************************/
uchar nRF24L01_RxPacket(uchar* rx_buf)
{
	uchar revale=0;
	uchar  sta;
	sta=SPI_Read(STATUS);	 // 读取状态寄存其来判断数据接收状况		
	if(sta&0x40)                 // 判断是否接收到数据
	{
		RF24L01_CE_0 ; 			                 //SPI使能
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH); // read receive payload from RX_FIFO buffer
		revale =1;	                 //读取数据完成标志
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);    //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	return revale;
}

/*************************************************
//函数名称：void nRF24L01_TxPacket(char * tx_buf)
//函数功能：发送 tx_buf中数据
//参    数：*tx_buf(待发数据)
//返 回 值：无
***************************************************/
void nRF24L01_TxPacket(uchar * tx_buf)
{
	RF24L01_CE_0 ;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);   // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 	             // 装载数据	
	//SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	RF24L01_CE_1;		 //置高CE，激发数据发送
	inerDelay_us(10);
}
/********************************
//函数名称：init_NRF24L01()
//函数功能：NRF24L01初始化
//参    数：无
//返 回 值：无
*********************************/
void init_NRF24L01(void)
{
	//inerDelay_us(100);
	NRF24L01_IO_set();
	RF24L01_CE_0 ;           // chip enable
	RF24L01_CSN_1;           // Spi disable 
	RF24L01_SCK_0;           // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 40);         //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0f);   //设置发射速率为1MHZ，发射功率为最大值0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0E);     // IRQ收发完成中断响应，16位CRC	，主接收
} 
/********************************
//函数名称：uchar NRF24L01_Check(void)
//函数功能：检查NRF24L01是否连接
//参    数：无
//返 回 值：0连接成功；1连接失败
*********************************/
uchar NRF24L01_Check(void)
{
	uchar buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uchar i;   	 
	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	SPI_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;  //检测24L01错误	
	return 0;		  //检测到24L01
}


