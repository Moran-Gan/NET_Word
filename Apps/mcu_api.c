/**********************************Copyright (c)**********************************
**                       版权所有 (C), 
**
**                             http://
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  电控部
 * @version v1.0.0
 * @date    2023.08.14
 * @brief   串口接口
 */
#include	"Dwin_CONF.h"
#include	"user_include.h"
#include	"Apps\wifi.h"

/*
*函数：void	Connect_Modbus_03_Fountion(uint16_t	addr,uint16_t	len,int16_t	*Data)
*功能：03指令，主动上报数据到云端使用
*/
void	Connect_Modbus_03_Fountion(uint16_t	addr,uint16_t	len,int16_t	*Data)
{
	uint8_t	i,j;
	uint16_t	crc_data;
	Connect_TXbuf[0]	=	0x01;
	Connect_TXbuf[1]	=	0x03;
	Connect_TXbuf[2]	=	addr	>> 8;
	Connect_TXbuf[3]	=	addr	>>	0;
	Connect_TXbuf[4]	=	len		>>	8;
	Connect_TXbuf[5]	=	len		>>	0;
	crc_data	=	crc16(&Connect_TXbuf[0],6);
	Connect_TXbuf[6]	=	crc_data	>>	0;
	Connect_TXbuf[7]	=	crc_data	>>	8;
	
	Connect_TXbuf[8]	=	0x01;
	Connect_TXbuf[9]	=	0x03;
	Connect_TXbuf[10]	=	len *2;
		
	j	=	0;
	for(i = 0;i < len;i++)
	{
		Connect_TXbuf[11 + j]				=	Data[i] 	>>8;
		Connect_TXbuf[11 + (j + 1)]	=	Data[i] 	>>0;
		j	+=	2;
	}	
	crc_data	=	crc16(&Connect_TXbuf[8],len*2+3);
	Connect_TXbuf[(len*2+3) + 8] = (u8)crc_data;
	Connect_TXbuf[(len*2+4) + 8] = (u8)(crc_data>>8);			
}

/*
*函数：void	Connect_Modbus_03_Fountion(uint16_t	addr,uint16_t	len,int16_t	*Data)
*功能：06指令，应答云端下发指令使用
*/
void	Connect_Modbus_06_Fountion(uint16_t	addr,int16_t	Data)
{
	uint16_t	crc_data;
	Connect_TXbuf[0]	=	0x01;
	Connect_TXbuf[1]	=	0x06;
	Connect_TXbuf[2]	=	addr 	>>8;
	Connect_TXbuf[3]	=	addr 	>>0;
	Connect_TXbuf[4]	=	Data 	>>8;
	Connect_TXbuf[5]	=	Data 	>>0;
	crc_data	=	crc16(Connect_TXbuf,6);
	Connect_TXbuf[6]	=	crc_data 	>>0;
	Connect_TXbuf[7]	=	crc_data 	>>8;
}

/*
*函数：void	Connect_SendStr(uint8_t *pstr,uint16_t strlen)
*功能：串口发送函数,需要根据硬件进行修改
*/
void	Connect_SendStr(uint8_t *pstr,uint16_t strlen)
{
	if((NULL == pstr)||(0 == strlen))
    {
        return;
    }
    while(strlen--)
    {
        UART3_SendData(*pstr);
        pstr++;
    }
		if(Connect_Send_fault_Count	<	60)														//防止超范围
			Connect_Send_fault_Count++;																//累计发送失败次数
}


