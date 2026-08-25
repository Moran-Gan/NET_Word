#include "Dwin_CONF.h"
#include "user_include.h"

bit write_eeprom_bit = 0;
bit write_flash_bit = 0;
/*****************************************************************************
 �� �� ��  : void Parm_Set_Function(void)
 ��������  : ����ɨ��
 �������  :
 �������  :
 �޸���ʷ  :
  1.��    ��   :
	��    ��   :
	�޸�����   : �޸�
*****************************************************************************/
void Parm_Set_Function(void)
{
	uint16_t data Touch_Event_Flag = 0;
	uint16_t VALUE = 0;
	uint16_t key_set_temp= 0;
	int16_t dgus_data;
	int16_t dgus_data2;
	read_dgus_vp(TOUCH_EVENT_FLAG, (uint8_t *)&Touch_Event_Flag, 1);

	if (Touch_Event_Flag)
	{
		if (Read_dgus_buf[0] & 0x01 && Touch_Event_Flag != key_fault) // �ж��Ƿ��йرհ�������
			write_Button(10);																		   // ��������

		if (Flag_Sleep == 0)
		{
			switch (Touch_Event_Flag)
				
			{
			case key_return:
			{
				Return_Home();
				if (write_eeprom_bit) //存关怀模式
				{
					write_eeprom_bit = 0; // ���дeeprom��־λ
					//T5L_Flash(WRITE, (EEPROM_PARA), (EEPROM_PARA)-MAGIC_NUMBER, 2);
					EEPROM_Write_page();
				}
			}
			break;

			case key_fault:
			{
				read_dgus_vp(DISP_FAULT_ICO, (uint8_t *)&VALUE, 1);
				if (VALUE) // �жϵ�ǰ�Ƿ���ڹ���
				{
					if (Read_dgus_buf[0] & 0x01)
					{
						write_Button(10); // ��������
					}

					write_PIC_Set(page_current_fault);
				}
			}
			break;

			case key_reset_fault:
			{
				Send_modebus_command_06(PARA_RESET_FAULT + PARAMETER_92_START_ADDRESS, ON);
				Return_Home();
			}
			break;

			case key_power:
			{
				if(Read_VP(TEMPERATURE_CONTROL)==0)
				{
					if (Para_pack[PARA_POWER_ON])
						write_PIC_Set(7);
					else
						write_PIC_Set(6);
				}
				else
				{
					if (Read_VP(INDOOR_POWER))
						write_PIC_Set(7);
					else
						write_PIC_Set(6);
				}
			}
			break;

			case key_power_OFF:
			{
				EXIT_VACATION();
				if(Read_VP(TEMPERATURE_CONTROL)==0)
				{
					Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF);
				}
				else
				{
					Wite_VP(INDOOR_POWER, OFF); 
					EEPROM_Write_page();
				}
				Return_Home();
			}
			break;

			case key_power_ON:
			{
				if(Read_VP(TEMPERATURE_CONTROL)==0)
				{
					Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, ON);
				}
				else
				{
					Wite_VP(INDOOR_POWER, ON); 
					EEPROM_Write_page();
				}
				Return_Home();
			}
			break;

			case key_mode:
			{
				if (Para_pack[PARA_F18_122] == 1)
				{ // ��������
					write_PIC_Set(page_mode_heat_cool);
				}
				else
				{
					write_PIC_Set(page_mode_heating);
				}
			}
			break;

			case key_cooling:
			{
				EXIT_VACATION();
				Return_Home();
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS, 0);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);
			}
			break;

			case key_heating:
			{
				EXIT_VACATION();
				Return_Home();
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS, 0);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);
			}
			break;

			case key_cooling_ceo:
			{
				EXIT_VACATION();
				Return_Home();
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS, ECO);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);
			}
			break;

			case key_heating_ceo:
			{
				EXIT_VACATION();
				Return_Home();
				Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS, ECO);
				Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);
			}
			break;

			case key_time_ok:
			{
				time_set_confirm();
			}
			break;

			case key_exit_timer_power:
			{
				if (write_eeprom_bit)
				{
					write_eeprom_bit = 0;
					//T5L_Flash(WRITE, (EEPROM_TIMER_POWER), (EEPROM_TIMER_POWER)-MAGIC_NUMBER, 8);
					EEPROM_Write_page();
				}
				Exit_Timer_Interface();
			}
			break;

			case key_exit_timer_temp:
			{
				if (write_eeprom_bit)
				{
					write_eeprom_bit = 0;
					EEPROM_Write_page();
					//T5L_Flash(WRITE, (EEPROM_TIMER_TEMP - 1), (EEPROM_TIMER_TEMP - 1) - MAGIC_NUMBER, 14);
				}
				Exit_Timer_Interface();
			}
			break;

			case key_exit_timer_eco:
			{
				if (write_eeprom_bit)
				{
					write_eeprom_bit = 0;
					EEPROM_Write_page();
					//T5L_Flash(WRITE, (EEPROM_TIMER_MUTE - 1), (EEPROM_TIMER_MUTE - 1) - MAGIC_NUMBER, 8);
				}
				Exit_Timer_Interface();
			}
			break;

			case key_Def:
			{
				Send_modebus_command_06(PARA_DEF + PARAMETER_92_START_ADDRESS, ON);
			}
			break;

			case key_refeigerant:
			{
				Send_modebus_command_06(PARA_F32_136 + PARAMETER_92_START_ADDRESS, ON);
			}
			break;

			case key_reset_para:
			{
				//					write_PIC_Set(READ_DARA_INTERFACE);
				//					Flag_Interface	=	interface;
				//					T_first_power_on_03_commnd	=	0;								//��ȡȫ������
				//					Flag_read_all_data = 1;
				Flag_Interface = 65; // ��ȡ�ɹ�������һҳ
				Send_modebus_command_06(PARA_RESET + PARAMETER_92_START_ADDRESS, ON);
				Clear_Fault_Bit = true;
			}
			break;

			case key_user_para:
			{
				Time_first_power_on = 10;
				Flag_Interface = 66;
				T_first_power_on_03_commnd = 0; // ��ȡȫ������
			}
			break;

			case key_time:
			{
				time_set_init_enter();
			}
			break;
			case key_timer_mute:
			{
				Exit_Timer_Interface();//跳转到定时界面
			}
			break;
			case key_exit_timer_holiday: // �˳���ʱ����ģʽ
			{
				if (write_eeprom_bit)
				{
					write_eeprom_bit = 0; // ���дeeprom��־λ
					EEPROM_Write_page();
					//T5L_Flash(WRITE, (EEPROM_VACATION - 1), (EEPROM_VACATION - 1) - MAGIC_NUMBER, 4);
				}
				Return_Home();
			}
			break;

			case key_wifi_connect:
			{
				//					wifi_work_state	=	0;
				//					mcu_set_wifi_mode(0);
				if (Para_pack[PARA_H06_99] != 0)
					T_Connect_4G_wifi_time = 300;
				if (Connect.at_cmd != CONNECT)
					Connect.at_cmd = Connect_wifi; // ��������
			}
			break;

			case key_get_wifi:
			{
				if (Para_pack[PARA_H06_99] != 0)
				{
					write_PIC_Set(56);
				}
				else
				{
					Display_TIP_ICO(6);
				}
			}
			break;

			case key_network_details:
			{
				if (Para_pack[PARA_H06_99] != 0)
				{
					Connect.at_cmd = WF_QWLMAC;
					SET_BIT(Flag_Connect_EN, 4);
					write_PIC_Set(57);
				}
				else
				{
					Display_TIP_ICO(6);
				}

				// Connect.at_cmd																=	WF_QMTCONN_READ;//Connect_wifi;			//��������
			}
			break;

			case key_clear_fault:
			{
				Clear_Fault_process();
			}
			break;

			case key_tset_beep:
			{
				write_Button(10); // ��������
			}
			break;

			case key_clear_kwh:
			{
				usElectricity = 0;
				ulNumberEl = 0;
				write_dgus_vp(DISP_KWH, (u8 *)&usElectricity, 1);
				write_dgus_vp(EEPROM_KWH, (u8 *)&ulNumberEl, 2);
				Display_Electricity_Stats();
				//T5L_Flash(WRITE, (EEPROM_KWH), (EEPROM_KWH)-MAGIC_NUMBER, 2);
				EEPROM_Write_page();
			}
			break;

			case key_exit_sop:
			{
				if (!GET_BIT(Read_dgus_buf[0], 4))
				{
					write_PIC_Set(2);
				}
				else
				{
					Return_Home();
				}
			}
			break;

			case key_exit_set:
			{
				if (write_eeprom_bit)
				{
					write_eeprom_bit = 0; // ���дeeprom��־λ
					//T5L_Flash(WRITE, (EEPROM_PARA), (EEPROM_PARA)-MAGIC_NUMBER, 2);
					EEPROM_Write_page();
				}
			}
			break;

			case key_eco:
			{
				read_dgus_vp(EEPROM_TIMER_MUTE, (uint8_t *)&VALUE, 1);
				if (VALUE & 0x01) // �жϵ�ǰ�Ƿ���ڹ���
				{
					VALUE = 0;
					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS, OFF);
				}
				else
				{
					VALUE = 1;
					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS, ECO);
				}
				write_eeprom_bit = true; // ��дeeprom����
				write_dgus_vp(EEPROM_TIMER_MUTE, (uint8_t *)&VALUE, 1);
			}
			break;

			case key_4G:
			{
				Para_pack[(PARA_H06_99 - PARAMETER_92_START_ADDRESS)] = 0;
				//
				T_Connect_4G_wifi_time = 0;
				send_modbus_4g = 1;
				SOP_MODE = 0;
			}
			break;

			case key_wifi:
			{
				send_modbus_4g = 0;
				Send_modebus_command_06(PARA_H06_99 + PARAMETER_92_START_ADDRESS, 1); // �л�WiFi�����·�������
				T_Connect_4G_wifi_time = 0;
			}
			break;
