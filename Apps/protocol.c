/**********************************Copyright (c)**********************************
**                       版权所有 (C), 
**
**                             http://
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  电控部
 * @version v1.0.0
 * @date    2023.08.14
 * @brief   串口数据处理，只需修改配置该文件即可
 */
#include	"Dwin_CONF.h"
#include	"user_include.h"
#include	"Apps\wifi.h"

/*
*	串口接收函数
*/
uint16_t	Connect_addr = 0;
int16_t	Connect_data;

/*
*函数：void	Connect_Master_receive_handle_process(uint8_t *tx_data_buf,uint8_t	tx_len)
*功能：联网MODBUS解析，该函数无需修改
*/
void	Connect_Master_receive_handle_process(uint8_t *tx_data_buf,uint8_t	tx_len)
{
	uint16_t	CRC_check_Value,CRC_check_Value1;
	if(tx_data_buf[0]	==	0x01)
	{
		CRC_check_Value	=	tx_data_buf[tx_len - 1] <<8 | tx_data_buf[tx_len - 2] << 0;			//读取CRC
		CRC_check_Value1	=	crc16(tx_data_buf,(tx_len-2));	
		if(CRC_check_Value	==	CRC_check_Value1)
		{
			switch(tx_data_buf[1])
			{
				case	MASTER_03_COMMAND:
					connect_Analyze_function_03_data(tx_data_buf);															//写单个参数														//读参数
					break;

				case	MASTER_06_COMMAND:
					connect_Analyze_function_06_data(tx_data_buf);															//写单个参数
					break;
				
				case	MASTER_10_COMMAND:
																				//写全部参数
					break;
				
				default:break;
			}
		}
	}
}



/*
*函数：void	connect_Analyze_function_03_data(uint8_t *tx_data_buf)
*功能：03指令，读取参数根据协议自行修改
*/
void	connect_Analyze_function_03_data(uint8_t *tx_data_buf)
{
	uint16_t	connect_addr = tx_data_buf[2]	<<8|tx_data_buf[3];
	uint16_t	connect_len = tx_data_buf[4]	<<8|tx_data_buf[5];
		if(connect_addr	 == 0 && connect_len	==	0)
		{																																					//上传全部参数
			Flag_Connect_03_command	=	0;
		}else	
		{
//			if(connect_addr	>=	PARAMETER_1001_START_ADDRESS	&&	connect_addr	<	PARAMETER_1101_START_ADDRESS)
//			{																																				//后台读取第一帧数据
//				Flag_Connect_function_03_command	=	1;
//			}
//			else	if(connect_addr	>=	PARAMETER_1101_START_ADDRESS	&&	connect_addr	<=	PARAMETER_1201_START_ADDRESS)
//			{																																				//后台读取第二帧数据
//				Flag_Connect_function_03_command	=	2;
//			}
//			else	if(connect_addr	>=	PARAMETER_1201_START_ADDRESS	&&	connect_addr	<=	PARAMETER_1301_START_ADDRESS)
//			{																																				//后台读取第三帧数据
//				Flag_Connect_function_03_command	=	3;
//			}
//			else	if(connect_addr	>=	PARAMETER_1301_START_ADDRESS	&&	connect_addr	<=	PARAMETER_1401_START_ADDRESS)
//			{																																				//后台读取第四帧数据
//				Flag_Connect_function_03_command	=	4;
//			}
//			else	if(connect_addr	>=	PARAMETER_1401_START_ADDRESS	&&	connect_addr	<=	PARAMETER_1501_START_ADDRESS)
//			{																																				//后台读取第五帧数据
//				Flag_Connect_function_03_command	=	5;
//			}
//			else	if(connect_addr	>=	PARAMETER_1501_START_ADDRESS	&&	connect_addr	<=	PARAMETER_1601_START_ADDRESS)
//			{																																				//后台读取第六帧数据
//				Flag_Connect_function_03_command	=	6;
//			}
//			else	if(connect_addr	>=	STATE_2001_START_ADDRESS)
//			{																																				//后台读取第七帧数据
//				Flag_Connect_function_03_command	=	7;
//			}
		}
}

