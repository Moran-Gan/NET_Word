#ifndef	_Dwin_int_h_
#define	_Dwin_int_h_

//电源开关宏定义
//#define POWER_ON()	P2_1=1;
//#define POWER_OFF()	P2_1=0;

sbit RTC_SDA = P3^3;
sbit RTC_SCL = P3^2;

sbit RS485_TX_EN=P0^1;
sbit RS485_TX_EN_TR4=P0^0;

sbit WIFI_RST=P2^4;//P2^4;

sbit WIFI_EN=P2^6;

//电源开关引脚
sbit P2_1=P2^1;
//电压反馈，低电平表示按键有效
sbit KEY1_Vin_F=P2^2;

//sbit L610_OPEN = P1^2;
// ????
sbit DHT83_I2C_SCL_PORT = P2 ^ 1; 
sbit DHT83_I2C_SDA_PORT = P2 ^ 0; 

extern	void	Dwin_int(void);



#endif