//			case key_brand:
//			{
//				if (GET_BIT(Read_VP(EEPROM_PARA), 3))
//				{
//					if(Read_VP(EEPROM_Brand) == ENERGY && GET_BIT(Read_dgus_buf[0],3) && !(GET_BIT(Read_dgus_buf[0],4)))
//					{
//						write_PIC_Set(117);
//					}
//					if (Read_VP(EEPROM_Brand) == NEUTRAL)
//					{
//						Wite_VP(0x4133, 1);
//					}
//					else if (Read_VP(EEPROM_Brand) == ENERGY)
//					{
//						Wite_VP(0x4133, 2);
//					}
//					else if (Read_VP(EEPROM_Brand) == XIAOYOU)
//					{
//						Wite_VP(0x4133, 4);
//					}
//				}

//			}
//			break;
//			case key_back:
//				if (Read_VP(PARA_MODE) == COOLING)
//				{
//					write_PIC_Set(5);
//				}
//				else
//				{
//					write_PIC_Set(4);
//				}
//				brand_message = Read_VP(EEPROM_Brand);
//				Wite_VP(DISP_POWER_ICO, brand_message);
//				Wite_VP(DISP_NET, brand_message);
//				break;
			case key_contact:
				#if defined (Brand_ENERGY)
				write_PIC_Set(21);
				#endif
				#if defined (Brand_XIAOYOU)
				write_PIC_Set(21);
				#endif
				#if defined (Brand_MSD)
				write_PIC_Set(21);
				#endif			
				break;
			case key_cleanCharge:
				Clear_Electricity_Stats();
				EEPROM_Write_page();
				Electricity_Display(0,0);
				break;
			case key_room_temp:
				if(Read_VP(0x50AE) == 1)
				{
					if(Read_VP(0x50AF) == 0)
					{
						write_PIC_Set(194);
					}
					else
					{
						write_PIC_Set(193);
					}
				}
				else
				{
					if(Read_VP(TEMPERATURE_CONTROL)==0)
					{
						write_PIC_Set(135);//跳转到135页 room temperature control confirmation interface
					}
					EEPROM_Write_page();
				}
			break;
			case key_room_temp0:
				if(Read_VP(0x50AE) == 1)
				{
					if (Read_VP(TEMPERATURE_CONTROL) == 1)
					{
						write_PIC_Set(192);
					}
					else
					{
						write_PIC_Set(135);
					}
				}
				else 
				{
					Wite_VP(TEMPERATURE_CONTROL,0);
					EEPROM_Write_page();				
				}
				EXIT_VACATION();
			break;
			case key_confirm_room_temp:
				if(Read_VP(TEMPERATURE_CONTROL) == 0)
				{
					Wite_VP(TEMPERATURE_CONTROL,1);
				}
				else
				{
					Wite_VP(TEMPERATURE_CONTROL,0);
				}
				Return_Home();//跳转到4页
				EEPROM_Write_page();
				EXIT_VACATION();
			break;
			case key_clear_curve:
			{
				uint8_t clearCurveCommand[16] = {0};//曲线0
				memset(clearCurveCommand, 0, sizeof(clearCurveCommand));
				write_dgus_vp(0x0300, (u8*)&clearCurveCommand, 8); // 写入清零曲线命令到地址 0x0300
			}
			break;
			case key_set_temp_add:
				EXIT_VACATION();
			if(Read_VP(TEMPERATURE_CONTROL)==0)//判断是否为室温控制
			{
				if (HostMode == COOLING)
				{
					key_set_temp = Para_pack[PARA_COOLING_TEMP];
					if(key_set_temp <20)
						key_set_temp += 1;
					else
						key_set_temp = 20;
					write_dgus_vp(DISP_SET_TEMP, (u8 *)&key_set_temp, 1);
					Send_modebus_command_06(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS, key_set_temp);
				}
				else
				{
					key_set_temp = Para_pack[PARA_HEATING_TEMP];
					if(key_set_temp <Para_pack[PARA_HEATING_MAX])
						key_set_temp += 1;
					else
						key_set_temp = Para_pack[PARA_HEATING_MAX];
					write_dgus_vp(DISP_SET_TEMP, (u8 *)&key_set_temp, 1);
					Send_modebus_command_06(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS, key_set_temp);
				}
			}
			else
			{
				key_set_temp = Read_VP(INDOOR_TEMP_SET);
				if(key_set_temp <32)
					key_set_temp += 1;
				else
					key_set_temp = 32;
				write_dgus_vp(INDOOR_TEMP_SET, (u8 *)&key_set_temp, 1);
				write_dgus_vp(DISP_SET_TEMP, (u8 *)&key_set_temp, 1);
				EEPROM_Write_page();
			}
			break;
			case key_set_temp_sub:
				EXIT_VACATION();
			if(Read_VP(TEMPERATURE_CONTROL)==0)//判断是否为室温控制
			{
				if (HostMode == COOLING)
				{
					key_set_temp = Para_pack[PARA_COOLING_TEMP];
					if(key_set_temp > Para_pack[PARA_COOLING_MIN])
						key_set_temp -= 1;
					else
						key_set_temp = Para_pack[PARA_COOLING_MIN];
					write_dgus_vp(DISP_SET_TEMP, (u8 *)&key_set_temp, 1);
					Send_modebus_command_06(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS, key_set_temp);
				}
				else
				{
					key_set_temp = Para_pack[PARA_HEATING_TEMP];
					if(key_set_temp > 20)
						key_set_temp -= 1;
					else
						key_set_temp = 20;
					write_dgus_vp(DISP_SET_TEMP, (u8 *)&key_set_temp, 1);
					Send_modebus_command_06(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS, key_set_temp);
				}
			}
			else
			{
				key_set_temp = Read_VP(INDOOR_TEMP_SET);
				if(key_set_temp > 16)
					key_set_temp -= 1;
				else
					key_set_temp = 16;
				write_dgus_vp(INDOOR_TEMP_SET, (u8 *)&key_set_temp, 1);
				write_dgus_vp(DISP_SET_TEMP, (u8 *)&key_set_temp, 1);
				EEPROM_Write_page();
			}
			break;
