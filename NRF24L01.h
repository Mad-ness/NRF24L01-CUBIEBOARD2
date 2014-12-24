#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include <stdlib.h>
#include <stdio.h>

#include "pyinclude.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

#define   RF24L01_CE		sunxi_gpio_set_cfgpin(PD0,OUTPUT)
#define   RF24L01_CSN		sunxi_gpio_set_cfgpin(PD1,OUTPUT)
#define   RF24L01_SCK		sunxi_gpio_set_cfgpin(PD2,OUTPUT)
#define   RF24L01_MOSI	sunxi_gpio_set_cfgpin(PD3,OUTPUT)
#define   RF24L01_MISO	sunxi_gpio_set_cfgpin(PD4,INPUT)
#define   RF24L01_IRQ		sunxi_gpio_set_cfgpin(PD5,OUTPUT)
#define   RF24L01_IRQ_R     sunxi_gpio_input(PD4)

//==============================================================================
#define  RF24L01_CE_0          sunxi_gpio_output(PD0,LOW)
#define  RF24L01_CE_1          sunxi_gpio_output(PD0,HIGH)  
//=============================RF24L01_CSN端口==================================
#define  RF24L01_CSN_0       sunxi_gpio_output(PD1,LOW)
#define  RF24L01_CSN_1       sunxi_gpio_output(PD1,HIGH)  
//=============================RF24L01_SCK======================================
#define  RF24L01_SCK_0        sunxi_gpio_output(PD2,LOW)
#define  RF24L01_SCK_1        sunxi_gpio_output(PD2,HIGH)  
//========================== RF24L01_MOSI端口================================
#define  RF24L01_MOSI_0      sunxi_gpio_output(PD3,LOW)
#define  RF24L01_MOSI_1      sunxi_gpio_output(PD3,HIGH)  
//=============================MISO端口=========================================

#define RF24L01_MISO_PIN   sunxi_gpio_input(PD4)

//==========================IRQ状态============================================
#define  RF24L01_IRQ_0       sunxi_gpio_output(PD5,LOW)
#define  RF24L01_IRQ_1       sunxi_gpio_output(PD5,HIGH)  
//==============================================================================

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
//=========================NRF24L01寄存器指令===================================
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP1            0xFF  	// 保留
//========================SPI(nRF24L01)寄存器地址===============================
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
//=============================RF24l01状态=====================================

extern void NRF24L01_IO_set(void);
extern uchar SPI_RW(uchar data);
extern void inerDelay_us(uchar n);
extern uchar SPI_Read(uchar reg);
extern uchar SPI_RW_Reg(uchar reg, uchar value);
extern uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar chars);
extern uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar chars);
extern void SetRX_Mode(void);
extern uchar nRF24L01_RxPacket(uchar* rx_buf);
extern void nRF24L01_TxPacket(uchar * tx_buf);
extern void init_NRF24L01(void);

extern void RX_Mode(void);
extern void TX_Mode(void);

extern uchar NRF24L01_Check(void);

#endif
