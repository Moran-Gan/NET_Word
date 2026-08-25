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
void	UART2_Init(uint32_t My_BaudRate)
{
	ADCON = 0x80;																		//使用 SREL0H:L
  SCON0 = 0x50;																		//7=SM0  .6=SM1  .5=SM2（多机通信位）.4=REN0  .3=TB80  .2=RB80  .1=TI0  .0=RI0
	PCON = 0x80;																		//波特率倍频  Bit7 == 0不倍频，1 == 倍频
  SREL0H = (1024-(CPU_CLK/(64*My_BaudRate)))>>8;  //计算波特兰
  SREL0L = (1024-(CPU_CLK/(64*My_BaudRate)));			
  ES0 = true;																			//串口2中断使能
}


/*****************************************************
*函数名称：void USCI0_TWI_SendData(uint8_t Data)
*函数功能：TWI发送数据
*入口参数：Data		发送的数据
*出口参数：void
*****************************************************/
void UART2_TWI_SendData(uint8_t Data)
{
	SBUF0 = Data;	
}


/*****************************************************
*函数名称：uint8_t USCI0_TWI_SendData(void)
*函数功能：获得US0CON3中的值
*入口参数：void
*出口参数：uint8_t	
*****************************************************/
uint8_t UART2_TWI_ReceiveData(void)
{
	return SBUF0;
}

/*****************************************************
*函数名称：FlagStatus UART0_GetFlagStatus(UART0_Flag_Typedef UART0_Flag)
*函数功能：获得UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：FlagStatus		UART0中断标志状态	
*****************************************************/
FlagStatus UART2_GetFlagStatus(UART2_Flag_Typedef UART2_Flag)
{
	return (SCON0 & UART2_Flag);//(bool)(SCON0 & UART2_Flag);
}


/*****************************************************
*函数名称：void UART0_ClearFlag(UART0_Flag_Typedef UART0_Flag)
*函数功能：清除UART0中断标志状态
*入口参数：UART0_Flag	中断标志位选择
*出口参数：void	
*****************************************************/
void UART2_ClearFlag(UART2_Flag_Typedef UART2_Flag)
{
	SCON0 &=	(~UART2_Flag);
}


void UART2_SendData(uint8_t Data)
{
	UART2_TWI_SendData(Data);
	while(GET_BIT(Uart_TX_Status,Uart2));
	SET_BIT(Uart_TX_Status,Uart2);
}

void	UART2_SendStr(uint8_t *pstr,uint16_t strlen)
{
	if((NULL == pstr)||(0 == strlen))
    {
        return;
    }
    while(strlen--)
    {
        UART2_SendData(*pstr);
        pstr++;
    }
}