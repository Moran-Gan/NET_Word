
#include	"Dwin_CONF.h"
//#include	"Global.h"
#include	"math.h"


//typedef	union
//{
//	uint32_t	L;
//	uint16_t	I;
//}IEEE_F;

//IEEE_F	g_exL;

uint16_t Get_ADC_Value(uint8_t	Length)
{
	uint16_t	temp;
	read_dgus_vp((AD_VALUE+Length),(u8*)&temp,1);//获取通道AD值
	return	temp;
}

	//T1 == 电阻值相对于的开尔文温度值
	//Rt == 输入的电阻值
	//R == 25°对应的电阻值
	//B == 3470(根据温度探头的资料)
	//T2 == 25°对应的开尔文值 == 273.15 + 25
	//T3 == 0°对应的开尔文值 == 273.15
	//T1=1/（ln（Rt/R）/B+1/T2）
	//T = T1-T3

void	Get_Temp_Value()
{
//	int16_t	read_value;
//	read_dgus_vp(TEMP_VALUE,(u8*)&read_value,1);//获取通道AD值
//	Master_Modbus_Fountion_06_value(0x1392,(read_value-30));
}

//void	Get_Temp_Value(void)
//{
////	float	Rt = 0,T1;
////	int16_t	U16_Rt;
////	int16_t	T3,Temp;
////	int16_t ad_value_array[2]= {0,0,};
//	int16_t cur_ad_average[2] = {0,0};
//	
////	static	uint16_t	ADC_Count = 0;
//	static 	uint8_t		Cunt = 0;
////	uint16_t	AD;
//	
//	if(Cunt < 10)
//	{
//		//ADC_Count	+=	Get_ADC_Value(6);
//		Cunt++;
//	}else
//	{
//		Cunt	=	0;
//		//cur_ad_average[0] = ADC_Count/10;
//		read_dgus_vp(0x3e,(u8*)&cur_ad_average[0],1);//获取通道AD值
//		write_dgus_vp(0X70B2,(u8*)&cur_ad_average[0],1);	
//	}
//	
//	
//	
//	
//	
////		Cunt	=	0;
////		AD	=	ADC_Count/10;
////		cur_ad_average[0] = ADC_Count/10;
////		ADC_Count = 0;
////		if(cur_ad_average[0] > 10 && cur_ad_average[0] < 4000)
////		{
////			ad_value_array[0] = (cur_ad_average[0]/0.4095)*3.3;											//计算电压(mV)值放大到1000倍
////			Rt = (ad_value_array[0]*10000)/((33000-ad_value_array[0]));							//转电阻值放大100倍
////			T1 = (1/(log(Rt/10000)/3950+1/298.15))*100;

////			Protect.temp.ROOM	=	(T1 - 273.15)/10;
////			
////			write_dgus_vp(0X70B2,(u8*)&Rt,2);											
////			write_dgus_vp(0X70B4,(u8*)&T1	,2);
//////			write_dgus_vp(0X70B6,(u8*)&U16_Rt	,1);
////			write_dgus_vp(0X70B7,(u8*)&Protect.temp.ROOM	,1);
////		}else
////		{
////			
////		}
////	}
//}


//void	Get_Temp_Value(void)
//{
//	float	Rt = 0,T1;//,T;													//热敏电阻相对于的电阻值
//	float	T3,Temp;
//	float ad_value_array[2]= {0,0,};
//	float cur_ad_average[2] = {0,0};

//	static	uint16_t	ADC_Count = 0;
//	static uint8_t		Cunt = 0;
//	uint16_t	AD;
//	if(Cunt < 10)
//	{
//		ADC_Count	+=	Get_ADC_Value(6);
//		Cunt++;
//	}else
//	{
//		AD	=	ADC_Count/10;
//		cur_ad_average[0] = ADC_Count/10;
//		ADC_Count = 0;
//		Cunt = 0;
//		if(cur_ad_average[0] > 10 && cur_ad_average[0] < 4000)
//		{
//			
//			ad_value_array[0] = (cur_ad_average[0]/4095)*3.3;//*5000;								//计算电压(mV)值放大到1000倍		
//			Rt = (ad_value_array[0]*10000)/(3.3-ad_value_array[0]);									//转电阻值放大100倍
//			T1 = 1/(log(Rt/10000)/3950+1/298.15);
//			Protect.temp.ROOM	=	(T1 - 273.15)*10;
////			if(Protect.State.MODE == MODE_COOL || Protect.State.MODE == MODE_FEN)
////			{
////				T3 = A07_Para;																						//制冷/风扇温度补偿
////			}else
////			{
////				T3 = A08_Para;																						//制热/辅助电加热温度补偿
////			}
////			Temp = (T1 - 273.15)*10;
////			Protect.TEMP.ROOM = Temp + T3;									//计算所得温度放大10倍

////			if(Temp	!=	Protect.temp.ROOM)
////		{
//			Temp	=	Protect.temp.ROOM;
//			write_dgus_vp(0X70B2,(u8*)&ad_value_array[0],2);											//弹窗提示
//			write_dgus_vp(0X70B4,(u8*)&AD	,1);																			//弹窗提示
//		//}
//			
//		}else
//		{
//			Protect.temp.ROOM = 32767;
//		}
//		
//		
//		
//	}
//	
//}

/*
int16_t	Rt = 0,T1;//,T;													//热敏电阻相对于的电阻值
//	float	T3,Temp;
//	float ad_value_array[2]= {0,0,};
	uint16_t ad_value_array[2]= {0,0,};
//	float cur_ad_average[2] = {0,0};
	uint16_t cur_ad_average[2] = {0,0};
	static	uint16_t	Temp	=	0xffff;
	static	uint16_t	ADC_Count = 0;
	static uint8_t		Cunt = 0;
	uint16_t	AD;
	if(Cunt < 10)
	{
		ADC_Count	+=	Get_ADC_Value(6);
		Cunt++;
	}else
	{
		AD	=	ADC_Count;
		cur_ad_average[0] = ADC_Count/10;
		ADC_Count = 0;
		Cunt = 0;
		if(cur_ad_average[0] > 10 && cur_ad_average[0] < 4000)
		{
			
			ad_value_array[0] = (cur_ad_average[0]/0.4095)*3.3;//*5000;								//计算电压(mV)值放大到1000倍
			Rt = (ad_value_array[0]*10000)/((3.3*10000)-ad_value_array[0]));										//转电阻值放大100倍
			T1 = 1/(log(Rt/10000)/3950+1/298.15);
			Protect.temp.ROOM	=	(T1 - 273.15)*10;
*/