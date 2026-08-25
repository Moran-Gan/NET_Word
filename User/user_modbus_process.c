#include	"Dwin_CONF.h"
#include	"user_include.h"

uint16_t crc16(uint8_t *dat, uint16_t length)  
{  
    u8 i;  
    u16 crc = 0xffff;        // Initial value  
    while(length--)  
    {  
        crc ^= *dat++;            // crc ^= *dat; dat++;  
        for (i = 0; i < 8; ++i)  
        {  
            if (crc & 1)  
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005  
            else  
                crc = (crc >> 1);  
        }
    }
    return crc;  
}

/****************************************************************************************/
//Function.Name:    R_User_modbus_receive()
//Function.Content: Low-level receiving function processing
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
//void R_User_modbus_receive(void)
//{
//	uint16_t rx_length;
//	if(SLAVE_ADRESS == modbus_Rxbuf[0])
//	{
//		if(T_modbus_rx_count > 2)
//		{
//			switch(modbus_Rxbuf[1])
//			{
//				case 0x03:
//						if(T_modbus_rx_count > (modbus_Rxbuf[2] + 4))
//						{
//							Flag_modbus_receive_ok = 1;
//							Rx_CRC_length = modbus_Rxbuf[2] + 3;
//							T_modbus_rx_count = 0;
//						}
//						else
//						{
//							;
//						}
//						
//				break;
//				case 0x06:
//						if(T_modbus_rx_count >= ANALYZE_FUNCTION_06_LENGTH)
//						{
//							Flag_modbus_receive_ok = 1;
//							Rx_CRC_length = ANALYZE_FUNCTION_06_LENGTH - 2;
//							T_modbus_rx_count = 0;
//						}
//						else
//						{
//							
//						}
//				break;
//				case 0x10:
//						rx_length = (modbus_Rxbuf[4] << 8) + modbus_Rxbuf[5];
//						if(T_modbus_rx_count > (rx_length + 4))
//						{
//							Flag_modbus_receive_ok = 1;
//							Rx_CRC_length = rx_length + 2;
//							T_modbus_rx_count = 0;
//						}
//						else
//						{
//							
//						}
//						
//				break;
//				default:
//					T_modbus_rx_count = 0;
//				break;
//			}
//		}
//	}
//	else
//	{
//		T_modbus_rx_count = 0;
//	}
//	
//}

/****************************************************************************************/
//Function.Name:    Master_receive_handle_process()
//Function.Content: Data receiving process
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Master_receive_handle_process(void)
{
	uint16_t CRC_check_Value;
	uint16_t CRC_check_Value1;
	uint8_t  CRC_check = 0, i = 0;
	CRC_check_Value = crc16(modbus_Rxbuf,Rx_CRC_length-2);
	CRC_check_Value1	=	modbus_Rxbuf[Rx_CRC_length-1]<<8|modbus_Rxbuf[Rx_CRC_length-2];
	if(CRC_check_Value	==	CRC_check_Value1)
	{
		CRC_check = 1;
	}
	else
	{
		CRC_check = 0;
	}
	if(CRC_check)
	{
		Time_communication_30S = 30;
		switch(modbus_Rxbuf[1])
		{
			case 0x03:
					Analyze_function_03_data(modbus_Rxbuf);
			break;
			case 0x06:
					Analyze_function_06_data(modbus_Rxbuf);
			break;
			case 0x10:
					Analyze_function_10_data(modbus_Rxbuf);
			break;
			case 0x83:		// RS485错误码处理
					if (Flag_function_03_command == 0x03)
					{
						Enable_Host_Unlock = 0;		// 判定为旧版本程序,主板无解锁功能
						for (i = 0; i < MODBUS_06_PACK; i++)
						{
							State2_pack[i] = 0;
						}
						write_dgus_vp(PARAMETER_250_START_ADDRESS + 0x5000,(u8*)&State2_pack[PARAMETER_250_START_ADDRESS	-	PARAMETER_250_START_ADDRESS],MODBUS_06_PACK);
						T_first_power_on_03_commnd	=	2;
					}
			break;
			default:
			break;
		}
	}
}

