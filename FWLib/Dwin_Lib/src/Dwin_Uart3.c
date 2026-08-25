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
void	UART3_Init(uint32_t My_BaudRate)
{
	MUX_SEL |= 0X20;																//bit5置1表示将uart3接口引出到P0.6和P0.7
	P0MDOUT &= 0x3f;
	P0MDOUT |= 0x40;																//p0.7 = 0,p0.6 = 1
	SCON1 = 0XD0;//0X90;
	SCON1 = 0XD0;//0X90;
  SREL1H = (1024-(CPU_CLK/(32*My_BaudRate)))>>8;  //
  SREL1L = (1024-(CPU_CLK/(32*My_BaudRate)));			
  SET_BIT(IEN2,0);																			//串口3中断使能
}


/*****************************************************
*函数名称：void USCI0_TWI_SendData(uint8_t Data)
*函数功能：TWI发送数据
*入口参数：Data		发送的数据
*出口参数：void
*****************************************************/
void UART3_TWI_SendData(uint8_t Data)
{
	SBUF1 = Data;	
}


/*****************************************************
*函数名称：uint8_t USCI0_TWI_SendData(void)
*函数功能：获得US0CON3中的值
*入口参数：void
*出口参数：uint8_t	
*****************************************************/
uint8_t UART3_TWI_ReceiveData(void)
{
	return SBUF1;
}

/*****************************************************
*函数名称：FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
*函数功能：获得UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：FlagStatus		UART0中断标志状态	
*****************************************************/
FlagStatus UART3_GetFlagStatus(UART3_Flag_Typedef UART3_Flag) //reentrant
{
	return (SCON1 & UART3_Flag);//(bool)(SCON1 & UART3_Flag);
}


/*****************************************************
*函数名称：void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
*函数功能：清除UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：void	
*****************************************************/
void UART3_ClearFlag(UART3_Flag_Typedef UART3_Flag)
{
	SCON1 &=	(~UART3_Flag);																	//清除SCON1标志位需要连续清空二次
	SCON1 &=	(~UART3_Flag);
}

void UART3_SendData(uint8_t Data)
{
	//UART3_TWI_SendData(Data);
	SBUF1	=	Data;
	while(GET_BIT(Uart_TX_Status,Uart3));
	SET_BIT(Uart_TX_Status,Uart3);
}

void	UART3_SendStr(uint8_t *pstr,uint16_t strlen)
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
}
