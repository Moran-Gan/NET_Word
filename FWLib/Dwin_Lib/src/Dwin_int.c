#include	"Dwin_CONF.h"


/*****************************************************************************
 函 数 名  : void PORT_Init(void)
 功能描述  : 端口初始化函数
 输入参数  :	
 输出参数  : 
 修改历史  :
  1.日    期   :2020年6月1日
    作    者   : ganboqing
    修改内容   : 创建
*****************************************************************************/ 
void PORT_Init(void)
{
		RS0=0;
    RS1=0;

    PORTDRV=0x01;   //驱动强度+/-8mA
    IEN0=0x00;      //关闭所有中断
    IEN1=0x00;
    IEN2=0x00;
    IP0=0x00;      //中断优先级默认
    IP1=0x00;

		 //IO口配置
    P0=0x00;
    P1=0x00;
    P2=0x00;
    P3=0x00;
    P0MDOUT=0x10;
//	P0MDOUT=0x00;
    P1MDOUT=0x00;
    P2MDOUT=0x00; 
    P3MDOUT=0x0C;
		
		P1MDOUT |= 0x1E;	//初始化P1.1 P1.2 P1.3 P1.4为输出
		P2MDOUT |= 0x53;	//初始化P2.0 P2.1为输出
		P0MDOUT |= 0x03;	//初始化P0.1为输出，485控制
		P3MDOUT=0x0C;
		
		P1=0x04;

}

void	Dwin_int(void)
{
	PORT_Init();
	TIM2_Init(1000);
	UART3_Init(115200);
	UART5_Init(9600);
	write_Button_Set(0);									//默认关闭声音
	RS485_TX_EN = false;									//开启485接收
	init_rtc();
//	write_Button(1);
//	WDT_ON();
}








