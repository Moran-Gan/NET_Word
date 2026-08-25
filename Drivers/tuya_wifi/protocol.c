/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  涂鸦综合协议开发组
 * @version v2.5.6
 * @date    2020.12.16
 * @brief                
 *                       *******非常重要，一定要看哦！！！********
 *          1. 用户在此文件中实现数据下发/上报功能
 *          2. DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
 *          3. 当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
 */

/****************************** 免责声明 ！！！ *******************************
由于MCU类型和编译环境多种多样，所以此代码仅供参考，用户请自行把控最终代码质量，
涂鸦不对MCU功能结果负责。
******************************************************************************/

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

#include "wifi.h"
#include	"Dwin_CONF.h"
#include	"user_include.h"
//#include "HeadFiles\user_include.h"
#ifdef WEATHER_ENABLE
/**
 * @var    weather_choose
 * @brief  天气数据参数选择数组
 * @note   用户可以自定义需要的参数，注释或者取消注释即可，注意更改
 */
const char *weather_choose[WEATHER_CHOOSE_CNT] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",
    "conditionNum",*/
};
#endif


/******************************************************************************
                              第一步:初始化
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_TEMP_SET, DP_TYPE_VALUE},
  {DPID_WORK_STATE, DP_TYPE_ENUM},
  {DPID_COOLING_SET, DP_TYPE_VALUE},
  {DPID_HEATING_WATER, DP_TYPE_ENUM},
  {DPID_COOLING_WATER, DP_TYPE_ENUM},
  {DPID_STATE_POWER, DP_TYPE_BOOL},
  {DPID_TEMP_IN, DP_TYPE_VALUE},
  {DPID_TEMP_OUT, DP_TYPE_VALUE},
  {DPID_BIT_FAULT, DP_TYPE_BOOL},
  {DPID_FAULT_TEMP, DP_TYPE_BITMAP},
  {DPID_SYS_FAULT_1, DP_TYPE_BITMAP},
  {DPID_SYS_FAULT_2, DP_TYPE_BITMAP},
  {DPID_SYS_FAULT_3, DP_TYPE_BITMAP},
  {DPID_SYS_FAULT_4, DP_TYPE_BITMAP},
  {DPID_DRIVER_FAULT_1, DP_TYPE_BITMAP},
  {DPID_DRIVER_FAULT_2, DP_TYPE_BITMAP},
  {DPID_DRIVER_FAULT_3, DP_TYPE_BITMAP},
  {DPID_MODEL_FAULT, DP_TYPE_BITMAP},
  {DPID_MODEL, DP_TYPE_ENUM},
};



/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 */
//void uart_transmit_output(unsigned char value)
//{
////    #error "请将MCU串口发送函数填入该函数,并删除该行"
///*
//    //Example:
//    extern void Uart_PutChar(unsigned char value);
//    Uart_PutChar(value);	                                //串口发送函数
//*/
//}

/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