/*
*函数：void	connect_Analyze_function_03_data(uint8_t *tx_data_buf)
*功能：06指令，云端写操作根据协议自行修改
*/
void	connect_Analyze_function_06_data(uint8_t *tx_data_buf)
{
	uint16_t	modbus_addr;
//	int16_t		modbus_data;
//	int16_t		para_max,para_min;
	
	Connect_addr	=	tx_data_buf[2]	<<8|tx_data_buf[3];
	Connect_data	=	tx_data_buf[4]	<<8|tx_data_buf[5];
	modbus_addr	=	Connect_addr	-	PARAMETER_1000_START_ADDRESS;
	
	if(Connect_Para_Data[modbus_addr][1]	==	PARA)
	{																											
		if(Connect_Para_Data[modbus_addr][0]	==	93)
		{
			if(Connect_data	==	4)
			{															//节能制热
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,ECO);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,HEATING);	
			}
			else	if(Connect_data	==	5)
			{															//强劲制热
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,0);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,HEATING);
			}
			else	if(Connect_data	==	6)
			{															//节能制冷
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,ECO);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,COOLING);
			}
			else	if(Connect_data	==	7)
			{															//强劲制冷
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,0);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS,COOLING);
			}
		}
		else
		{
			Send_modebus_command_06(Connect_Para_Data[modbus_addr][0],Connect_data);
			write_dgus_vp(TOUCH_EVENT_FLAG	+	Connect_Para_Data[modbus_addr][0],(u8*)&Connect_data,1);	//显示到屏幕上
		}
	}
	else
	{
		if(Connect_addr	==	1000)
		{																							//监控模式
			monitor_mode	=	Connect_data;
		}
		else	if(Connect_addr	==	1001)
		{																							//监控间隔时长
			monitor_timer = Connect_data;
		}
		else	if(Connect_addr	==	1002)
		{																							//监控时长
			monitor_time	=	Connect_data;
		}
		else	if(Connect_addr	==	1003)
		{																							//状态参数
//			Connect.at_cmd	=	SEND_DATA;
			Flag_Connect_03_command	=	1;								//WIFI空闲时上传状态参数			
		}
		else	if(Connect_addr	==	1004)
		{																							//所有参数
			Flag_Connect_03_command	=	0;								//WiFi空闲时上传全部参数
		}
		else	if(Connect_addr	==	1006)
		{																							//常规上传间隔时间
			if(Connect_data	>=	5	&&	Connect_data	<=	1000)
			{																																				//防止超范围
				write_dgus_vp(EEPROM_Connect_UP_TIME,(u8*)&Connect_data,1);
				//T5L_Flash(WRITE,(EEPROM_Connect_UP_TIME-1),(EEPROM_Connect_UP_TIME - 1) - MAGIC_NUMBER,2);
				EEPROM_Write_page();
			}
			
		}
		else if (Connect_addr == 2050)		// 主板解锁功能
		{
			Enable_Host_Unlock = Connect_data;
			State2_pack[STATE_ENABLE_UNLOCK] = Connect_data;
			Send_modebus_command_06(STATE_ENABLE_UNLOCK + PARAMETER_250_START_ADDRESS, Connect_data);
		}
		else if (Connect_addr == 2051)		// 主板解锁标志
		{
			State2_pack[STATE_FLAG_UNLOCK] = Connect_data;
			Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, Connect_data);
		}
	}
	
	
