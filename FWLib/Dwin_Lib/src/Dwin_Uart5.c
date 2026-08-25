#include	"Dwin_CONF.h"
#include 	<string.h>
#include	"HeadFiles\user_global.h"

/*****************************************************************************
 函 数 名  : void Uart2_Init(u32 My_BaudRate)
 功能描述  : 串口2初始化
 输入参数  : My_BaudRate（波特率）
 输出参数  : 无
 修改历史  :
*****************************************************************************/
void	UART5_Init(uint32_t My_BaudRate)
{
	SCON3T = 0x80;																	//串口发送使能8Bit
	SCON3R = 0x80;																	//串口接收使能
  BODE3_DIV_H = (CPU_CLK/(8*My_BaudRate))>>8;			//计算波特率     
  BODE3_DIV_L = (CPU_CLK/(8*My_BaudRate));
	ES3R = true;																		//使能串口5接收中断使能
	ES3T = true;																		//使能串口5发送中断使能
}


/*****************************************************
*函数名称：void USCI0_TWI_SendData(uint8_t Data)
*函数功能：TWI发送数据
*入口参数：Data		发送的数据
*出口参数：void
*****************************************************/
void UART5_TWI_SendData(uint8_t Data)
{
	SBUF3_TX = Data;	
}


/*****************************************************
*函数名称：uint8_t USCI0_TWI_SendData(void)
*函数功能：获得US0CON3中的值
*入口参数：void
*出口参数：uint8_t	
*****************************************************/
uint8_t UART5_TWI_ReceiveData(void)
{
	return SBUF3_RX;
}

/*****************************************************
*函数名称：FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
*函数功能：获得UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：FlagStatus		UART0中断标志状态	
*****************************************************/
FlagStatus UART5_GetFlagStatus(UART5_Flag_Typedef UART5_Flag)
{
	uint8_t	SCON;
	if(UART5_Flag == UART5_FLAG_RI)
																						SCON  =	SCON3R;
	else  if(UART5_Flag == UART5_FLAG_TI)
																						SCON  =	SCON3T;
	
	return (SCON & 0x01);//(bool)(SCON & 0x01);
}

FlagStatus UART5_RX_GetFlagStatus(void)
{
	return (SCON3R & 0x01);
}

FlagStatus UART5_TX_GetFlagStatus(void)
{
	return (SCON3T & 0x01);
}
/*****************************************************
*函数名称：void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
*函数功能：清除UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：void	
*****************************************************/
void UART5_ClearFlag(UART5_Flag_Typedef UART5_Flag)
{
	if(UART5_Flag == UART5_FLAG_RI)
																						SCON3R &=	(~0x01);
	else  if(UART5_Flag == UART5_FLAG_TI)
																						SCON3T &=	(~0x01);
}


void UART5_RX_ClearFlag(void)
{
	SCON3R &= (~0x01);																		
}

void UART5_TX_ClearFlag(void)
{
	SCON3T &=	(~0x01);																		
}

void UART5_SendData(uint8_t Data)
{
	SBUF3_TX	=	Data;
	while(GET_BIT(Uart_TX_Status,Uart3));
	SET_BIT(Uart_TX_Status,Uart3);
}

void	UART5_SendStr(uint8_t *pstr,uint16_t strlen)
{
	if((NULL == pstr)||(0 == strlen))
    {
        return;
    }
    while(strlen--)
    {
        UART5_SendData(*pstr);
        pstr++;
    }
}

