#include	"Dwin_CONF.h"
#include 	<string.h>

/*****************************************************************************
 函 数 名  : void Uart2_Init(u32 My_BaudRate)
 功能描述  : 串口2初始化
 输入参数  : My_BaudRate（波特率）
 输出参数  : 无
 修改历史  :
*****************************************************************************/
void	UART4_Init(uint32_t My_BaudRate)
{
	P0MDOUT |= 0x01;																//0=P0.0 输出关闭 1=P0.0 输出开启（推挽输出）。
  SCON2T = 0x80;																	//串口4使能发送
  SCON2R = 0x80;																	//串口4使能接收
  BODE2_DIV_H = (CPU_CLK/(8*My_BaudRate))>>8;    	//
  BODE2_DIV_L = (CPU_CLK/(8*My_BaudRate));
  ES2R = true;																		//使能串口4接收中断
}


/*****************************************************
*函数名称：void USCI0_TWI_SendData(uint8_t Data)
*函数功能：TWI发送数据
*入口参数：Data		发送的数据
*出口参数：void
*****************************************************/
void UART4_TWI_SendData(uint8_t Data)
{
	SBUF2_TX = Data;	
}


/*****************************************************
*函数名称：uint8_t USCI0_TWI_SendData(void)
*函数功能：获得US0CON3中的值
*入口参数：void
*出口参数：uint8_t	
*****************************************************/
uint8_t UART4_TWI_ReceiveData(void)
{
	return SBUF2_RX;
}

/*****************************************************
*函数名称：FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
*函数功能：获得UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：FlagStatus		UART0中断标志状态	
*****************************************************/
FlagStatus UART4_GetFlagStatus(UART4_Flag_Typedef UART4_Flag)
{
	uint8_t	SCON;
	if(UART4_Flag == UART4_FLAG_RI)
																						SCON  =	SCON2R;
	else  if(UART4_Flag == UART4_FLAG_TI)
																						SCON  =	SCON2T;
	
	return (SCON & 0x01);//(bool)(SCON & 0x01);
}

FlagStatus UART4_RX_GetFlagStatus(void)
{
	return (SCON2R & 0x01);
}

FlagStatus UART4_TX_GetFlagStatus(void)
{
	return (SCON2T & 0x01);
}

/*****************************************************
*函数名称：void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
*函数功能：清除UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：void	
*****************************************************/
void UART4_ClearFlag(UART4_Flag_Typedef UART4_Flag)
{
	if(UART4_Flag == UART4_FLAG_RI)
																						SCON2R &=	(~0x01);//SCON2R & = ~0x01;
	else  if(UART4_Flag == UART4_FLAG_TI)
																						SCON2T &=	(~0x01);//SCON2T & =	~0x01;
}

void UART4_RX_ClearFlag(void)
{
	SCON2R &=	(~0x01);
}

void UART4_TX_ClearFlag(void)
{
	SCON2T &=	(~0x01);
}


void UART4_SendData(uint8_t Data)
{
	UART4_TWI_SendData(Data);
	while(!UART4_TX_GetFlagStatus());
	UART4_TX_ClearFlag();
}

void	UART4_SendStr(uint8_t *pstr,uint16_t strlen)
{
	if((NULL == pstr)||(0 == strlen))
    {
        return;
    }
    while(strlen--)
    {
        UART4_SendData(*pstr);
        pstr++;
    }
}