/****************************************************************************************/
//Function.Name:    Analyze_function_03_data()
//Function.Content: Analyze 03 instructions
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Analyze_function_03_data(uint8_t rx_03_buff[])
{
	uint16_t i = 0;
	uint8_t j = 0;
	uint8_t	time[7];
	uint16_t	DATA;

	switch(Flag_function_03_command)
	{
		case 0x01: 
		{
				j=3;
				for(i=0;i<MODBUS_81_PACK;i++)
				{
					Para_pack[i + (PARAMETER_92_START_ADDRESS	-	PARAMETER_92_START_ADDRESS)] = (rx_03_buff[j] << 8) + rx_03_buff[j+1];
					j +=2;
				}
				write_dgus_vp(PARAMETER_92_START_ADDRESS + 0x5000,(u8*)&Para_pack[PARAMETER_92_START_ADDRESS	-	PARAMETER_92_START_ADDRESS],MODBUS_81_PACK);
				DATA	=	1;
				write_dgus_vp(DIPS_INIT_MODGUS_PARA,(u8*)&DATA,1);								
				Flag_function_03_command = 0;
//				Flag_Connect_03_command	=	0;
		}
		break;
		case 0x02:
		{
			j=3;
			for(i=0;i<MODBUS_20_PACK;i++)
			{
				Para_pack_174[i + (PARAMETER_174_START_ADDRESS	-	PARAMETER_174_START_ADDRESS)] = (rx_03_buff[j] << 8) + rx_03_buff[j+1];
				Para_pack[i+82]= (rx_03_buff[j] << 8) + rx_03_buff[j+1];
				j +=2;
			}
			write_dgus_vp(PARAMETER_174_START_ADDRESS + 0x5000,(u8*)&Para_pack_174[PARAMETER_174_START_ADDRESS	-	PARAMETER_174_START_ADDRESS],MODBUS_20_PACK);
			Flag_function_03_command = 0;
		}
		break;
		case 0x03:
		{
				j=3;
				for(i=0;i<MODBUS_06_PACK;i++)
				{
					State2_pack[i + (PARAMETER_250_START_ADDRESS	-	PARAMETER_250_START_ADDRESS)] = (rx_03_buff[j] << 8) + rx_03_buff[j+1];
					j +=2;
				}
			
				if (State2_pack[STATE_ENABLE_UNLOCK] != 0)		// 新机制版本,主板有解锁功能
				{
					Enable_Host_Unlock = 1;
				}
				else
				{
					Enable_Host_Unlock = 0;
				}
				
				write_dgus_vp(PARAMETER_250_START_ADDRESS + 0x5000,(u8*)&State2_pack[PARAMETER_250_START_ADDRESS	-	PARAMETER_250_START_ADDRESS],MODBUS_06_PACK);
				Flag_function_03_command = 0;
		}
		break;
		
		case 0x04:
		{
				j=3;
				for(i=0;i<MODBUS_33_PACK;i++)
				{
					State1_pack[i + (PARAMETER_200_START_ADDRESS	-	PARAMETER_200_START_ADDRESS)] = (rx_03_buff[j] << 8) + rx_03_buff[j+1];
					j +=2;
				}
				
				if (State1_pack[STATE_NETWORK_STATUS])		// 4G联网状态
				{
					T_Network_Offline = 0;
				}
				
				if(State_pack[STATE_PARA]	&	0x01	)//&&	(wifi_work_state	!=	4))																														//��ȡ״̬������WiFi������״̬����״̬
				{
					
					time[1]	=	State1_pack[0] >> 0;					//��
					time[2]	=	State1_pack[0] >> 8;					//��
					
					time[3]	=	State1_pack[1] >> 0;					//��
					time[4]	=	State1_pack[1] >> 8;					//ʱ
					
					time[5]	=	State1_pack[2] >> 0;					//��
					time[6]	=	State1_pack[2] >> 8;					//��
					
					time[7]	=	getWeekdayByYearday(time[1],time[2],time[3]);//RTC_Get_Week(rtc_set_t[0],rtc_set_t[1],rtc_set_t[2]);;																	//��
					if((time[1]	>	0	&&	time[1]	<	99)	&&	(time[2]	>	0	&&	time[2]	<	13)&&	(time[3]	>	0	&&	time[3]	<	32)&&	(time[4]	>=	0	&&	time[4]	<	24)&&	(time[5]	>=	0	&&	time[5]	<	60)&&	(time[6]	>=	0	&&	time[6]	<	60))
					{
						if(GET_BIT(Read_dgus_buf[0],7))
						{
							Write_time_set_confirm(time);
						}
					}
				}
				
				State_pack[STATE_PARA]	= 0;
				
				write_dgus_vp(IMEI,(u8*)&State1_pack[203	-	PARAMETER_200_START_ADDRESS],8);
				write_dgus_vp(PARAMETER_200_START_ADDRESS + 0x5000,(u8*)&State1_pack[PARAMETER_200_START_ADDRESS	-	PARAMETER_200_START_ADDRESS],MODBUS_33_PACK);
				Flag_function_03_command = 0;
		}
		break;
		
		case 0x05:
		{
				j=3;
				for(i=0;i<MODBUS_72_PACK;i++)
				{
					State_pack[i + (PARAMETER_20_START_ADDRESS	-	PARAMETER_20_START_ADDRESS)] = (rx_03_buff[j] << 8) + rx_03_buff[j+1];
					j +=2;
				}
				if(State_pack[STATE_TEMP_OUT]<0)
				{
					State_pack[STATE_TEMP_OUT] = 0;
				}
				if(State_pack[STATE_TEMP_IN]<0)
				{
					State_pack[STATE_TEMP_IN] = 0;
				}
				write_dgus_vp(PARAMETER_20_START_ADDRESS + 0x5000,(u8*)&State_pack[PARAMETER_20_START_ADDRESS	-	PARAMETER_20_START_ADDRESS],MODBUS_72_PACK);
				Flag_function_03_command = 0;			
				read_dgus_vp(DIPS_INIT_MODGUS_PARA,(u8*)&DATA,1);		
				if(DATA	==	0)																								//��ȡ��״̬�������Ҹ߼�����δ��ȡ��ɣ�����ִ�лض�����
				{
					T_first_power_on_03_commnd	=	0;
				}
				Wite_VP(0x5000+62,Read_VP(TEMP_REAL));
				Wite_VP(0x5000+63,Read_VP(DHT83_REAL));
		}
		break;
				
		default:
		break;
	}
}