//			case key_re_Detection:
//				CLR_BIT(Read_dgus_buf[0],3);
//				write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
//				T5L_Flash(WRITE, EEPROM_PARA, EEPROM_PARA - MAGIC_NUMBER, 2);
//			break;
			case key_wifi_sop:
			{
				Connect.at_cmd = Connect_wifi_sop; // 发送连接命令
				SOP_MODE = 1; // Set SOP mode to test mode
			}
			break;
			
			case key_wifi_status_refresh:
			{
					Connect.at_cmd = WF_QGETWIFISTATE; // 获取wifi信号
			}
			break;
			
			case key_manual_get_location:		// 手动获取经纬度
			{
				if (State2_pack[STATE_LOCATION_STATUS] >= 2)
				{
					Send_modebus_command_06(STATE_GET_LOCATION + PARAMETER_250_START_ADDRESS, 1);
				}
			}
			break;
			
			case key_manual_update_net:		// 手动更新网络
			{
				if (T_Network_Offline > 300)
				{
					Send_modebus_command_06(STATE_4G_RESET + PARAMETER_250_START_ADDRESS, 2);
				}
			}
			break;
			
			case key_read_mode_holiday: //进入离家页面读取当前机型
			{
				write_PIC_Set(10);
				read_dgus_vp(0x507A, (u8 *)&VALUE, 1);
				read_dgus_vp(0x401C, (u8 *)&dgus_data, 1);
				read_dgus_vp(INDOOR_TEMP_SET, (u8 *)&dgus_data2, 1);
				if (VALUE == 1)
				{
					if (dgus_data)
						dgus_data = 1;
					else
						dgus_data = 0;
				}
				else
				{
					dgus_data = 0;
				}
				write_dgus_vp(0x401C, (u8 *)&dgus_data, 1);
				if(Read_VP(TEMPERATURE_CONTROL)==0)
				{
					if (dgus_data == 0)
					{
						write_dgus_vp(0x401D, (u8 *)&Para_pack[PARA_HEATING_TEMP], 1);
					}
					else
					{
						write_dgus_vp(0x401D, (u8 *)&Para_pack[PARA_COOLING_TEMP], 1);
					}
				}
				else
				{
					Wite_VP(0x401D,dgus_data2);
				}
			}
			break;
			case key_refresh_electricity:
				Display_Electricity_Curve();
				write_PIC_Set(198);
				break;
			case key_electricity_daily_interface:
				Display_Electricity_Curve();
				write_PIC_Set(201);
				break;
			case key_electricity_monthly_interface:
				Display_Electricity_Curve();
				write_PIC_Set(203);
				break;
			case key_electricity_yearly_interface:
				Display_Electricity_Curve();
				write_PIC_Set(206);
				break;
//			case key_Aging_sop:
//			{
//				Sleep_Mode = 0 ;
//			}
//			break;
			
			default:
				break;
			}
		}

		Touch_Event_Flag = 0;
		write_dgus_vp(TOUCH_EVENT_FLAG, (uint8_t *)&Touch_Event_Flag, 1);
	}
}

