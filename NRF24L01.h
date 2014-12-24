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
//=============================RF24L01_CSN�˿�==================================
#define  RF24L01_CSN_0       sunxi_gpio_output(PD1,LOW)
#define  RF24L01_CSN_1       sunxi_gpio_output(PD1,HIGH)  
//=============================RF24L01_SCK======================================
#define  RF24L01_SCK_0        sunxi_gpio_output(PD2,LOW)
#define  RF24L01_SCK_1        sunxi_gpio_output(PD2,HIGH)  
//========================== RF24L01_MOSI�˿�================================
#define  RF24L01_MOSI_0      sunxi_gpio_output(PD3,LOW)
#define  RF24L01_MOSI_1      sunxi_gpio_output(PD3,HIGH)  
//=============================MISO�˿�=========================================

#define RF24L01_MISO_PIN   sunxi_gpio_input(PD4)

//==========================IRQ״̬============================================
#define  RF24L01_IRQ_0       sunxi_gpio_output(PD5,LOW)
#define  RF24L01_IRQ_1       sunxi_gpio_output(PD5,HIGH)  
//==============================================================================

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
//=========================NRF24L01�Ĵ���ָ��===================================
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP1            0xFF  	// ����
//========================SPI(nRF24L01)�Ĵ�����ַ===============================
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//=============================RF24l01״̬=====================================

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