void	wifi_all_data_update(void)
{
	uint8_t	mode;
	static int16_t	all_data_updata[25] = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
																				 0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
																				 0xffff,0xffff,0xffff,0xffff,0xffff};
	if(wifi_work_state == 4)
	{
	
		if(Para_pack[PARA_MODE]	==	HEATING)
		{
			if(Para_pack[PARA_MUTE]	==	ECO)
			{
				mode	=	3;
			}else
			{
				mode	=	1;
			}
			
		}
		else
		{
			if(Para_pack[PARA_MUTE]	==	ECO)
			{
				mode	=	2;
			}else
			{
				mode	=	0;
			}
		}
		
		if(all_data_updata[0]	!=	Para_pack[PARA_POWER_ON])
		{
			all_data_updata[0]	=	Para_pack[PARA_POWER_ON];
			mcu_dp_bool_update(DPID_SWITCH,Para_pack[PARA_POWER_ON]); //BOOL型数据上报;
		}
		else	if(all_data_updata[1]	!=	mode)
		{
			all_data_updata[1]	=	mode;
			mcu_dp_enum_update(DPID_MODE,mode); //枚举型数据上报;
		}
		else	if(all_data_updata[2]	!=	Para_pack[PARA_HEATING_TEMP])
		{
			all_data_updata[2]	=	Para_pack[PARA_HEATING_TEMP];
			 mcu_dp_value_update(DPID_TEMP_SET,Para_pack[PARA_HEATING_TEMP]); //VALUE型数据上报;
		}
		else	if(all_data_updata[3]	!=	Para_pack[PARA_COOLING_TEMP])
		{
			all_data_updata[3]	=	Para_pack[PARA_COOLING_TEMP];
			mcu_dp_value_update(DPID_COOLING_SET,Para_pack[PARA_COOLING_TEMP]); //VALUE型数据上报;
		}
		else	if(all_data_updata[4]	!=	Para_pack[PARA_F18_122])
		{
			all_data_updata[4]	=	Para_pack[PARA_F18_122];
			mcu_dp_enum_update(DPID_MODEL,Para_pack[PARA_F18_122]); //枚举型数据上报;
		}
		else	if(all_data_updata[5]	!=	State_pack[STATE_E_SERIES_00])
		{
			all_data_updata[5]	=	State_pack[STATE_E_SERIES_00];
			mcu_dp_fault_update(DPID_FAULT_TEMP,State_pack[STATE_E_SERIES_00]); //故障型数据上报;
		}
		else	if(all_data_updata[6]	!=	State_pack[STATE_E_SERIES_01])
		{
			all_data_updata[6]	=	State_pack[STATE_E_SERIES_01];
			mcu_dp_fault_update(DPID_SYS_FAULT_1,State_pack[STATE_E_SERIES_01]); //故障型数据上报;
		}
		else	if(all_data_updata[7]	!=	State_pack[STATE_E_SERIES_02])
		{
			all_data_updata[7]	=	State_pack[STATE_E_SERIES_02];
			mcu_dp_fault_update(DPID_SYS_FAULT_2,State_pack[STATE_E_SERIES_02]); //故障型数据上报;
		}
		else	if(all_data_updata[8]	!=	State_pack[STATE_E_SERIES_03])
		{
			all_data_updata[8]	=	State_pack[STATE_E_SERIES_03];
			mcu_dp_fault_update(DPID_SYS_FAULT_3,State_pack[STATE_E_SERIES_03]); //故障型数据上报;
		}
		else	if(all_data_updata[9]	!=	State_pack[STATE_E_SERIES_04])
		{
			all_data_updata[9]	=	State_pack[STATE_E_SERIES_04];
			mcu_dp_fault_update(DPID_SYS_FAULT_4,State_pack[STATE_E_SERIES_04]); //故障型数据上报;
		}
		else	if(all_data_updata[10]	!=	State_pack[STATE_E_SERIES_06])
		{
			all_data_updata[10]	=	State_pack[STATE_E_SERIES_06];
			mcu_dp_fault_update(DPID_DRIVER_FAULT_1,State_pack[STATE_E_SERIES_06]); //故障型数据上报;
		}
		else	if(all_data_updata[11]	!=	State_pack[STATE_E_SERIES_07])
		{
			all_data_updata[11]	=	State_pack[STATE_E_SERIES_07];
			mcu_dp_fault_update(DPID_DRIVER_FAULT_2,State_pack[STATE_E_SERIES_07]); //故障型数据上报;
		}
		else	if(all_data_updata[12]	!=	State_pack[STATE_E_SERIES_08])
		{
			all_data_updata[12]	=	State_pack[STATE_E_SERIES_08];
			mcu_dp_fault_update(DPID_DRIVER_FAULT_3,State_pack[STATE_E_SERIES_08]); //故障型数据上报;
		}
		else	if(all_data_updata[13]	!=	State_pack[STATE_E_SERIES_12])
		{
			all_data_updata[13]	=	State_pack[STATE_E_SERIES_12];
			 mcu_dp_fault_update(DPID_MODEL_FAULT,State_pack[STATE_E_SERIES_12]); //故障型数据上报;
		}
		else	if(all_data_updata[14]	!=	fault_ico)
		{
			all_data_updata[14]	=	fault_ico;
			 mcu_dp_bool_update(DPID_BIT_FAULT,fault_ico); //BOOL型数据上报;
		}
		else	if(all_data_updata[15]	!=	State_pack[STATE_TEMP_IN])
		{
			all_data_updata[15]	=	State_pack[STATE_TEMP_IN];
			 mcu_dp_value_update(DPID_TEMP_IN,State_pack[STATE_TEMP_IN]); //VALUE型数据上报;
		}
		else	if(all_data_updata[16]	!=	State_pack[STATE_TEMP_OUT])
		{
			all_data_updata[16]	=	State_pack[STATE_TEMP_OUT];
			 mcu_dp_value_update(DPID_TEMP_OUT,State_pack[STATE_TEMP_OUT]); //VALUE型数据上报;
		}
	}

	
}


//自动化生成数据上报函数