//	
//	if(Para_init_DATA[modbus_addr][2] == TEMP_10)				//温度单位值强行转为摄氏度进行判断
//			modbus_data =  EXCHANGE_F_C(Connect_data,UNIT_TEMP);								
//		else
//			modbus_data = Connect_data;
//		/*
//		*制热、制冷、热水、地暖目标温度设定范围可变动需要单独判断
//		*/
//	if(modbus_addr	==	PARA_M03_1353 || modbus_addr	==	PARA_M49_1399)
//		{																																										//制热目标温度或者B区
//			para_min = Para_pack[PARA_M01_1351];
//			para_max = Para_pack[PARA_M02_1352];
//		}
//		else	if(modbus_addr	==	PARA_M06_1356)
//		{																																										//制冷目标温度
//			para_min = Para_pack[PARA_M04_1354];
//			para_max = Para_pack[PARA_M05_1355];
//		}
//		else	if(modbus_addr	==	PARA_M09_1359)
//		{																																										//热水目标温度
//			para_min = Para_pack[PARA_M07_1357];
//			para_max = Para_pack[PARA_M08_1358];
//		}
//		else
//		{																																										//其他参数设定范围
//			para_min = Para_init_DATA[modbus_addr][0];
//			para_max = Para_init_DATA[modbus_addr][1];
//		}

//	if(modbus_data	>	para_max || modbus_data	<	para_min)
//	{
//		if(modbus_data	>	para_max)
//		{																																											//云端写入值大于最大值等于最大值，小于最小值等于最小值									
//			modbus_data	=	para_max;
//		}else	//if(modbus_data	<	para_min)
//		{
//			modbus_data	=	para_min;
//		}
//		
//		if(Para_init_DATA[modbus_addr][2] == TEMP_10)																					//超复位根据实际值应答														
//			Connect_data =  EXCHANGE_C_F(modbus_data,UNIT_TEMP);								
//		else
//			Connect_data = modbus_data;
//	}