/****************************************************************************************/
//Function.Name:    Analyze_function_06_data()
//Function.Content: Analyze 06 instructions
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Analyze_function_06_data(uint8_t rx_06_buff[])
{
	int16_t rx_06_adress = 0;
	int16_t rx_06_data = 0;
	rx_06_adress = (rx_06_buff[2] << 8) + rx_06_buff[3];
	rx_06_data = (rx_06_buff[4] << 8) + rx_06_buff[5];
	if(Flag_function_06_command)
	{
		if(rx_06_adress == Adress_function_06_command_buf)
		{
			if((rx_06_data != Data_function_06_command_buf))// && (!Flag_OnlineTest))
			{
//				Para_pack[rx_06_adress] = rx_06_data;
//				Send_modebus_command_06(rx_06_adress,rx_06_data);
			}
			else
			{
				Flag_function_06_command	=	0;
				if(rx_06_adress	==	(PARA_RESET	+	PARAMETER_92_START_ADDRESS))
				{																							//ת���¶ȵ�λ���߻ָ��������ö���Ҫ��ȡȫ���߼�����
					T_first_power_on_03_commnd	=	0;
//					Flag_read_all_data = 1;
				}
//				else	if(rx_06_adress	==	(PARA_1010 + PARAMETER_92_START_ADDRESS))
//				{
//					if(GET_BIT(State_pack[STATE_2024],0))
//					{
//						T_first_power_on_03_commnd = 0;
//					}
//					else	if(GET_BIT(State_pack[STATE_2024],1))
//					{
//						T_first_power_on_03_commnd = 1;
//					}else	if(GET_BIT(State_pack[STATE_2024],2))
//					{
//						T_first_power_on_03_commnd = 2;
//					}else	if(GET_BIT(State_pack[STATE_2024],3))
//					{
//						T_first_power_on_03_commnd = 3;
//					}else	if(GET_BIT(State_pack[STATE_2024],4))
//					{
//						T_first_power_on_03_commnd = 4;
//					}else	if(GET_BIT(State_pack[STATE_2024],5))
//					{
//						T_first_power_on_03_commnd = 5;
//					}
//				}
			}
//			else if (Flag_OnlineTest)
//			{
//				if(rx_06_adress == (21))
//				{
//					Flag_mainboard_onlinetest = rx_06_data;
//					F_OnlineTest_items |= ONLINETEST_MODBUS_OK;
//				}
//			}
		}
		else
		{
			;
		}
	}
	else
	{
		;
	}
}