/**
 * @brief  系统所有dp点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void)
{
 //   #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"
    /*
    //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
    mcu_dp_bool_update(DPID_SWITCH,当前开关); //BOOL型数据上报;
    mcu_dp_enum_update(DPID_MODE,当前模式); //枚举型数据上报;
    mcu_dp_value_update(DPID_TEMP_SET,当前制热设定温度); //VALUE型数据上报;
    mcu_dp_enum_update(DPID_WORK_STATE,当前工作状态); //枚举型数据上报;
    mcu_dp_value_update(DPID_COOLING_SET,当前制冷设定温度); //VALUE型数据上报;
    mcu_dp_enum_update(DPID_HEATING_WATER,当前制热水温控制); //枚举型数据上报;
    mcu_dp_enum_update(DPID_COOLING_WATER,当前制冷水温控制); //枚举型数据上报;
    mcu_dp_bool_update(DPID_STATE_POWER,当前开关机状态); //BOOL型数据上报;
    mcu_dp_value_update(DPID_TEMP_IN,当前进水温度); //VALUE型数据上报;
    mcu_dp_value_update(DPID_TEMP_OUT,当前出水温度); //VALUE型数据上报;
    mcu_dp_bool_update(DPID_BIT_FAULT,当前故障标志位); //BOOL型数据上报;
    mcu_dp_fault_update(DPID_FAULT_TEMP,当前传感器故障); //故障型数据上报;
    mcu_dp_fault_update(DPID_SYS_FAULT_1,当前系统故障1); //故障型数据上报;
    mcu_dp_fault_update(DPID_SYS_FAULT_2,当前系统故障2); //故障型数据上报;
    mcu_dp_fault_update(DPID_SYS_FAULT_3,当前系统故障3); //故障型数据上报;
    mcu_dp_fault_update(DPID_SYS_FAULT_4,当前系统故障4); //故障型数据上报;
    mcu_dp_fault_update(DPID_DRIVER_FAULT_1,当前驱动故障1); //故障型数据上报;
    mcu_dp_fault_update(DPID_DRIVER_FAULT_2,当前驱动故障2); //故障型数据上报;
    mcu_dp_fault_update(DPID_DRIVER_FAULT_3,当前驱动故障3); //故障型数据上报;
    mcu_dp_fault_update(DPID_MODEL_FAULT,当前模块机故障); //故障型数据上报;
   

    */

		
//    
   
		
		 
		
		
		
		
		
		
		
		
		
//    mcu_dp_enum_update(DPID_WORK_STATE,当前工作状态); //枚举型数据上报;
//    
//    mcu_dp_enum_update(DPID_HEATING_WATER,当前制热水温控制); //枚举型数据上报;
//    mcu_dp_enum_update(DPID_COOLING_WATER,当前制冷水温控制); //枚举型数据上报;
//    mcu_dp_bool_update(DPID_STATE_POWER,当前开关机状态); //BOOL型数据上报;


		
		
}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/
/*****************************************************************************
函数名称 : dp_download_switch_handle
功能描述 : 针对DPID_SWITCH的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[])
{
    //示例:当前DP类型为BOOL
    unsigned char ret;
    //0:off/1:on
    unsigned char switch_1;
    
    switch_1 = value[0];//mcu_get_dp_download_bool(value);
		Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS,switch_1);
//    if(switch_1 == 0) {
//        //bool off
//    }else {
//        //bool on
//    }
  
    //There should be a report after processing the DP
    ret = mcu_dp_bool_update(DPID_SWITCH,switch_1);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_mode_handle
功能描述 : 针对DPID_MODE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[])
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char mode;
    
    mode = value[0];//mcu_get_dp_download_enum(value,length);
    switch(mode) {
        case 0:									//制冷
					EXIT_VACATION();
					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,0);
					Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,COOLING);
        break;
        
        case 1:									//制热
					EXIT_VACATION();
					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,0);
					Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,HEATING);
        break;
        
        case 2:								//制冷+eco
					EXIT_VACATION();
					Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,COOLING);
					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,ECO);
        break;
        
        case 3:								//制热+eco
					EXIT_VACATION();
					Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,HEATING);
					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,ECO);
        break;
        
        default:
    
        break;
    }
    
    //There should be a report after processing the DP
    ret = mcu_dp_enum_update(DPID_MODE, mode);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_temp_set_handle
功能描述 : 针对DPID_TEMP_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_temp_set_handle(const unsigned char value[])
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long temp_set;
		uint16_t	set_temp;
//		uint16_t	para_min,para_max;
    
    temp_set = mcu_get_dp_download_value(value);
		set_temp	=	temp_set;
		if(set_temp	>	Para_pack[PARA_HEATING_MAX])
		{
			set_temp	=	Para_pack[PARA_HEATING_MAX];
		}
		else	if(set_temp	<	Para_init_DATA[PARA_HEATING_TEMP][0])
		{
			set_temp	=	Para_init_DATA[PARA_HEATING_TEMP][0];
		}
		write_dgus_vp(TOUCH_EVENT_FLAG + (PARA_HEATING_TEMP	+ PARAMETER_92_START_ADDRESS)	,(u8*)&set_temp,1);
		Send_modebus_command_06(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS,set_temp);					//上传到云端
		
	
//		para_min = Para_init_DATA[PARA_HEATING_TEMP][0];
//		para_max = Para_pack[PARA_HEATING_MAX];
    /*
    //VALUE type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_TEMP_SET,set_temp);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_cooling_set_handle
功能描述 : 针对DPID_COOLING_SET的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_cooling_set_handle(const unsigned char value[])
{
    //示例:当前DP类型为VALUE
    unsigned char ret;
    unsigned long cooling_set;
	uint16_t	set_cooling;
    
    cooling_set = mcu_get_dp_download_value(value);
	set_cooling	=	cooling_set;
    /*
    //VALUE type data processing
    
    */
    if(set_cooling	>	Para_init_DATA[PARA_COOLING_TEMP][1])
		{
			set_cooling	=	Para_init_DATA[PARA_COOLING_TEMP][1];
		}
		else	if(set_cooling	<	Para_pack[PARA_COOLING_MIN])
		{
			set_cooling	=	Para_pack[PARA_COOLING_MIN];
		}
		write_dgus_vp(TOUCH_EVENT_FLAG + (PARA_COOLING_TEMP	+ PARAMETER_92_START_ADDRESS)	,(u8*)&set_cooling,1);
		Send_modebus_command_06(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS,set_cooling);					//上传到云端
    //There should be a report after processing the DP
    ret = mcu_dp_value_update(DPID_COOLING_SET,set_cooling);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_heating_water_handle
