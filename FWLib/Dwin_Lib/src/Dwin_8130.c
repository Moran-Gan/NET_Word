#include	"Dwin_CONF.h"
#include	"HeadFiles\user_global.h"
#include "HeadFiles\user_include.h"

//年参数
//const uint8_t code table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
//const uint8_t code time_set_init[6]={19,10,20,12,00,00};
const uint8_t code mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};


/***************************时钟*****************************************************/
//检查8130有没有掉电，掉电则初始化8130，设置时间为2017.01.01 星期一 00:00:00
void init_rtc(void)
{	
	unsigned char i;
	//检查有没有掉电
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x1d);
	i2cstop();
	i2cstart();
	i2cbw(0x65);
	i=i2cbr();
	mack();
	i2cbr();
	mnak();
	i2cstop();
	if((i&0x02)==0x02)
	{	
		//重新配置时间
		i2cstart();		//30=00
		i2cbw(0x64);
		i2cbw(0x30);
		i2cbw(0x00);
		i2cstop();
		i2cstart();		//1C-1F=48 00 40 10
		i2cbw(0x64);
		i2cbw(0x1C);
		i2cbw(0x48);
		i2cbw(0x00);
		i2cbw(0x40);
		i2cbw(0x10);
		i2cstop();
		i2cstart();		//10-16=RTC设置值 BCD格式
		i2cbw(0x64);
		i2cbw(0x10);
		i2cbw(0x00);	//秒
		i2cbw(0x00);	//分
		i2cbw(0x00);	//时
		i2cbw(0x01);	//星期
		i2cbw(0x01);	//日
		i2cbw(0x01);	//月
		i2cbw(0x17);	//年
		i2cstop();
		i2cstart();		//1E-1F 00 10
		i2cbw(0x64);
		i2cbw(0x1E);
		i2cbw(0x00);	
		i2cbw(0x10);			
		i2cstop();
	}
}

void RTC_Set_Time(uint8_t* prtc_set)
{
	i2cstart();		//30=00
	i2cbw(0x64);
	i2cbw(0x30);
	i2cbw(0x00);
	i2cstop();
	i2cstart();		//1C-1F=48 00 40 10
	i2cbw(0x64);
	i2cbw(0x1C);
	i2cbw(0x48);
	i2cbw(0x00);
	i2cbw(0x40);
	i2cbw(0x10);
	i2cstop();
	i2cstart();		//10-16=RTC设置值 BCD格式
	i2cbw(0x64);
	i2cbw(0x10);
	i2cbw(prtc_set[6]);	//秒
	i2cbw(prtc_set[5]);	//分
	i2cbw(prtc_set[4]);	//时
	i2cbw(prtc_set[3]);	//星期
	i2cbw(prtc_set[2]);									//日
	i2cbw(prtc_set[1]);									//月
	i2cbw(prtc_set[0]);									//年
	i2cstop();
	i2cstart();													//1E-1F 00 10
	i2cbw(0x64);
	i2cbw(0x1E);
	i2cbw(0x00);	
	i2cbw(0x10);			
	i2cstop();
}

/*****************************************************
						读时间
******************************************************/
void time_set_init_enter(void)
{
	uint16_t	rtc_set[6];
	read_dgus_vp(RTC,(uint8_t*)&system.year,4);
	rtc_set[0] = system.year + 2000;
	rtc_set[1] = system.month;
	rtc_set[2] = system.date;
	rtc_set[3] = system.hour;
	rtc_set[4] = system.min;
	rtc_set[5] = system.sec;
	
	write_dgus_vp(KEY_SET_RTC,(u8*)rtc_set,6);
}