/****************************************************************************************/
//Function.Name:    Analyze_function_10_data()
//Function.Content: Analyze 10 instructions
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Analyze_function_10_data(uint8_t rx_10_buff[])
{
	int16_t rx_10_adress = 0;
	int16_t rx_10_length = 0;
	rx_10_adress = (rx_10_buff[2] << 8) + rx_10_buff[3];
	rx_10_length = (rx_10_buff[4] << 8) + rx_10_buff[5];
	if(Flag_function_10_command)
	{
		if((rx_10_adress == Adress_function_10_command) && (rx_10_length == Length_function_10_command))
		{
			Flag_function_10_command = 0;
		}
	}
	else
	{
		;
	}
}

/****************************************************************************************/
//Function.Name:    Master_command_handle_process()
//Function.Content: Host computer roll call sequence
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Master_command_handle_process(void)
{
	if(T_first_power_on_03_commnd < 5)//������ʼ����ȡ
	{
		switch(T_first_power_on_03_commnd)
		{
			case	0:
			{
				Flag_function_03_command = 0x01;
				T_first_power_on_03_commnd = 1;
				Flag_function_03_count	=	3;
			}
				break;
			
			case	1:
			{
				if(!Flag_function_03_command	||	(!Flag_function_03_count))
				{
					Flag_function_03_command = 0x02;
					T_first_power_on_03_commnd = 2;
					Flag_function_03_count	=	3;
				}
				else
				{
					Flag_function_03_command = 0x01;
				}
			}
				break;
			
			case	2:
			{
				if(!Flag_function_03_command	||	(!Flag_function_03_count))
				{
					Flag_function_03_command = 0x03;
					T_first_power_on_03_commnd = 3;
					Flag_function_03_count	=	3;
					if(Flag_Interface)
					{
						write_PIC_Set(Flag_Interface);
						Flag_Interface	=	0;
					}
				}
				else
				{
					Flag_function_03_command = 0x02;
				}
			}
				break;
			
			case	3:
			{
				if(!Flag_function_03_command	||	(!Flag_function_03_count))
				{
					Flag_function_03_command = 0x04;
					T_first_power_on_03_commnd = 4;
					Flag_function_03_count	=	3;
					UART_MODBUS_OK	=	true;
				}
				else
				{
					Flag_function_03_command = 0x03;
				}
			}
				break;	
			case	4:
			{
				if(!Flag_function_03_command)
				{
					Flag_function_03_command = 0x05;
					T_first_power_on_03_commnd = 5;
				}
				else
				{
					Flag_function_03_command = 0x04;
				}
			}
				break;
//			
//			case	5:
//			{
//				if(!Flag_function_03_command)
//				{
//					Flag_function_03_command = 0x06;
//					T_first_power_on_03_commnd = 6;
//				}
//				else
//				{
//					Flag_function_03_command = 0x05;
//				}
//			}
//				break;
//			
//			case	6:
//			{
//				if(!Flag_function_03_command)
//				{
//					Flag_function_03_command = 0x07;
//					T_first_power_on_03_commnd = 7;
//				}
//				else
//				{
//					Flag_function_03_command = 0x06;
//				}
//			}
//				break;
//			
//			case	7:
//			{
//				if(!Flag_function_03_command)
//				{
//					Flag_function_03_command = 0x08;
//					T_first_power_on_03_commnd = 8;
//					if(Flag_Interface)
//					{
//						write_PIC_Set(Flag_Interface);
//						Flag_Interface	=	0;
//					}
//					Flag_read_all_data	=	0;
//				}
//				else
//				{
//					Flag_function_03_command = 0x07;
//				}
//			}
//				break;
			default:break;
		}
	}
	else
	{

		if(Flag_function_06_command)//06д
		{
			Flag_function_06_command -- ;
			Master_Modbus_06_Fountion(Adress_function_06_command,Data_function_06_command);
		}
		else if(Flag_function_10_command)//10д
		{
			Flag_master_tx_command = 1;
		}
		else	if(modbus_send_06_length > 0)									//���ۼ�06ָ��δ�������
		{
			modbus_send_06_length--;
			Adress_function_06_command = modbus_send_06[modbus_send_06_length][0];
			Data_function_06_command = modbus_send_06[modbus_send_06_length][1];
			Flag_function_06_command = 3;
			Master_Modbus_06_Fountion(Adress_function_06_command,Data_function_06_command);
		}
//		else if (SYSTEM_PASSWORD == read_PIC_Now())  // 密码界面，循环读取4帧数据
//		{
//			if(4 == T_first_power_on_03_commnd)
//			{
//				T_first_power_on_03_commnd = 5;
//				Flag_function_03_command = 0x01;
//			}
//			else if (5 == T_first_power_on_03_commnd)
//			{
//				T_first_power_on_03_commnd = 6;
//				Flag_function_03_command = 0x02;
//			}
//			else if (6 == T_first_power_on_03_commnd)
//			{
//				T_first_power_on_03_commnd = 7;
//				Flag_function_03_command = 0x03;
//			}
//			else
//			{
//				T_first_power_on_03_commnd = 4;
//				Flag_function_03_command = 0x04;
//			}
//		}
// 		else	if(State_pack[STATE_PARA]	&	0x0D)							//��������ota�лض�����״̬
// 		{																										//ͬ������ʱ����IMEI
// //			//State_pack[STATE_PARA]	&= ~0X05;
// 			T_first_power_on_03_commnd	=	3;
// 			Flag_function_03_command = 0x03;
// 		}

		else if(GET_BIT(State_pack[STATE_PARA],1))
		{																										//����ͬ������
			CLR_BIT(State_pack[STATE_PARA],1);
			T_first_power_on_03_commnd	=	1;
			Flag_function_03_command = 0x01;
		}
		else if(GET_BIT(State_pack[STATE_PARA],2))
		{
			T_first_power_on_03_commnd = 4;
			Flag_function_03_command = 0x04;
		}
		else if(4 == T_first_power_on_03_commnd)
		{
			T_first_power_on_03_commnd = 5;
			Flag_function_03_command = 0x02;
		}
		else if (5 == T_first_power_on_03_commnd)
		{
			T_first_power_on_03_commnd = 4;
			Flag_function_03_command = 0x04;
		}
		else if (6 == T_first_power_on_03_commnd)
		{
			T_first_power_on_03_commnd = 5;
			Flag_function_03_command = 0x05;
		}		
	}
	if((!Flag_function_06_command) && (!Flag_function_10_command))//û���ԡ�ûд����
	{
		switch(Flag_function_03_command)
		{
			case 0x01:
			    Master_Modbus_03_Fountion(PARAMETER_92_START_ADDRESS,MODBUS_81_PACK);
			break;
			case 0x02:
				Master_Modbus_03_Fountion(PARAMETER_174_START_ADDRESS,MODBUS_20_PACK);
			break;
			case 0x03:
				Master_Modbus_03_Fountion(PARAMETER_250_START_ADDRESS,MODBUS_06_PACK);
			break;
			case 0x04:
			    Master_Modbus_03_Fountion(PARAMETER_200_START_ADDRESS,MODBUS_33_PACK);
			break;
			case 0x05:
				Master_Modbus_03_Fountion(PARAMETER_20_START_ADDRESS,MODBUS_72_PACK);
			break;
//			case 0x05:
//			      Master_Modbus_03_Fountion(PARAMETER_1361_START_ADDRESS,MODBUS_PACK);
//			break;
//			case 0x06:
//						Master_Modbus_03_Fountion(PARAMETER_1451_START_ADDRESS,MODBUS_PACK);
//			break;
//			case 0x07:
//						Master_Modbus_03_Fountion(PARAMETER_1541_START_ADDRESS,MODBUS_PACK);
//			break;
//			case 0x08:
//						Master_Modbus_03_Fountion(PARAMETER_2000_START_ADDRESS,MODBUS_PACK);
//			break;

			default:
			break;
		}
		if(Flag_function_03_count	>	0)
			Flag_function_03_count--;
	}
}

