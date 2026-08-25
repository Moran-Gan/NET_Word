#include	"Dwin_CONF.h"


/******开启看门狗*********/
void	WDT_ON(void)	
{
	MUX_SEL|=0x02;		
}

/******关闭看门狗*********/
void	WDT_OFF(void)	
{
	MUX_SEL&=0xFD;		
}

/******喂狗*********/
void	WDT_RST(void)	
{
	MUX_SEL|=0x01;		
}
	
	