//	Send_modebus_command_06(modbus_addr + PARAMETER_1001_START_ADDRESS,modbus_data);
//	Adress_Connect_06_command_buf	=	0;																										//不在上报数据
//	
//	
//	
//	write_dgus_vp(TOUCH_EVENT_FLAG	+	(modbus_addr + PARAMETER_1001_START_ADDRESS),(u8*)&Connect_data,1);	//显示到屏幕上

}
/*
*函数：void connect_Master_command_handle_process(void)
*功能：联网模块MODBUS发送参数，根据协议自行修改
*/
void connect_Master_command_handle_process(void)
{
	int16_t	connect_send[MODBUS_PACK];
	uint8_t	temp,i;
	memset(connect_send, 0, sizeof(connect_send));
	
	if(Connect_addr	!=	0	)
	{																							//	平台应答
		temp	=	Connect_Send_Data(MASTER_06_COMMAND,Connect_addr,Connect_data,&Connect_data);
		if(temp	==	0)
		{																										//发送完成
			Connect_addr	=	0;
		}
	}
	else	if(Adress_Connect_06_command_buf	!= 0	)
	{
																								//线控器有数据修改
		temp	=	Connect_Send_Data(MASTER_03_COMMAND,Adress_Connect_06_command_buf,1,&Data_Connect_06_command_buf);
		if(temp	==	0)
		{																										//发送完成
			Adress_Connect_06_command_buf	=	0;
		}
	}
	else	if(WF_WAIT_SEND	==	Connect.at_cmd)
	{
		if(Flag_Connect_03_command	<	Connect_length	&&	Flag_Connect_function_03_command	==	0)
		{
			switch(Flag_Connect_03_command)
			{
				case	0:
				{
					Flag_Connect_03_command++;
					Flag_Connect_function_03_command	=	1;
				}
				break;
				
				case	1:
				{
					Flag_Connect_03_command++;
					Flag_Connect_function_03_command	=	2;
				}
				break;
				
				case	2:
				{
					if(Send_State_Time	>	1000 || Send_State_Time < 5)
					{
						Send_State_Time	=	300;
					}
					
					if(T_Connect_Send_Advance_time >= Send_State_Time * 6)
					{
						T_Connect_Send_Advance_time = 0;
						Connect.at_cmd	=	SEND_DATA;//WF_WAIT_SEND;//SEND_DATA;
						Flag_Connect_function_03_command	=	1;
					}
					else if(T_Connect_Send_State_time	>=	Send_State_Time)
					{																													//计时
						T_Connect_Send_State_time	=	0;
						Connect.at_cmd	=	SEND_DATA;//WF_WAIT_SEND;//SEND_DATA;
						Flag_Connect_function_03_command	=	2;
					}
				}
				break;
				default:break;
			}
		}
		
		switch(Flag_Connect_function_03_command)
		{
			case	1:
			{																							//高级参数
				for(i = 0; i < 100	;i++)
				{
					if(Connect_Para_Data[i][1]	==	PARA)
					{
						connect_send[i]	=	Para_pack[Connect_Para_Data[i][0]	-	PARAMETER_92_START_ADDRESS];
					}
					else	if(Connect_Para_Data[i][1]	==	STATUS)
					{
						connect_send[i]	=	State_pack[Connect_Para_Data[i][0]	-	PARAMETER_20_START_ADDRESS];
					}
					else
					{
						connect_send[i]	=	Connect_Para_Data[i][0];
					}
				}
				connect_send[6]	=	Send_State_Time;
				
				
				
				if(Para_pack[PARA_MODE]	==	COOLING)
				{
					if(Para_pack[PARA_MUTE]	==	ECO)
						connect_send[16]	=	6;
					else
						connect_send[16]	=	7;
				}
				else
				{
					if(Para_pack[PARA_MUTE]	==	ECO)
						connect_send[16]	=	4;
					else
						connect_send[16]	=	5;
				}
				
				connect_send[0]	=	monitor_mode;									//监控模式
				connect_send[1]	=	monitor_timer;								//监控间隔时长
				connect_send[2]	=	monitor_time;									//监控时间
				
				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_1000_START_ADDRESS,100,&connect_send[0]);
				if(temp	==	0)
				{
					Flag_Connect_function_03_command = 0;
				}
			}
				break;
			
			case	2:
			{																							//状态参数
				for(i = 0; i < 50	;i++)
				{
					if(Connect_Status_Data[i]	!=	0)
					{
						connect_send[i]	=	State_pack[Connect_Status_Data[i] - PARAMETER_20_START_ADDRESS];
					}
				}
				
				if(Para_pack[PARA_MODE]	==	COOLING)
				{
					if(Para_pack[PARA_MUTE]	==	ECO)
						connect_send[1]	=	6;
					else
						connect_send[1]	=	7;
				}
				else
				{
					if(Para_pack[PARA_MUTE]	==	ECO)
						connect_send[1]	=	4;
					else
						connect_send[1]	=	5;
				}
				
				
				connect_send[34]	=	WIFI_dBM;
				
				connect_send[35]	=	State1_pack[29];
				connect_send[36]	=	State1_pack[30];
				connect_send[37]	=	State1_pack[31];
				connect_send[38]	=	State1_pack[32];
				
				connect_send[44]	= Read_VP(TEMP_REAL);
				
				connect_send[45]	= 	totalHour / 10;// 每小时耗电量（0.1kWh）
				connect_send[46]	=		totalDay / 10;// 每天耗电量（0.1kWh）
				connect_send[47]	=		0;// 每月用电量_H
				connect_send[48]	=		(int16_t)(totalMonth / 100UL);// 每月用电量_L（内部0.01，对外仍/10）
				
				connect_send[50]	=	State2_pack[STATE_ENABLE_UNLOCK];	// host unlock enable
				connect_send[51]	=	State2_pack[STATE_FLAG_UNLOCK];	// host unlock flag
				connect_send[52]	=	State2_pack[STATE_LOCATION_STATUS];	// location status
				
				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_2000_START_ADDRESS,53,&connect_send[0]);
				if(temp	==	0)
				{
					Flag_Connect_function_03_command = 0;
				}
			}
				break;
			default:break;
		}
		
		
		
		
	}
		
	
	
	
	
	