/****************************************************************************************/
//Function.Name:    Fill_master_pack()
//Function.Content: Master fill send data
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Fill_master_pack(uint8_t CRC_tx_length)
{
	uint16_t CRC_check_Value = 0;
	modbus_TXbuf[0] = SLAVE_ADRESS;									//Header
	CRC_check_Value = crc16(modbus_TXbuf,CRC_tx_length);
	modbus_TXbuf[CRC_tx_length]   = (CRC_check_Value&0x00FF);		//Error Check Lo
	modbus_TXbuf[CRC_tx_length+1] = (CRC_check_Value&0xFF00) >> 8;	//Error Check Hi
}

/****************************************************************************************/
//Function.Name:    Master_Modbus_03_Fountion()
//Function.Content: master ctrol 0x03 command
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Master_Modbus_03_Fountion(uint16_t start_adress,uint16_t tx_length)
{
	memset(modbus_TXbuf, 0, sizeof(modbus_TXbuf));
	modbus_TXbuf[1] = MASTER_03_COMMAND;			//Function
	modbus_TXbuf[2] = (start_adress&0xFF00) >> 8;	//Starting Address Hi
	modbus_TXbuf[3] = (start_adress&0x00FF);		//Starting Address Lo
	modbus_TXbuf[4] = (tx_length&0xFF00) >> 8;		//Quantity of Registers Hi
	modbus_TXbuf[5] = (tx_length&0x00FF);			//Quantity of Registers Lo
	Tx_CRC_length = MASTER_03_COMMAND_CRC_LENGTH;
	Fill_master_pack(Tx_CRC_length);
	Flag_master_tx_command = 1;
}