功能描述 : 针对DPID_HEATING_WATER的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_heating_water_handle(const unsigned char value[])
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char heating_water;
    
    heating_water = value[0];//mcu_get_dp_download_enum(value,length);
    switch(heating_water) {
        case 0:
        break;
        
        case 1:
        break;
        
        default:
    
        break;
    }
    
    //There should be a report after processing the DP
    ret = mcu_dp_enum_update(DPID_HEATING_WATER, heating_water);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_cooling_water_handle
功能描述 : 针对DPID_COOLING_WATER的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_cooling_water_handle(const unsigned char value[])
{
    //示例:当前DP类型为ENUM
    unsigned char ret;
    unsigned char cooling_water;
    
    cooling_water = value[0];//mcu_get_dp_download_enum(value,length);
    switch(cooling_water) {
        case 0:
        break;
        
        case 1:
        break;
        
        default:
    
        break;
    }
    
    //There should be a report after processing the DP
    ret = mcu_dp_enum_update(DPID_COOLING_WATER, cooling_water);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}




/******************************************************************************
                                WARNING!!!                     
此部分函数用户请勿修改!!
******************************************************************************/

/**
 * @brief  dp下发处理函数
 * @param[in] {dpid} dpid 序号
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {length} dp数据长度
 * @return dp处理结果
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   该函数用户不能修改
 */
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[])
{
    /*********************************
    当前函数处理可下发/可上报数据调用                    
    具体函数内需要实现下发数据处理
    完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
    ***********************************/
    unsigned char ret;
    switch(dpid) {
        case DPID_SWITCH:
            //开关处理函数
            ret = dp_download_switch_handle(value);
        break;
        case DPID_MODE:
            //模式处理函数
            ret = dp_download_mode_handle(value);
        break;
        case DPID_TEMP_SET:
            //制热设定温度处理函数
            ret = dp_download_temp_set_handle(value);
        break;
        case DPID_COOLING_SET:
            //制冷设定温度处理函数
            ret = dp_download_cooling_set_handle(value);
        break;
//        case DPID_HEATING_WATER:
//            //制热水温控制处理函数
//            ret = dp_download_heating_water_handle(value);
//        break;
//        case DPID_COOLING_WATER:
//            //制冷水温控制处理函数
//            ret = dp_download_cooling_water_handle(value);
//        break;

        
        default:
        break;
    }
    return ret;
}