//	if(Flag_Connect_03_command	<	Connect_length	&&	WF_WAIT_SEND	==	Connect.at_cmd)//	&&	send_en	== 1)
//	{																											//上报高级参数或者状态参数
//		switch(Flag_Connect_03_command)
//		{
//			case	0:
//			{
//				Flag_Connect_function_03_command = 1;
//				Flag_Connect_03_command++;
//			}
//				break;
//			
//			case	1:
//			{
//				Flag_Connect_function_03_command = 2;
//				Flag_Connect_03_command++;
//			}
//				break;
//			
////			case	2:
////			{
////				Flag_Connect_function_03_command = 3;
////				Flag_Connect_03_command++;
////			}
////				break;
////			
////			case	3:
////			{
////				Flag_Connect_function_03_command = 4;
////				Flag_Connect_03_command++;
////			}
////				break;
////			
////			case	4:
////			{
////				Flag_Connect_function_03_command = 5;
////				Flag_Connect_03_command++;
////			}
////				break;
////			
////			case	5:
////			{
////				Flag_Connect_function_03_command = 6;
////				Flag_Connect_03_command++;
////			}
////				break;
////			
////			case	6:
////			{
////				Flag_Connect_function_03_command = 7;
////				Flag_Connect_03_command++;
////			}
////				break;
//			default:break;
//		}

//	}
//	else
//	{
//		if(T_Connect_Send_State_time	>=	Send_State_Time	&&	Connect.at_cmd	==	WF_WAIT_SEND)
//		{
//			T_Connect_Send_State_time	=	0;
//			Flag_Connect_function_03_command	=	2;
//			Connect.at_cmd	=	SEND_DATA;
//			Connect_reset_time();
//		}
//	}

//	if(Connect_addr	!=	0	)
//	{																											//应答平台
////		send_en	=	0;																				//上传高级参数优先级最低
////		temp	=	Connect_Send_Data(MASTER_06_COMMAND,Connect_addr,Para_pack[Connect_addr - PARAMETER_1001_START_ADDRESS],&Connect_data);
//		if(temp	==	0)
//		{																										//发送完成
//			Connect_addr	=	0;
//		}
//	}
//	else	if(Adress_Connect_06_command_buf	!= 0)
//	{																											//线控器有数据修改主动上报数据
////		send_en	=	0;																				//上传高级参数优先级最低
//		temp	=	Connect_Send_Data(MASTER_03_COMMAND,Adress_Connect_06_command_buf,1,&Data_Connect_06_command_buf);
//		if(temp	==	0)
//		{																										//发送完成
//			Adress_Connect_06_command_buf	=	0;
//		}
//	}
//	else	
//	{
//		switch(Flag_Connect_function_03_command)
//		{
//			case	1:
//				
//				for(i = 0; i < 96	;i++)
//				{
//					if(Connect_Para_Data[i][1]	==	KEY)
//					{
//						if(i	==	9)
//						{																			//线控器版本号
//							connect_send[i]	=	MY_PROGRAM_VERSION;
//						}
//					}
//					else	if(Connect_Para_Data[i][1]	==	EEPROM)
//					{
//						
//					}
//					else	if(Connect_Para_Data[i][1]	==	STATUS)
//					{
//						connect_send[i]	=	State_pack[Connect_Para_Data[i][0]];
//					}
//					else	if(Connect_Para_Data[i][1]	==	PARA)
//					{
//						connect_send[i]	=	Para_pack[Connect_Para_Data[i][0]];
//					}
//				}
//			
//				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_1000_START_ADDRESS,96,&connect_send[0]);
//				if(temp	==	0)
//				{
//					Flag_Connect_function_03_command = 0;
//				}
//					
//				break;
//			
//			case	2:

//				for(i = 0; i < 39	;i++)
//				{
//					if(Connect_Status_Data[i]	!=	OFF)
//					{
//						connect_send[i]	=	State_pack[Connect_Status_Data[i]];
//					}
//				}

//				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_2000_START_ADDRESS,39,&connect_send[0]);
//				if(temp	==	0)
//				{
//					Flag_Connect_function_03_command = 0;
//				}
//				break;
//			
////			case	3:

//////				Connect_Para_Data_C_F(PARAMETER_1201_START_ADDRESS,connect_send);
//////				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_1201_START_ADDRESS,MODBUS_PACK,connect_send);
////				if(temp	==	0)
////				{
////					Flag_Connect_function_03_command = 0;
////				}
////				break;
////			
////			case	4:

//////				Connect_Para_Data_C_F(PARAMETER_1301_START_ADDRESS,connect_send);
//////				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_1301_START_ADDRESS,MODBUS_PACK,connect_send);
////				if(temp	==	0)
////				{
////					Flag_Connect_function_03_command = 0;
////				}
////				break;
////			
////			case	5:

//////				Connect_Para_Data_C_F(PARAMETER_1401_START_ADDRESS,connect_send);
//////				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_1401_START_ADDRESS,MODBUS_PACK,connect_send);
////				if(temp	==	0)
////				{
////					Flag_Connect_function_03_command = 0;
////				}
////				break;
////			
////			case	6:

//////				Connect_Para_Data_C_F(PARAMETER_1501_START_ADDRESS,connect_send);
//////				temp	=	Connect_Send_Data(MASTER_03_COMMAND,PARAMETER_1501_START_ADDRESS,MODBUS_PACK,connect_send);
////				if(temp	==	0)
////				{
////					Flag_Connect_function_03_command = 0;
////				}
////				break;
////			
////			case	7:
////				State_pack[STATE_XKQ_VERSION]	=	XKQ_VERSION_ADRESS;																			//线控器版本号
////				for(i = 0; i < MODBUS_PACK	;i++)
////				{
//////					if(Status_DATA[i] == TEMP_10)
//////						connect_send[i] = EXCHANGE_C_F(State_pack[i],Para_pack[PARA_N04_1027]);
//////					else
////						connect_send[i] = State_pack[i];
////				}
////				
//////				temp	=	Connect_Send_Data(MASTER_03_COMMAND,STATE_2001_START_ADDRESS,MODBUS_PACK,connect_send);
////				if(temp	==	0)
////				{
////					Flag_Connect_function_03_command = 0;
////				}
////				break;
//			default:break;
//		}
//	}
}

/****************************************************************************************/
//Function.Name:    Send_modebus_command_06()
//Function.Content: modbus 06 command Trigger instruction
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Connect_Send_modebus_command_06(int16_t address_06,int16_t data_06)
{
	Adress_Connect_06_command_buf	=	address_06;
	
	
	if(Adress_Connect_06_command_buf	==	1016)
	{
		if(Para_pack[PARA_MODE]	==	COOLING)
		{
			if(Para_pack[PARA_MUTE]	==	ECO)
				Data_Connect_06_command_buf	=	6;
			else
				Data_Connect_06_command_buf	=	7;
				
		}
		else
		{
			if(Para_pack[PARA_MUTE]	==	ECO)
				Data_Connect_06_command_buf	=	4;
			else
				Data_Connect_06_command_buf	=	5;
		}
	}
	else
	{
		Data_Connect_06_command_buf	=	data_06;
	}
	
	
	
}


/*
*函数：void	Connect_Para_Data_C_F(uint16_t	addr,int16_t connect_send[])
*功能：摄氏度转华氏度
*/
//void	Connect_Para_Data_C_F(uint16_t	addr,int16_t connect_send[])
//{
//	int8_t	i;
//	for(i = 0; i < MODBUS_PACK	;i++)
//	{
//		if(Para_init_DATA[i + (addr	-	PARAMETER_1001_START_ADDRESS)][2] == TEMP_10)
//			connect_send[i] = EXCHANGE_C_F(Para_pack[i + (addr	-	PARAMETER_1001_START_ADDRESS)],Para_pack[PARA_N04_1027]);
//		else
//			connect_send[i] = Para_pack[i + (addr	-	PARAMETER_1001_START_ADDRESS)];
//	}
//}