/****************************************************************************************/
//Function.Name:    Master_Modbus_06_Fountion()
//Function.Content: master ctrol 0x06 command
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Master_Modbus_06_Fountion(int16_t start_adress,int16_t tx_data)
{
	memset(modbus_TXbuf, 0, sizeof(modbus_TXbuf));
	modbus_TXbuf[1] = MASTER_06_COMMAND;			//Function
	modbus_TXbuf[2] = (start_adress&0xFF00) >> 8;	//Starting Address Hi
	modbus_TXbuf[3] = (start_adress&0x00FF);		//Starting Address Lo
	modbus_TXbuf[4] = (tx_data&0xFF00) >> 8;		//Write Data Hi
	modbus_TXbuf[5] = (tx_data&0x00FF);				//Write Data Lo
	Tx_CRC_length = MASTER_06_COMMAND_CRC_LENGTH;
	Fill_master_pack(Tx_CRC_length);
	if(start_adress >= PARAMETER_92_START_ADDRESS && start_adress < PARAMETER_200_START_ADDRESS)
	{
		Connect_Send_modebus_command_06(923+(start_adress),tx_data);//线控器操作发送到云平台	
	}
	Flag_master_tx_command = 1;
}
/****************************************************************************************/
//Function.Name:    Master_Modbus_10_Fountion()
//Function.Content: master ctrol 0x10 command
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Master_Modbus_10_Fountion(int16_t start_adress,uint16_t tx_data_buf[],uint16_t tx_length)
{
	uint16_t i,j;
	memset(modbus_TXbuf, 0, sizeof(modbus_TXbuf));
	modbus_TXbuf[1] = MASTER_10_COMMAND;			//Function
	modbus_TXbuf[2] = (start_adress&0xFF00) >> 8;	//Starting Address Hi
	modbus_TXbuf[3] = (start_adress&0x00FF);		//Starting Address Lo
	modbus_TXbuf[4] = (tx_length&0xFF00) >> 8;		//Quantity of Registers Hi
	modbus_TXbuf[5] = (tx_length&0x00FF);			//Quantity of Registers Lo
	modbus_TXbuf[6] =  tx_length * 2;				//Byte Count
	j = 0;
	for(i=0;i<tx_length;i++)
	{
		modbus_TXbuf[j]   = (tx_data_buf[i]&0xFF00) >> 8;
		modbus_TXbuf[j+1] = (tx_data_buf[i]&0x00FF);
		j += 2;
	}
	Tx_CRC_length = (tx_length * 2) + 7;
	Fill_master_pack(Tx_CRC_length);
}