void EXIT_VACATION(void)
{
	uint16_t VALUE;
	read_dgus_vp(EEPROM_VACATION, (u8 *)&VALUE, 1);
	if (VALUE == 1)
	{
		VALUE = 0;
		write_dgus_vp(EEPROM_VACATION, (u8 *)&VALUE, 1);
		//T5L_Flash(WRITE, (EEPROM_VACATION - 1), (EEPROM_VACATION - 1) - MAGIC_NUMBER, 2);
		EEPROM_Write_page();
		T_holiday_TIME = 0;
		Flag_holiday_mode = 0;
	}
}
void Parameter_setting_process(void)
{
	uint8_t read_dgus_key_addr[4] = 0;
	int16_t dgus_addr, dgus_data,dgus_data2;
	int16_t modbus_addr, modbus_data;
	int16_t para_min, para_max;
	int16_t read_value[4];
	uint8_t Password_Buf[15];
	u32 password;
	uint32_t password_value;
	uint16_t VALUE;

	read_dgus_vp(TOUCH_AUTO_FLAG, (u8 *)read_dgus_key_addr, 2);

	if (read_dgus_key_addr[0] == 0x5A)
	{
		if (Read_dgus_buf[0] & 0x01) // �ж��Ƿ��йرհ�������
			write_Button(10);		 // ��������
		dgus_addr = read_dgus_key_addr[1] << 8 | read_dgus_key_addr[2];

		if (dgus_addr >= MAGIC_NUMBER && dgus_addr <= 0x4FFE)
		{
			if(dgus_addr >= MAGIC_NUMBER && (dgus_addr <= MAGIC_NUMBER+	EEPROM_LENGTH))
			{
				EEPROM_Write_page(); // 写入eeprom数据到flash
			}
			if (dgus_addr == 0x401D || dgus_addr == 0x4012 || dgus_addr == 0x4015 || dgus_addr == 0x4018)
			{
				read_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);
				if (dgus_addr == 0x401D)
				{
					read_dgus_vp(dgus_addr - 1, (u8 *)&modbus_data, 1);
				}
				else
				{
					if (Para_pack[PARA_MODE] == HEATING)
						modbus_data = 0;
					else
						modbus_data = 1;
				}

				if (Read_VP(TEMPERATURE_CONTROL)==0)
				{
					if (modbus_data == 0)
					{ // ����
						para_min = Para_init_DATA[PARA_HEATING_TEMP][0];
						para_max = Para_pack[PARA_HEATING_MAX];
					}
					else
					{ // ����
						para_min = Para_pack[PARA_COOLING_MIN];
						para_max = Para_init_DATA[PARA_COOLING_TEMP][1];
					}
				}
				else
				{
					para_min = 16;
					para_max = 32;
				}
				if ((dgus_data > para_max) || (dgus_data < para_min))
				{
					if (dgus_data > para_max)
						dgus_data = para_max;
					else
						dgus_data = para_min;
					Display_TIP_ICO(4);
					write_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);
				}

				if (dgus_addr == 0x401D)
				{
					read_dgus_vp(EEPROM_VACATION, (u8 *)&VALUE, 1);
					if (VALUE == 1)
					{
						if(Read_VP(TEMPERATURE_CONTROL)==0)
						{
							if (modbus_data == 0)
							{ // ����
								write_dgus_vp((PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS + TOUCH_EVENT_FLAG), (u8 *)&dgus_data, 1);
								Send_modebus_command_06(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS, dgus_data); // �ϴ����ƶ�
							}
							else
							{
								write_dgus_vp((PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS + TOUCH_EVENT_FLAG), (u8 *)&dgus_data, 1);
								Send_modebus_command_06(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS, dgus_data); // �ϴ����ƶ�
							}
						}
						else
						{
							Wite_VP(INDOOR_TEMP_SET,dgus_data);
						}
					}
				}
			}
			else if (dgus_addr == 0x401C)
			{
				read_dgus_vp(0x507A, (u8 *)&read_value[0], 1);
				read_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);
				read_dgus_vp(INDOOR_TEMP_SET, (u8 *)&dgus_data2, 1);
				if (read_value[0] == 1)
				{
					if (dgus_data)
						dgus_data = 1;
					else
						dgus_data = 0;
				}
				else
				{
					dgus_data = 0;
				}
				write_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);

				if(Read_VP(TEMPERATURE_CONTROL)==0)
				{
					if (dgus_data == 0)
					{
						write_dgus_vp((dgus_addr + 1), (u8 *)&Para_pack[PARA_HEATING_TEMP], 1);
					}
					else
					{
						write_dgus_vp((dgus_addr + 1), (u8 *)&Para_pack[PARA_COOLING_TEMP], 1);
					}
				}
				else
				{
					Wite_VP((dgus_addr + 1),dgus_data2);
				}
	
				read_dgus_vp(EEPROM_VACATION, (u8 *)&VALUE, 1);
				if (VALUE == 1)
				{
					if (dgus_data == 0)
						Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);
					else
						Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);
				}
			}
			else if (dgus_addr == 0x401B)
			{
				read_dgus_vp(dgus_addr, (u8 *)&read_value[0], 3);
				if (read_value[0] == 1)
				{ // ����
					if (read_value[2] > 0 && read_value[2] < 77)
					{
						Flag_holiday_mode = 1;
						if(Read_VP(TEMPERATURE_CONTROL)==0)
						{
							if (read_value[1] == 0)
							{
								Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);	
								Send_modebus_command_06(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS, read_value[2]);
							}
							else
							{
								Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);	
								Send_modebus_command_06(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS, read_value[2]);							
							}
						}
						else
						{
							if (read_value[1] == 0)
							{
								Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);	
							}
							else
							{
								Send_modebus_command_06(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);						
							}
							Wite_VP(INDOOR_TEMP_SET,read_value[2]);
						}							
					}
					else
					{
						Display_TIP_ICO(4);
						read_value[0] = 0;
						read_dgus_vp(dgus_addr, (u8 *)&read_value[0], 1);
						T_holiday_TIME = 0;
						Flag_holiday_mode = 0;
					}
				}
				else
				{
					T_holiday_TIME = 0;
					Flag_holiday_mode = 0;
				}
			}		
			else if (dgus_addr == 0x4001)
			{
				read_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);
				dgus_data &= 0x7E;
				//				if(dgus_data	&	0x01)
				//				{
				//					dgus_data	=	1;
				//					Send_modebus_command_06(PARA_MUTE + PARAMETER_92_START_ADDRESS,ECO);
				//				}
				write_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);
			}
			else if (dgus_addr == 0x4132)
			{
				if (Read_VP(0x4132) == NEUTRAL)
				{
					Wite_VP(0x4133, 1);
				}
				else if (Read_VP(0x4132) == ENERGY)
				{
					Wite_VP(0x4133, 2);
				}
				else if (Read_VP(0x4132) == XIAOYOU)
				{
					Wite_VP(0x4133, 4);
				}
				//T5L_Flash(WRITE, 0x4132, 306, 2);
				EEPROM_Write_page();
			}
			
		}
		else
		{
			read_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);
			if ((dgus_addr >= (TOUCH_EVENT_FLAG + PARAMETER_92_START_ADDRESS) && dgus_addr <= (TOUCH_EVENT_FLAG + PARAMETER_200_START_ADDRESS))) //	||	dgus_addr	==	SET_POWER_A_TEMP)
			{

				modbus_addr = (dgus_addr - TOUCH_EVENT_FLAG) - PARAMETER_92_START_ADDRESS;

				modbus_data = dgus_data;

				if (modbus_addr == PARA_COOLING_TEMP)
				{
					para_min = Para_pack[PARA_COOLING_MIN];
					para_max = Para_init_DATA[modbus_addr][1];
				}
				else if (modbus_addr == PARA_HEATING_TEMP)
				{
					para_min = Para_init_DATA[modbus_addr][0];
					para_max = Para_pack[PARA_HEATING_MAX];
				}
				else if (modbus_addr == PARA_WATER_TEMP)
				{
					para_min = Para_init_DATA[modbus_addr][0];
					para_max = Para_pack[PARA_WATER_MAX];
				}
				else
				{ // ����Ŀ���¶�
					para_min = Para_init_DATA[modbus_addr][0];
					para_max = Para_init_DATA[modbus_addr][1];
				}

				if ((modbus_data > para_max) || (modbus_data < para_min))
				{
					if (modbus_data > para_max)
						modbus_data = para_max;
					else
						modbus_data = para_min;

					Display_TIP_ICO(4);
				}
				dgus_data = modbus_data;
				write_dgus_vp(dgus_addr, (u8 *)&dgus_data, 1);

				if (modbus_addr == PARA_H06_99)
				{ // �л�������ʽ��Ҫ���������߼�
					if (modbus_data == 0)
					{ // �л�4Gʱ��WiFi����2Min�����·��л�ָ���������
						VALUE = key_4G;
					}
					else
					{ // �л�WiFiʱ�������·��������أ�WiFi�ȴ�2Min���ٴ�ʹ��
						VALUE = key_wifi;
					}
					write_dgus_vp(TOUCH_EVENT_FLAG, (uint8_t *)&VALUE, 1);
				}
				else
				{
					Send_modebus_command_06(modbus_addr + PARAMETER_92_START_ADDRESS, modbus_data); // �ϴ����ƶ�
				}

				if (modbus_addr == PARA_COOLING_TEMP || modbus_addr == PARA_HEATING_TEMP)
				{
					EXIT_VACATION();
				}
				if (modbus_addr == PARA_MUTE_ENABLE)
				{
					//清零定时静音
					Wite_VP(TIMER_MUTE_ENABLE,0);
					EEPROM_Write_page();
				}

			    if (Type_Interface == 71 || Type_Interface == 72|| Type_Interface == 175 || Type_Interface == 176)
				{
					write_PIC_Set(66);
				}
				else if (Type_Interface == 177 || Type_Interface == 178)
				{
					write_PIC_Set(67);
				}
				else if (Type_Interface == 179 || Type_Interface == 180)
				{
					write_PIC_Set(68);
				}
				else if (Type_Interface == 181 || Type_Interface == 182 || Type_Interface == 183 || Type_Interface == 184)
				{
					write_PIC_Set(69);
				}
				else if (Type_Interface >= 89 && Type_Interface <= 90)
				{
					write_PIC_Set(77);
				}
				else if (Type_Interface == 96)
				{
					write_PIC_Set(81);
				}
				else if (Type_Interface >= 91 && Type_Interface <= 95)
				{
					write_PIC_Set(80);
				}
				else if (Type_Interface >= 97 && Type_Interface <= 98)
				{
					write_PIC_Set(82);
				}
				else if (Type_Interface == 99)
				{
					write_PIC_Set(83);
				}
				else if (Type_Interface == 100)
				{
					write_PIC_Set(85);
				}
				else if (Type_Interface == 101)
				{
					write_PIC_Set(87);
				}
				else if (Type_Interface == 115)
				{
					write_PIC_Set(76);
				}
				else if (Type_Interface == 170 || Type_Interface == 171 || Type_Interface == 172 )
				{
					write_PIC_Set(160);
				}
				else if (Type_Interface == 173)
				{
					write_PIC_Set(78);
				}
				else if (Type_Interface == 174)
				{
					write_PIC_Set(85);
				}
				else if (Type_Interface == 192 || Type_Interface == 193 || Type_Interface == 194)
				{
					Return_Home();
				}
			}
			else
			{
				if (dgus_addr == KEY_PASSWORD_1)
				{												 // ��������
					read_dgus_vp(dgus_addr, (u8 *)&password, 2); // ��ȡ��������ֵ
					password_value = PasswordCalculation();
					if (password != 0 && password == password_value) // 密码正确已解密
					{
						SET_BIT(Read_dgus_buf[0], 4);
						write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
						Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 1);
						State2_pack[STATE_FLAG_UNLOCK] = 1;
						//T5L_Flash(WRITE, EEPROM_PARA, EEPROM_PARA - MAGIC_NUMBER, 2);
						Return_Home();
						Clear_Fault_Bit = true; // Enter the power-on password to clear historical faults
						// 输入开机密码成功：清除全部电量统计
						Clear_Electricity_Stats();
						EEPROM_Write_page();
						Electricity_Display(0, 0);
					}
					else if (!GET_BIT(Read_dgus_buf[0], 3) && password == 610000) // �����̼����벢���̼�������Ч
					{
						SET_BIT(Read_dgus_buf[0], 3);
						write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
						Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 3);
						State2_pack[STATE_FLAG_UNLOCK] = 3;
						T_TSET_TIME_Min = 59;
						//T5L_Flash(WRITE, EEPROM_PARA, EEPROM_PARA - MAGIC_NUMBER, 2);
						EEPROM_Write_page();
						Return_Home();
					}
					else
					{
						memset(Password_Buf, 0x00, sizeof(Password_Buf));
						Password_Buf[11] = system.month / 10;
						Password_Buf[12] = system.month % 10;
						Password_Buf[13] = system.date / 10;
						Password_Buf[14] = system.date % 10;
						password_value = L610_PasswordCalculation(&Password_Buf[0]); // ���㿪������
						if (password == password_value && password != 0)
						{
							Display_TIP_ICO(7);
							CLR_BIT(Read_dgus_buf[0], 3); // �����̼�����
							write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
							//T5L_Flash(WRITE, EEPROM_PARA, EEPROM_PARA - MAGIC_NUMBER, 2);
							//EEPROM_Write_page();;
						}
						else if (password == 368969)
						{
							Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 3);
							State2_pack[STATE_FLAG_UNLOCK] = 3;
							write_PIC_Set(105);
						}
						else
						{
							Display_TIP_ICO(3);
						}
					}
					password = 0;
					write_dgus_vp(dgus_addr, (u8 *)&password, 2);
				}
				else if (dgus_addr == KEY_PASSWORD_2)
				{ // �߼�����/SOP
					advan_password_confirm(dgus_addr, 369, 190, READ_PARA);
					advan_password_confirm(dgus_addr, 368969, 105, NO_READ_PARA);
					advan_password_confirm(dgus_addr, 589, 76, READ_PARA);
				}
				else if (dgus_addr == KEY_PASSWORD_3)
				{ // �ͻ���������
					advan_password_confirm(dgus_addr, 589, 24, READ_PARA);
				}
				else if (dgus_addr == KEY_PASSWORD_4)
				{ // ��ʷ����
					advan_password_confirm(dgus_addr, 589, 43, NO_READ_PARA);
				}
				else if (dgus_addr == KEY_PASSWORD_5)
				{ // �ָ���������
					advan_password_confirm(dgus_addr, 400866, 73, NO_READ_PARA);
				}
				else if(dgus_addr == ELECTRICITY_PAGE)
				{
					electricityPage = Read_VP(ELECTRICITY_PAGE);
					if (electricityPage >=1 && electricityPage <=4)
					{
						Electricity_Display(0,electricityPage-1);
					}
					else if (electricityPage >= 5 && electricityPage<=9)
					{
						Electricity_Display(1,electricityPage-5);
					}
					else if(electricityPage >= 10 && electricityPage<=11)
					{
						Electricity_Display(2,electricityPage-10);
					}
				}
				else if (dgus_addr == DISP_ELEC_ENABLE)
				{
					memset(read_dgus_key_addr, 0, 2);
					write_dgus_vp(TOUCH_AUTO_FLAG, (u8 *)read_dgus_key_addr, 1);
					Set_Elec_Hide_Select((u16)dgus_data);
				}
			}
		}
		memset(read_dgus_key_addr, 0, 2);
		write_dgus_vp(TOUCH_AUTO_FLAG, (u8 *)read_dgus_key_addr, 1);
	}
}

