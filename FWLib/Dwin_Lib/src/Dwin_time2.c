#include	"Dwin_CONF.h"

#define		T2_CPU_CLK_MHZ			206.4384

/*****************************************************************************
 函 数 名  : void T2_Init(u32 Time_uS))
 功能描述  : 串口2初始化
 输入参数  : Time_uS
 输出参数  : 无
 修改历史  :
*****************************************************************************/
void TIM2_Init(uint32_t Time_uS)
{
    T2CON = 0x70;																						//时钟分频选择：CPU 主频/12
    TH2 = 0x00;																							//溢出自动重载
    TL2 = 0x00;																							//溢出自动重载
    TRL2H = (int)(65536-(Time_uS*(T2_CPU_CLK_MHZ/12)))>>8;
    TRL2L = (int)(65536-(Time_uS*(T2_CPU_CLK_MHZ/12)));        
    IEN0 |= 0x20;     																			//开启定时器2
    TR2 = 0x01;																							//运行T2
}

/*****************************************************
*函数名称：void TIM3_ClearFlag(TIM3_Flag_TypeDef TIM3_Flag)
*函数功能：清除TIMER3中断标志状态
*入口参数：TIM3_Flag   TIMER3标志选择
*出口参数：void	
*****************************************************/
void TIM2_ClearFlag(void)
{
	TF2 = false;	
}