/****************************************************************************************/
//Function.Name:    Master_send()
//Function.Content: 485 Master_send process
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Master_send(void)
{
	if(2 == Flag_master_tx_command)
	{
		Flag_master_tx_command = 3;
		Tx_senddata_num = Tx_CRC_length+2;
		SCON3T |= 0x01;
		if(0x06 == modbus_TXbuf[1]) 
		{
			Adress_function_06_command_buf = (modbus_TXbuf[2] << 8) + modbus_TXbuf[3];
			Data_function_06_command_buf = (modbus_TXbuf[4] << 8) + modbus_TXbuf[5];
		}
	}
}

/****************************************************************************************/
//Function.Name:    Rs485_envelope_time_process()
//Function.Content: 485 communication enable terminal control process
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Rs485_envelope_time_process(void)
{
	if(1 == Flag_master_tx_command)
	{
		RS485_TX_EN	=	true;
		if(T_485_DE_envelope_time < 3)
		{
			T_485_DE_envelope_time ++;
		}
		else
		{
			Flag_master_tx_command = 2;
		}	
	}
	else if(2 == Flag_master_tx_command)
	{
	}
	else if(3 == Flag_master_tx_command)
	{
	}
	else if(4 == Flag_master_tx_command)
	{
		if(T_485_DE_envelope_time < 6)
		{
			T_485_DE_envelope_time ++;
		}
		else
		{
			RS485_TX_EN	=	false;
			Flag_master_tx_command = 0;
		}
	}
	else
	{
		T_485_DE_envelope_time = 0;
	}
}

/****************************************************************************************/
//Function.Name:    Send_modebus_command_06()
//Function.Content: modbus 06 command Trigger instruction
//Function.Editor:  cwx
//Function.Time:    2021/04/30
/****************************************************************************************/
void Send_modebus_command_06(int16_t address_06,int16_t data_06)
{
	int16_t	 modbus_send[MODBUS_BUFF_06][2];
	int8_t	 i;
	
	
	
	for(i	=	0;	i< MODBUS_BUFF_06;i++)
	{
		modbus_send[i][0] = modbus_send_06[i][0];
		modbus_send[i][1] = modbus_send_06[i][1];
	}
	
	modbus_send_06[0][0] = address_06;										//����������ǰ��
	modbus_send_06[0][1] = data_06;

	if(modbus_send_06_length	<	MODBUS_BUFF_06)
	{
		modbus_send_06_length++;
	}
	else
	{
		modbus_send_06_length	=	MODBUS_BUFF_06;
	}

	for(i	=	0;	i< (MODBUS_BUFF_06 - 1);i++)												//��ȥ
	{
		modbus_send_06[i+1][0] = modbus_send[i][0];										//����������ǰ��
		modbus_send_06[i+1][1] = modbus_send[i][1];
	}
	Para_pack[(address_06	-	PARAMETER_92_START_ADDRESS)]	=	data_06;
	
	
	

}