void Interface_running_process(void)
{
	static uint16_t Type_Interface_value = 0;
	static int16_t time = 0;
	Type_Interface = read_PIC_Now();
	switch (Type_Interface)
	{
	case POWER_ON_INTERFACE:
	{ // ��������20S�����������
		if (Time_first_power_on == 0)
		{
			if (Enable_Host_Unlock == 1)		// 新机制版本，由主板解密标志位决定
			{
				if (State2_pack[STATE_FLAG_UNLOCK] == 1 || State2_pack[STATE_FLAG_UNLOCK] == 3)		// 已解密
				{
					write_PIC_Set(SYSTEM_CONNECT); // 跳转到使用指引
					Time_first_power_on = 10;
					if (!GET_BIT(Read_dgus_buf[0], 4))
					{
						SET_BIT(Read_dgus_buf[0], 4);
						write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
						EEPROM_Write_page();
					}
				}
				else if (State2_pack[STATE_FLAG_UNLOCK] == 2)		// 主板存储异常
				{
//					Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 1);
					if (GET_BIT(Read_dgus_buf[0], 4))
					{
						write_PIC_Set(SYSTEM_CONNECT); 
						Time_first_power_on = 10;
					}
					else
					{
						write_PIC_Set(SYSTEM_PASSWORD);
						Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF);
					}
				}
				else		// 未解密
				{
					write_PIC_Set(SYSTEM_PASSWORD); // 跳转到开机密码界面
					Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF);
					Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 0);
					if (GET_BIT(Read_dgus_buf[0], 4))
					{
						CLR_BIT(Read_dgus_buf[0], 4);
						write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
						EEPROM_Write_page();
					}
				}
			}
			else
			{
				if (GET_BIT(Read_dgus_buf[0], 4))
				{
					write_PIC_Set(SYSTEM_CONNECT); // ����ָ��
					Time_first_power_on = 10;
				}
				else
				{
					write_PIC_Set(SYSTEM_PASSWORD); // ���뿪���������
					Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF);
				}
			}
			
			if (!GET_BIT(Flag_Connect_EN, 2))
			{
				Connect.at_cmd = WF_QWLMAC;
				SET_BIT(Flag_Connect_EN, 4);
			}
		}
	}
	break;

	case SYSTEM_CONNECT:
	{
		if (Time_first_power_on == 0)
		{
			Return_Home();
		}
		else if (time != Time_first_power_on)
		{
			time = Time_first_power_on;
			write_dgus_vp(DISP_TIME_TIMER, (u8 *)&time, 1);
		}
	}
	break;

	case SYSTEM_PASSWORD:
	{
		if(GET_BIT(Read_dgus_buf[0], 4))
		{
			write_PIC_Set(106);
		}
	}
	break;
	
	case SYATEM_LOCK_INTERFACE: // ����������
	{
		Type_Interface_value = POWER_HOME_INTERFACE;
		if (Para_pack[PARA_LOCK] == OFF)
		{
			Return_Home();
		}
	}
	break;

	case POWER_HOME_INTERFACE: // ����������
	{
		HostModeProcessing();
		if (Para_pack[PARA_LOCK] == ON)
		{
			write_PIC_Set(3);
		}
		else if(Read_dgus_buf[0] & 0x100)
		{
			write_PIC_Set(108);
		}
		else if(HostMode == COOLING)
		{
			write_PIC_Set(MODE_COOLING_INTERFACE);
		}
	}
	break;

	case MODE_COOLING_INTERFACE:
	{
		HostModeProcessing();
		if (Para_pack[PARA_LOCK] == ON)
		{
			write_PIC_Set(3);
		}
		else if(HostMode == HEATING)
		{
			write_PIC_Set(POWER_HOME_INTERFACE);
		}
	}
	break;

	case READ_DARA_INTERFACE:
	{
		if (Time_first_power_on == 0)
		{
			write_PIC_Set(13);
			Flag_Interface = 0;
			Display_TIP_ICO(5);
		}
	}
	break;

	case RESET_DATA_INTERFACE:
	{
		if(GET_BIT(Read_dgus_buf[0],4)==false)
		{
			write_PIC_Set(2);
			Flag_Interface = 0;
		}
		else if (Time_first_power_on == 0)
		{
			Flag_Interface = 0;
			write_PIC_Set(65);
			Display_TIP_ICO(5);
		}
	}
	break;

	case SYSTEM_SOP_INTERFACE:
	{
		write_Painting(0x0004);
	}
	break;

	default:
	{
		Time_first_power_on = 10;
		Painting_cmd_byte = 0; // �������Ը������
		Type_Interface_value = Type_Interface;
	}
	break;
	}
}