/**
 * @brief  获取所有dp命令总和
 * @param[in] Null
 * @return 下发命令总和
 * @note   该函数用户不能修改
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  升级包大小选择
 * @param[in] {package_sz} 升级包大小
 * @ref           0x00: 256byte (默认)
 * @ref           0x01: 512byte
 * @ref           0x02: 1024byte
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void upgrade_package_choose(unsigned char package_sz)
{
    #error "请自行实现请自行实现升级包大小选择代码,完成后请删除该行"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(UPDATE_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  MCU进入固件升级模式
 * @param[in] {value} 固件缓冲区
 * @param[in] {position} 当前数据包在于固件位置
 * @param[in] {length} 当前固件包长度(固件包长度为0时,表示固件包发送完成)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    #error "请自行完成MCU固件升级代码,完成后请删除该行"
    if(length == 0) {
        //固件数据发送完成
      
    }else {
        //固件数据处理
      
    }
    
    return SUCCESS;
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  获取到的格林时间
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_get_greentime(unsigned char time[])
{
    #error "请自行完成相关代码,并删除该行"
    /*
    time[0] 为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    time[1] 为年份，0x00 表示 2000 年
    time[2] 为月份，从 1 开始到12 结束
    time[3] 为日期，从 1 开始到31 结束
    time[4] 为时钟，从 0 开始到23 结束
    time[5] 为分钟，从 0 开始到59 结束
    time[6] 为秒钟，从 0 开始到59 结束
    */
    if(time[0] == 1) {
        //正确接收到wifi模块返回的格林数据
        
    }else {
        //获取格林时间出错,有可能是当前wifi模块未联网
    }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU校对本地RTC时钟
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_write_rtctime(unsigned char time[])
{
 //   #error "请自行完成RTC时钟写入代码,并删除该行"
    /*
    Time[0] 为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    Time[1] 为年份，0x00 表示 2000 年
    Time[2] 为月份，从 1 开始到12 结束
    Time[3] 为日期，从 1 开始到31 结束
    Time[4] 为时钟，从 0 开始到23 结束
    Time[5] 为分钟，从 0 开始到59 结束
    Time[6] 为秒钟，从 0 开始到59 结束
    Time[7] 为星期，从 1 开始到 7 结束，1代表星期一
   */
    if(time[0] == 1) {
        //正确接收到wifi模块返回的本地时钟数据
			Write_time_set_confirm(time);
     
    }else {
        //获取本地时钟数据出错,有可能是当前wifi模块未联网
    }
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi功能测试反馈
 * @param[in] {result} wifi功能测试结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @param[in] {rssi} 测试成功表示wifi信号强度/测试失败表示错误类型
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void wifi_test_result(unsigned char result,unsigned char rssi)
{
//    #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
	uint16_t	value	=	0;
    if(result == 0) {
			value	=	0;
        //测试失败
        if(rssi == 0x00) {
            //未扫描到名称为tuya_mdev_test路由器,请检查
        }else if(rssi == 0x01) {
            //模块未授权
        }
    }else {
			value = 1;
        //测试成功
        //rssi为信号强度(0-100, 0信号最差，100信号最强)
    }
		write_dgus_vp(DISP_test_wifi1,(u8*)&value,1);
		value	=	rssi;
		write_dgus_vp(DISP_test_wifi2,(u8*)&value,1);
}
#endif

#ifdef WEATHER_ENABLE
/**
* @brief  mcu打开天气服务
 * @param  Null
 * @return Null
 */
void mcu_open_weather(void)
{
    int i = 0;
    char buffer[13] = {0};
    unsigned char weather_len = 0;
    unsigned short send_len = 0;
    
    weather_len = sizeof(weather_choose) / sizeof(weather_choose[0]);
      
    for(i=0;i<weather_len;i++) {
        buffer[0] = sprintf(buffer+1,"w.%s",weather_choose[i]);
        send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    }
    
    #error "请根据提示，自行完善打开天气服务代码，完成后请删除该行"
    /*
    //当获取的参数有和时间有关的参数时(如:日出日落)，需要搭配t.unix或者t.local使用，需要获取的参数数据是按照格林时间还是本地时间
    buffer[0] = sprintf(buffer+1,"t.unix"); //格林时间   或使用  buffer[0] = sprintf(buffer+1,"t.local"); //本地时间
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    */
    
    buffer[0] = sprintf(buffer+1,"w.date.%d",WEATHER_FORECAST_DAYS_NUM);
    send_len = set_wifi_uart_buffer(send_len, (unsigned char *)buffer, buffer[0]+1);
    
    wifi_uart_write_frame(WEATHER_OPEN_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  打开天气功能返回用户自处理函数
 * @param[in] {res} 打开天气功能返回结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @param[in] {err} 错误码
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void weather_open_return_handle(unsigned char res, unsigned char err)
{
    #error "请自行完成打开天气功能返回数据处理代码,完成后请删除该行"
    unsigned char err_num = 0;
    
    if(res == 1) {
        //打开天气返回成功
    }else if(res == 0) {
        //打开天气返回失败
        //获取错误码
        err_num = err; 
    }
}

/**
 * @brief  天气数据用户自处理函数
 * @param[in] {name} 参数名
 * @param[in] {type} 参数类型
 * @ref       0: int 型
 * @ref       1: string 型
 * @param[in] {data} 参数值的地址
 * @param[in] {day} 哪一天的天气  0:表示当天 取值范围: 0~6
 * @ref       0: 今天
 * @ref       1: 明天
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void weather_data_user_handle(char *name, unsigned char type, const unsigned char *data, char day)
{
    #error "这里仅给出示例，请自行完善天气数据处理代码,完成后请删除该行"
    int value_int;
    char value_string[50];//由于有的参数内容较多，这里默认为50。您可以根据定义的参数，可以适当减少该值
    
    memset(value_string, '\0', 50);
    
    //首先获取数据类型
    if(type == 0) { //参数是INT型
        value_int = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    }else if(type == 1) {
        strcpy(value_string, data);
    }
    
    //注意要根据所选参数类型来获得参数值！！！
    if(strcmp(name, "temp") == 0) {
        printf("day:%d temp value is:%d\r\n", day, value_int);          //int 型
    }else if(strcmp(name, "humidity") == 0) {
        printf("day:%d humidity value is:%d\r\n", day, value_int);      //int 型
    }else if(strcmp(name, "pm25") == 0) {
        printf("day:%d pm25 value is:%d\r\n", day, value_int);          //int 型
    }else if(strcmp(name, "condition") == 0) {
        printf("day:%d condition value is:%s\r\n", day, value_string);  //string 型
    }
}
#endif

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  状态同步上报结果
 * @param[in] {result} 结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_upload_syn_result(unsigned char result)
{
    #error "请自行完成状态同步上报结果代码,并删除该行"
      
    if(result == 0) {
        //同步上报出错
    }else {
        //同步上报成功
    }
}
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  获取 WIFI 状态结果
 * @param[in] {result} 指示 WIFI 工作状态
 * @ref       0x00: wifi状态 1 smartconfig 配置状态
 * @ref       0x01: wifi状态 2 AP 配置状态
 * @ref       0x02: wifi状态 3 WIFI 已配置但未连上路由器
 * @ref       0x03: wifi状态 4 WIFI 已配置且连上路由器
 * @ref       0x04: wifi状态 5 已连上路由器且连接到云端
 * @ref       0x05: wifi状态 6 WIFI 设备处于低功耗模式
 * @ref       0x06: wifi状态 7 WIFI 设备处于smartconfig&AP配置状态
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_wifi_status(unsigned char result)
{
  #error "请自行完成获取 WIFI 状态结果代码,并删除该行"
 
    switch(result) {
        case 0:
            //wifi工作状态1
        break;
    
        case 1:
            //wifi工作状态2
        break;
        
        case 2:
            //wifi工作状态3
        break;
        
        case 3:
            //wifi工作状态4
        break;
        
        case 4:
            //wifi工作状态5
        break;
        
        case 5:
            //wifi工作状态6
        break;
      
        case 6:
            //wifi工作状态7
        break;
        
        default:break;
    }
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  流服务发送结果
 * @param[in] {result} 结果
 * @ref       0x00: 成功
 * @ref       0x01: 流服务功能未开启
 * @ref       0x02: 流服务器未连接成功
 * @ref       0x03: 数据推送超时
 * @ref       0x04: 传输的数据长度错误
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void stream_trans_send_result(unsigned char result)
{
    #error "这里仅给出示例，请自行完善流服务发送结果处理代码,完成后请删除该行"
    switch(result) {
        case 0x00:
            //成功
        break;
        
        case 0x01:
            //流服务功能未开启
        break;
        
        case 0x02:
            //流服务器未连接成功
        break;
        
        case 0x03:
            //数据推送超时
        break;
        
        case 0x04:
            //传输的数据长度错误
        break;
        
        default:break;
    }
}

/**
 * @brief  多地图流服务发送结果
 * @param[in] {result} 结果
 * @ref       0x00: 成功
 * @ref       0x01: 失败
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void maps_stream_trans_send_result(unsigned char result)
{
    #error "这里仅给出示例，请自行完善多地图流服务发送结果处理代码,完成后请删除该行"
    switch(result) {
        case 0x00:
            //成功
        break;
        
        case 0x01:
            //失败
        break;
        
        default:break;
    }
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  路由信息接收结果通知
 * @param[in] {result} 模块是否成功接收到正确的路由信息
 * @ref       0x00: 失败
 * @ref       0x01: 成功
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void wifi_connect_test_result(unsigned char result)
{
    #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
    if(result == 0) {
        //路由信息接收失败，请检查发出的路由信息包是否是完整的JSON数据包
    }else {
        //路由信息接收成功，产测结果请注意WIFI_STATE_CMD指令的wifi工作状态
    }
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  获取模块mac结果
 * @param[in] {mac} 模块 MAC 数据
 * @ref       mac[0]: 为是否获取mac成功标志，0x00 表示成功，0x01 表示失败
 * @ref       mac[1]~mac[6]: 当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_get_mac(unsigned char mac[])
{
    #error "请自行完成mac获取代码,并删除该行"
    /*
    mac[0]为是否获取mac成功标志，0x00 表示成功，为0x01表示失败
    mac[1]~mac[6]:当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
   */
   
    if(mac[0] == 1) {
        //获取mac出错
    }else {
        //正确接收到wifi模块返回的mac地址
    }
}
#endif

#ifdef GET_IR_STATUS_ENABLE
/**
 * @brief  获取红外状态结果
 * @param[in] {result} 指示红外状态
 * @ref       0x00: 红外状态 1 正在发送红外码
 * @ref       0x01: 红外状态 2 发送红外码结束
 * @ref       0x02: 红外状态 3 红外学习开始
 * @ref       0x03: 红外状态 4 红外学习结束
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_ir_status(unsigned char result)
{
    #error "请自行完成红外状态代码,并删除该行"
    switch(result) {
        case 0:
            //红外状态 1
        break;
      
        case 1:
            //红外状态 2
        break;
          
        case 2:
            //红外状态 3
        break;
          
        case 3:
            //红外状态 4
        break;
          
        default:break;
    }
    
    wifi_uart_write_frame(GET_IR_STATUS_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  红外进入收发产测结果通知
 * @param[in] {result} 模块是否成功接收到正确的信息
 * @ref       0x00: 失败
 * @ref       0x01: 成功
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void ir_tx_rx_test_result(unsigned char result)
{
    #error "请自行实现红外进入收发产测功能测试成功/失败代码,完成后请删除该行"
    if(result == 0) {
        //红外进入收发产测成功
    }else {
        //红外进入收发产测失败，请检查发出的数据包
    }
}
#endif

#ifdef FILE_DOWNLOAD_ENABLE
/**
 * @brief  文件下载包大小选择
 * @param[in] {package_sz} 文件下载包大小
 * @ref       0x00: 256 byte (默认)
 * @ref       0x01: 512 byte
 * @ref       0x02: 1024 byte
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void file_download_package_choose(unsigned char package_sz)
{
    #error "请自行实现请自行实现文件下载包大小选择代码,完成后请删除该行"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
    wifi_uart_write_frame(FILE_DOWNLOAD_START_CMD, MCU_TX_VER, send_len);
}

/**
 * @brief  文件包下载模式
 * @param[in] {value} 数据缓冲区
 * @param[in] {position} 当前数据包在于文件位置
 * @param[in] {length} 当前文件包长度(长度为0时,表示文件包发送完成)
 * @return 数据处理结果
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   MCU需要自行实现该功能
 */
unsigned char file_download_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    #error "请自行完成文件包下载代码,完成后请删除该行"
    if(length == 0) {
        //文件包数据发送完成
        
    }else {
        //文件包数据处理
      
    }
    
    return SUCCESS;
}
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
/**
 * @brief  打开模块时间服务通知结果
 * @param[in] {value} 数据缓冲区
 * @param[in] {length} 数据长度
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void open_module_time_serve_result(const unsigned char value[], unsigned short length)
{
    #error "请自行实现模块时间服务通知结果代码,完成后请删除该行"
    unsigned char sub_cmd = value[0];
    
    switch(sub_cmd) {
        case 0x01: { //子命令  打开模块时间服务通知
            if(0x02 != length) {
                //数据长度错误
                return;
            }
            
            if(value[1] == 0) {
                //服务开启成功
            }else {
                //服务开启失败
            }
        }
        break;
        
        case 0x02: {  //子命令  模块时间服务通知
            if(0x09 != length) {
                //数据长度错误
                return;
            }
            
            unsigned char time_type = value[1]; //0x00:格林时间  0x01:本地时间
            unsigned char time_data[7];
            
            memcpy(time_data, value + 2, length - 2);
            /*
            Data[0]为年份, 0x00表示2000年
            Data[1]为月份，从1开始到12结束
            Data[2]为日期，从1开始到31结束
            Data[3]为时钟，从0开始到23结束
            Data[4]为分钟，从0开始到59结束
            Data[5]为秒钟，从0开始到15结束
            Data[6]为星期，从1开始到7结束，1代表星期一
            */
            
            //在此处添加时间数据处理代码，time_type为时间类型
            
            unsigned short send_len = 0;
            send_len = set_wifi_uart_byte(send_len,sub_cmd);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;
        
        case 0x03: {  //子命令  主动请求天气服务数据
            if(0x02 != length) {
                //数据长度错误
                return;
            }
            
            if(value[1] == 0) {
                //成功
            }else {
                //失败
            }
        }
        break;
        
        case 0x04: {  //子命令  打开模块重置状态通知
            if(0x02 != length) {
                //数据长度错误
                return;
            }
            
            if(value[1] == 0) {
                //成功
            }else {
                //失败
            }
        }
        break;
        
        case 0x05: {  //子命令  模块重置状态通知
            if(0x02 != length) {
                //数据长度错误
                return;
            }
            
            switch(value[1]) {
                case 0x00:
                    //模块本地重置
                    
                break;
                case 0x01:
                    //APP远程重置
                    
                break;
                case 0x02:
                    //APP恢复出厂重置
                    
                break;
                default:break;
            }
            
            unsigned short send_len = 0;
            send_len = set_wifi_uart_byte(send_len, sub_cmd);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;
        
        default:break;
    }
}
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  蓝牙功能性测试结果
 * @param[in] {value} 数据缓冲区
 * @param[in] {length} 数据长度
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void BLE_test_result(const unsigned char value[], unsigned short length)
{
    #error "请自行实现蓝牙功能性测试结果代码,完成后请删除该行"
    unsigned char sub_cmd = value[0];
    
    if(0x03 != length) {
        //数据长度错误
        return;
    }
    
    if(0x01 != sub_cmd) {
        //子命令错误
        return;
    }
    
    unsigned char result = value[1];
    unsigned char rssi = value[2];
        
    if(result == 0) {
        //测试失败
        if(rssi == 0x00) {
            //未扫描到名称为 ty_mdev蓝牙信标,请检查
        }else if(rssi == 0x01) {
            //模块未授权
        }
    }else if(result == 0x01) {
        //测试成功
        //rssi为信号强度(0-100, 0信号最差，100信号最强)
    }
}
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  获取语音状态码结果
 * @param[in] {result} 语音状态码
 * @ref       0x00: 空闲
 * @ref       0x01: mic静音状态
 * @ref       0x02: 唤醒
 * @ref       0x03: 正在录音
 * @ref       0x04: 正在识别
 * @ref       0x05: 识别成功
 * @ref       0x06: 识别失败
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_voice_state_result(unsigned char result)
{
    #error "请自行实现获取语音状态码结果处理代码,完成后请删除该行"
    switch(result) {
        case 0:
            //空闲
        break;
    
        case 1:
            //mic静音状态
        break;
        
        case 2:
            //唤醒
        break;
        
        case 3:
            //正在录音
        break;
        
        case 4:
            //正在识别
        break;
    
        case 5:
            //识别成功
        break;
        
        case 6:
            //识别失败
        break;
        
      default:break;
    }
}