/*****************************************************
						时间设置
******************************************************/
uint8_t Is_Leap_Year(uint16_t year)
{		
	if(year%4==0) //必须能被4整除
	{
		if(year%100==0) 
		{
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

/*****************************************************************************
 函 数 名  : u8 getWeekdayByYearday(u16 year, u16 month, u16 days)
 功能描述  : 计算周几
 输入参数  :	 年，月，日
 输出参数  : 	周几 0~6 == 周末~周六
 修改历史  :
  1.日    期   :2020年6月2日
    作    者   : ganboqing
    修改内容   : 创建
*****************************************************************************/
uint8_t getWeekdayByYearday(uint16_t year, uint16_t month, uint16_t days)
{
	const uint8_t code mdays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 };
	uint16_t i, y = (year + 2000) - 1;
	for (i=0; i<month; ++i) days += mdays[i];
	if (month>2)
	{
	if (((year%400)==0) || ((year&3)==0 && (year%100))) ++days;
	}
	return (y+y/4-y/100+y/400+days)%7;
}


void time_set_confirm(void)
{
	uint8_t 	m,n;
	uint8_t 	rtc_set[8];
	uint16_t	rtc_set_t[6];
	uint8_t 	week_set = 0;
	read_dgus_vp(KEY_SET_RTC,(u8*)rtc_set_t,6);
	rtc_set_t[0] -= 2000;
	if(rtc_set_t[2] > mon_table[rtc_set_t[1] - 1])
	{
		if(Is_Leap_Year(rtc_set_t[0]))
		{
			if(rtc_set_t[1] == 2)
			{
				rtc_set_t[2] = mon_table[rtc_set_t[1] - 1] + 1;
			}
			else
			{
				rtc_set_t[2] = mon_table[rtc_set_t[1] - 1];
			}
		}
		else
		{
			rtc_set_t[2] = mon_table[rtc_set_t[1] - 1];
		}	
	}
	
	m = rtc_set_t[0] / 10;
	n = rtc_set_t[0] % 10;
	rtc_set[0]=m*16+n;
	m = rtc_set_t[1] / 10;
	n = rtc_set_t[1] % 10;
	rtc_set[1]=m*16+n;
	m = rtc_set_t[2] / 10;
	n = rtc_set_t[2] % 10;
	rtc_set[2]=m*16+n;
	week_set = getWeekdayByYearday(rtc_set_t[0],rtc_set_t[1],rtc_set_t[2]);//RTC_Get_Week(rtc_set_t[0],rtc_set_t[1],rtc_set_t[2]);
	m = week_set / 10;
	n = week_set % 10;
	rtc_set[3]=m*16+n;
	m = rtc_set_t[3] / 10;
	n = rtc_set_t[3] % 10;
	rtc_set[4]=m*16+n;
	m = rtc_set_t[4] / 10;
	n = rtc_set_t[4] % 10;
	rtc_set[5]=m*16+n;
	m = rtc_set_t[5] / 10;
	n = rtc_set_t[5] % 10;
	rtc_set[6]=m*16+n;
	RTC_Set_Time(rtc_set);
}



//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void rdtime(void)
{	
	unsigned char rtcdata[8];
	unsigned char i,n,m;
	i2cstart();
	i2cbw(0x64);
	i2cbw(0x10);
	i2cstop();
	i2cstart();
	i2cbw(0x65);
	for(i=6;i>0;i--)
	{	
		rtcdata[i]=i2cbr();
		mack();
	}
	rtcdata[0]=i2cbr();
	mnak();
	i2cstop();
	for(i=0;i<3;i++)	//年月日转换成HEX
	{	
		n=rtcdata[i]/16;
		m=rtcdata[i]%16;
		rtcdata[i]=n*10+m;
	}
	for(i=4;i<7;i++)	//时分秒转换成HEX
	{	
		n=rtcdata[i]/16;
		m=rtcdata[i]%16;
		rtcdata[i]=n*10+m;
	}
	//处理星期的数据格式
	n=0;
	m=rtcdata[3];
	for(i=0;i<7;i++)
	{   
		if(m&0x01)  
		{
			break;
		}
		n++;
		m=(m>>1);
	}
	rtcdata[3]=n;
	rtcdata[7]=0;
	write_dgus_vp(0x0010,(uint8_t*)rtcdata,4);	//写入DGUS变量空间
}



void	Write_time_set_confirm(uint8_t* time)
{
	uint8_t	Time_Arr[8];
	uint8_t 	m,n;
	Time_Arr[0] = time[1];
	m = Time_Arr[0] / 10;
	n = Time_Arr[0] % 10;
	Time_Arr[0] = m*16+n;
	
	Time_Arr[1] = time[2];
	m = Time_Arr[1] / 10;
	n = Time_Arr[1] % 10;
	Time_Arr[1] = m*16+n;
	
	Time_Arr[2] = time[3];
	m = Time_Arr[2] / 10;
	n = Time_Arr[2] % 10;
	Time_Arr[2] = m*16+n;
	if(time[7] <= 6)
	Time_Arr[3] = time[7];
	else Time_Arr[3] = 0;
	
	m = Time_Arr[3] / 10;
	n = Time_Arr[3] % 10;
	Time_Arr[3] = m*16+n;
	
	Time_Arr[4] = time[4];
	m = Time_Arr[4] / 10;
	n = Time_Arr[4] % 10;
	Time_Arr[4] = m*16+n;
	
	Time_Arr[5] = time[5];
	m = Time_Arr[5] / 10;
	n = Time_Arr[5] % 10;
	Time_Arr[5] = m*16+n;
	
	Time_Arr[6] = time[6];
	m = Time_Arr[6] / 10;
	n = Time_Arr[6] % 10;
	Time_Arr[6] = m*16+n;

	RTC_Set_Time(Time_Arr);
}


void Time_Update(void)
{	
	uint16_t	time_display[6];
	rdtime();
	read_dgus_vp(RTC,(u8*)&system.year,4);
	
	time_display[0] = system.year + 2000;
	time_display[1] = system.month;
	time_display[2] = system.date+ 100;;
	time_display[3] = system.hour + 100;
	time_display[4] = system.min	+ 100;
	time_display[5] = system.sec + 100;
	system.week = getWeekdayByYearday(system.year,system.month,system.date);//RTC_Get_Week(rtc_set_t[0],rtc_set_t[1],rtc_set_t[2]);
	time_display[6] = system.week;
	write_dgus_vp(DISP_RTC,(u8*)time_display,7);
//	if(system.hour	==	6	&&	system.min	==	0	&&	system.sec	==	1)
//	{
//		mcu_get_system_time();
//	}
	
}