void Return_Home(void)
{
	HostModeProcessing();
	if (Para_pack[PARA_LOCK] == ON)
	{
		write_PIC_Set(3);
	}
	if(Read_dgus_buf[0] & 0x100)
	{
		write_PIC_Set(108);
	}
	else
	{
		if(HostMode == COOLING)
		{
			write_PIC_Set(5);
		}
		else
		{
			write_PIC_Set(4);
		}
	}
}

void Exit_Timer_Interface(void)
{
	if (Read_VP(0x50AE) == 1)
	{
		write_PIC_Set(17);
	}
	else
	{
		write_PIC_Set(195);
	}
}

void HostModeProcessing(void)
{
	 if(Para_pack[PARA_F19_123]==0)
	{
		if (Para_pack[PARA_MODE] == COOLING)
		{
			HostMode = COOLING;
		}
		else
		{
			HostMode = HEATING;
		}
	}
	else
	{
		if(State_pack[STATE_SWITCH] & 0X80)	
		{
			HostMode = COOLING;
		}	
		else
		{
			HostMode = HEATING;
		}
	}	
}

void HostPowerProcessing(void)
{
	if(Para_pack[PARA_F5_122]==0)
	{
		if (Para_pack[PARA_POWER_ON] == ON)
		{
			HostPower = 2;
		}
		else
		{
			HostPower = 1;
		}
	}
	else if(Para_pack[PARA_F5_122]==1)
	{
		if(State_pack[STATE_SWITCH] & 0X01)	
		{
			HostPower = 0;
		}	
		else
		{
			HostPower = 2;
		}
	}
	else if(Para_pack[PARA_F5_122]==2)
	{
		if((State_pack[STATE_SWITCH] & 0X01)==0)	
		{
			if (Para_pack[PARA_POWER_ON] == ON)
			{
				HostPower = 2;
			}
			else
			{
				HostPower = 1;
			}
		}	
		else
		{
			HostPower = 0;
		}
	}
}
/*****************************************************************************
 �� �� ��  : void Sleep_Handler(void)
 ��������  : ���ߴ�������
 �������  :
 �������  :
 �޸���ʷ  :
  1.��    ��   :2021��6��22��
	��    ��   : qiuxiaobin
	�޸�����   : �޸�
*****************************************************************************/
void Sleep_Handler(void)
{
	static uint8_t TP_Status_Old[8] = {0};
	uint8_t TP_Status_New[8];
	uint16_t Value = 0;
	

	read_dgus_vp(TP_STATUS, TP_Status_New, 4);
	Value = strcmp(TP_Status_New, TP_Status_Old);
	
	if (Value == 0)
	{
		switch (Flag_Sleep)
		{
			case 0:
			{
				if (Sleep_Count_M > 0)
				{
					if ((Read_dgus_buf[0] & 0x02) && (Read_dgus_buf[0] & 0x10)) // ����״̬���޷�������������//BIT0:����������BIT1:����������BIT2�澯����,BIT3�Ƿ�ʹ�ù��̼����룬BIT4�Ƿ��Ѿ�������
					{
						write_PIC_Set(11);
					}
					else
					{
						write_LED_Config(20, 0);
					}
					Flag_Sleep = 1;
				}
			}
		break;

		case 1:
		{
			if (Sleep_Count_M > 4)
			{
				write_LED_Config(20, 0);
				if ((Read_dgus_buf[0] & 0x02) && (Read_dgus_buf[0] & 0x10)) // ����״̬���޷�������������//BIT0:����������BIT1:����������BIT2�澯����,BIT3�Ƿ�ʹ�ù��̼����룬BIT4�Ƿ��Ѿ�������
				{
					Flag_Sleep = 2;
				}
				else
				{
					Flag_Sleep = 3;
				}
			}
		}
		break;

		case 2:
		{
			if (Sleep_Count_M > 6)
			{
				Flag_Sleep = 3;
			}
		}
		break;

		case 3:
		{
			write_LED_Config(0, 0);
			Flag_Sleep = 4;
		}
		break;

		case 4:
			break;
		
		default:
		{
			Flag_Sleep = 0;
		}
		break;
		}
	}
	else
	{
		Sleep_Count = 0;
		Sleep_Count_M = 0;
		if (Flag_Sleep != 0)
		{
			write_LED_Config(80, 0x64);
			if (Flag_Sleep == 4)
			{
				if ((Read_dgus_buf[0] & 0x10) && (!(Read_dgus_buf[0] & 0x02))) // ����
				{
					Return_Home();
				}
			}
			Flag_Sleep = 0;
		}
		memcpy(TP_Status_Old, TP_Status_New, sizeof(TP_Status_New));
	}
}

