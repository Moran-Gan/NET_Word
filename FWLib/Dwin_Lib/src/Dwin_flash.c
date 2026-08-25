#include	"Dwin_CONF.h"




//检查指定DGUS变量地址的DATA3清零的话就退出
void wait_ok(uint16_t addr)
{	
	ADR_H=0x00;
  ADR_M=(uint8_t)(addr>>8);
  ADR_L=(uint8_t)(addr);
  ADR_INC=0x00;
	do
	{ 
		for(addr=0;addr<1000;addr++)	//释放变量空间一段时间
		{ 
			DATA2=DATA1;
		}
		RAMMODE=0xAF;
		while(APP_ACK==0);
		APP_EN=1;
		while(APP_EN==1);
		RAMMODE=0x00;
	}	
	while(DATA3!=0);
}
//读写T5L片内256KW Flash，mod=0x5A 为读取，mod=0xA5为写入
//addr=DGUS变量地址，必须是偶数；addr_flash=flash读取地址，必须是偶数；len=读取字长度，必须是偶数。
void T5L_Flash(uint8_t mod,uint16_t addr,long addr_flash,uint32_t len)
{	
	ADR_H=0x00;
  ADR_M=0x00;
  ADR_L=0x04;
  ADR_INC=0x01;
  RAMMODE=0x8F;		
	while(APP_ACK==0);
	DATA3=mod;
	DATA2=(uint8_t)(addr_flash>>16);
	DATA1=(uint8_t)(addr_flash>>8);
	DATA0=(uint8_t)(addr_flash&0xFE);
	APP_EN=1;	
	while(APP_EN==1);
	DATA3=(uint8_t)(addr>>8);
	DATA2=(uint8_t)(addr&0xFE);
	DATA1=(uint8_t)(len>>8);
	DATA0=(uint8_t)(len&0xFE);
	APP_EN=1;
	while(APP_EN==1);
	RAMMODE=0x00;
	wait_ok(0x0004);
}	