/**
 * @brief  MIC静音设置结果
 * @param[in] {result} 语音状态码
 * @ref       0x00: mic 开启
 * @ref       0x01: mic 静音
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void set_voice_MIC_silence_result(unsigned char result)
{
    #error "请自行实现MIC静音设置处理代码,完成后请删除该行"
    if(result == 0) {
        //mic 开启
    }else {
        //mic 静音
    }
}

/**
 * @brief  speaker音量设置结果
 * @param[in] {result} 音量值
 * @ref       0~10: 音量范围
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void set_speaker_voice_result(unsigned char result)
{
    #error "请自行实现speaker音量设置结果处理代码,完成后请删除该行"
    
}

/**
 * @brief  音频产测结果
 * @param[in] {result} 音频产测状态
 * @ref       0x00: 关闭音频产测
 * @ref       0x01: mic1音频环路测试
 * @ref       0x02: mic2音频环路测试
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void voice_test_result(unsigned char result)
{
    #error "请自行实现音频产测结果处理代码,完成后请删除该行"
    if(result == 0x00) {
        //关闭音频产测
    }else if(result == 0x01) {
        //mic1音频环路测试
    }else if(result == 0x02) {
        //mic2音频环路测试
    }
}

/**
 * @brief  唤醒产测结果
 * @param[in] {result} 唤醒返回值
 * @ref       0x00: 唤醒成功
 * @ref       0x01: 唤醒失败(10s超时失败)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void voice_awaken_test_result(unsigned char result)
{
    #error "请自行实现唤醒产测结果处理代码,完成后请删除该行"
    if(result == 0x00) {
        //唤醒成功
    }else if(result == 0x01) {
        //唤醒失败
    }
}

/**
 * @brief  语音模组扩展功能
 * @param[in] {value} 数据缓冲区
 * @param[in] {length} 数据长度
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void voice_module_extend_fun(const unsigned char value[], unsigned short length)
{
    unsigned char sub_cmd = value[0];
    unsigned char play;
    unsigned char bt_play;
    unsigned short send_len = 0;
  
    switch(sub_cmd) {
        case 0x00: { //子命令  MCU功能设置
            if(0x02 != length) {
                //数据长度错误
                return;
            }
            
            if(value[1] == 0) {
                //成功
            }else {
                //失败
            }
        }
        break;
        
        case 0x01: {  //子命令  状态通知
            if(0x02 > length) {
                //数据长度错误
                return;
            }
            
            unsigned char play = 0xff;
            unsigned char bt_play = 0xff;
            
            const char *str_buff = (const char *)&value[1];
            const char *str_result = NULL;
            
            str_result = strstr(str_buff,"play") + strlen("play") + 2;
            if(NULL == str_result) {
                //数据错误
                goto ERR_EXTI;
            }
            
            if(0 == memcmp(str_result, "true", strlen("true"))) {
                play = 1;
            }else if(0 == memcmp(str_result, "false", strlen("false"))) {
                play = 0;
            }else {
                //数据错误
                goto ERR_EXTI;
            }
            
            str_result = strstr(str_buff,"bt_play") + strlen("bt_play") + 2;
            if(NULL == str_result) {
                //数据错误
                goto ERR_EXTI;
            }
            
            if(0 == memcmp(str_result, "true", strlen("true"))) {
                bt_play = 1;
            }else if(0 == memcmp(str_result, "false", strlen("false"))) {
                bt_play = 0;
            }else {
                //数据错误
                goto ERR_EXTI;
            }
            
            #error "请自行实现语音模组状态通知处理代码,完成后请删除该行"
            //MCU设置暂仅支持”播放/暂停” ”蓝牙开关”
            //play    播放/暂停功能  1(播放) / 0(暂停)
            //bt_play 蓝牙开关功能   1(开)   / 0(关)
            
            
            
            send_len = 0;
            send_len = set_wifi_uart_byte(send_len, sub_cmd);
            send_len = set_wifi_uart_byte(send_len, 0x00);
            wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
        }
        break;

        default:break;
    }
    
    return;

ERR_EXTI:
    send_len = 0;
    send_len = set_wifi_uart_byte(send_len, sub_cmd);
    send_len = set_wifi_uart_byte(send_len, 0x01);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD, MCU_TX_VER, send_len);
    return;
}
#endif