void advan_password_confirm(u16 Addr, u32 passwordByte, u8 interface, u8 Read_Byte)
{

	u32 password;
	read_dgus_vp(Addr, (u8 *)&password, 2);
	if (password == passwordByte && password != 0)
	{
		if (Read_Byte == 0x5A)
		{
			write_PIC_Set(READ_DARA_INTERFACE);
			Time_first_power_on = 10;
			Flag_Interface = interface;
			T_first_power_on_03_commnd = 0; // ��ȡȫ������
		}
		else
		{
			write_PIC_Set(interface);
		}
		Display_TIP_ICO(0);

		password = 0;
		write_dgus_vp(Addr, (u8 *)&password, 2);
	}
	else
	{
		if(passwordByte!=368969)
		{
			Display_TIP_ICO(3);
		}
	}
}

/******************************************
 �� �� ��  : u32 L610_PasswordCalculation(uint8_t	*IMEI_CODE)
 ��������  : ���㿪������
 �������  :	��
 �������  :
 �޸���ʷ  :
******************************************/
u32 PasswordCalculation(void)
{

	u32 ulPassword = 0;
	u16 usFactorA, usFactorB, usFactorC, usFactorD;
	u16 usNumA, usNumB, usNumC, i;
	uint8_t IMEI_CODE[16];
	read_dgus_vp(MODBUS_IMEI, (u8 *)IMEI_CODE, 8);
	for (i = 1; i < 15; i++)
	{
		IMEI_CODE[i] = IMEI_CODE[i] - 0x30;
	}
	usFactorA = IMEI_CODE[5] * 100 + IMEI_CODE[6] * 10 + IMEI_CODE[7];
	usFactorB = IMEI_CODE[8] * 100 + IMEI_CODE[9] * 10 + IMEI_CODE[10];
	usFactorC = IMEI_CODE[11] * 100 + IMEI_CODE[12] * 10 + IMEI_CODE[13];
	usFactorD = IMEI_CODE[14];
	usNumA = IMEI_CODE[5] + IMEI_CODE[6] + IMEI_CODE[7];
	usNumB = IMEI_CODE[8] + IMEI_CODE[9] + IMEI_CODE[10];
	usNumC = IMEI_CODE[11] + IMEI_CODE[12] + IMEI_CODE[13];

	ulPassword = ((u32)usFactorA * 0xAA) + ((u32)usFactorB * 0x55) + ((u32)usFactorC * 0xA5) + ((u32)(usFactorA + usFactorB + usFactorC) * 0x5A) + ((u32)(27 - usNumA) * 10000) + ((u32)(27 - usNumB) * 1000) + ((u32)(27 - usNumC) * 100) + (u32)usFactorD;
	return ulPassword;
}

/******************************************
 �� �� ��  : u32 L610_PasswordCalculation(uint8_t	*IMEI_CODE)
 ��������  : ���㿪������
 �������  :	��
 �������  :
 �޸���ʷ  :
******************************************/
u32 L610_PasswordCalculation(uint8_t *IMEI_CODE)
{ // 86551       505 894 952 1

	//	u8  ucIMEI_Buf[15]={8,6,4,9,0,8,0,5,7,5,4,7,7,5,3};//IMEI =864908057547753;ulPassword = 690678
	u32 ulPassword = 0;
	u16 usFactorA, usFactorB, usFactorC, usFactorD;
	u16 usNumA, usNumB, usNumC; //,i;
	// read_dgus_vp(0x221E,(u8*)ucIMEI_Buf,15);

	usFactorA = IMEI_CODE[5] * 100 + IMEI_CODE[6] * 10 + IMEI_CODE[7];
	usFactorB = IMEI_CODE[8] * 100 + IMEI_CODE[9] * 10 + IMEI_CODE[10];
	usFactorC = IMEI_CODE[11] * 100 + IMEI_CODE[12] * 10 + IMEI_CODE[13];
	usFactorD = IMEI_CODE[14];
	usNumA = IMEI_CODE[5] + IMEI_CODE[6] + IMEI_CODE[7];
	usNumB = IMEI_CODE[8] + IMEI_CODE[9] + IMEI_CODE[10];
	usNumC = IMEI_CODE[11] + IMEI_CODE[12] + IMEI_CODE[13];

	ulPassword = ((u32)usFactorA * 0xAA) + ((u32)usFactorB * 0x55) + ((u32)usFactorC * 0xA5) + ((u32)(usFactorA + usFactorB + usFactorC) * 0x5A) + ((u32)(27 - usNumA) * 10000) + ((u32)(27 - usNumB) * 1000) + ((u32)(27 - usNumC) * 100) + (u32)usFactorD;
	return ulPassword;
}

void EEPROM_Read_page(void)
{
	int16_t read_eeprom[EEPROM_LENGTH];
	uint16_t ee_crc = 0;
	T5L_Flash(READ, MAGIC_NUMBER, 0X0000, EEPROM_LENGTH);
	read_dgus_vp(MAGIC_NUMBER, (u8 *)&read_eeprom, EEPROM_LENGTH);
	ee_crc = crc16((uint8_t *)read_eeprom, EEPROM_LENGTH - 1);
	if ((read_eeprom[EEPROM_LENGTH-1] != ee_crc) || (read_eeprom[384]!= XKQ_VERSION_ADRESS))
	{
		EEPROM_Reset_page();
	}
	read_dgus_vp(EEPROM_KWH, (u8 *)&ulNumberEl, 2);
}

void EEPROM_Write_page(void)
{
	int16_t write_eeprom[EEPROM_LENGTH];
	uint16_t ee_crc = 0;
	read_dgus_vp(MAGIC_NUMBER, (u8 *)&write_eeprom, EEPROM_LENGTH - 1);
	ee_crc = crc16((uint8_t *)write_eeprom, EEPROM_LENGTH - 1);

	write_eeprom[EEPROM_LENGTH - 1] = ee_crc;
	write_dgus_vp(MAGIC_NUMBER, (u8 *)write_eeprom, EEPROM_LENGTH);
	T5L_Flash(WRITE, MAGIC_NUMBER, 0X0000, EEPROM_LENGTH);
}

void EEPROM_Reset_page(void)
{
	int16_t write_eeprom[EEPROM_LENGTH];
	uint16_t ee_crc = 0;
	memset(write_eeprom, 0, sizeof(write_eeprom));
	write_eeprom[0] = 0x83;	
	write_eeprom[29] = 30;	
	write_eeprom[305] = 300; 
	write_eeprom[306] = ENERGY;
	write_eeprom[378] = 0;
	write_eeprom[379] = 2;
	write_eeprom[380] = 25;
	write_eeprom[381] = 0;
	write_eeprom[384] = XKQ_VERSION_ADRESS;
	ee_crc = crc16((uint8_t *)write_eeprom, EEPROM_LENGTH - 1);
	write_eeprom[EEPROM_LENGTH - 1] = ee_crc;
	write_dgus_vp(MAGIC_NUMBER, (u8 *)write_eeprom, EEPROM_LENGTH);
	T5L_Flash(WRITE, MAGIC_NUMBER, 0X0000, EEPROM_LENGTH);
}

int16_t EXCHANGE_C_F(int16_t temp_CV, int16_t type_CV)
{
	int16_t temp, temp1;
	temp1 = temp_CV;
	if (type_CV == C_MODE)
	{
		temp = temp1;
	}
	else
	{
		temp = ((temp1 * 0.18) + 32) * 10;
	}
	return temp;
}

int16_t EXCHANGE_F_C(int16_t temp_CV, int16_t type_CV)
{
	int16_t temp, temp1;
	temp1 = temp_CV;
	if (type_CV == C_MODE)
	{
		temp = temp1;
	}
	else
	{
		temp = (temp1 - 320) / 1.8;
	}
	return temp;
}