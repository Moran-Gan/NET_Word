#include "Dwin_CONF.h"
#include "user_include.h"
uint8_t T_timer_3S = 0;
uint16_t Ttimer_monitor_time = 0;
/****************************************************************************************/
// Function.Name:    Task_10ms_process()
// Function.Content: 10ms Task process
// Function.Editor:  cwx
// Function.Time:    2021/04/30
/****************************************************************************************/
void Task_10ms_process(void)
{
	switch (T_timer_10ms)
	{
	case 1:
		if (T_timer_100ms < 9) // 100ms
		{
			T_timer_100ms++;
			Task_100ms_process(); // 100ms process
		}
		else
		{
			T_timer_100ms = 0;
		}

		break;
	case 2:
		if (T_timer_500ms < 49) // 500ms
		{
			T_timer_500ms++;
			Task_500ms_process(); // 500ms peocess
		}
		else
		{
			T_timer_500ms = 0;
		}
		Interface_running_process();
		break;
	case 3:
		Parameter_setting_process();
		WDT_RST();
		break;
	case 4:
		Parm_Set_Function();
		break;

	default:
		break;
	}
}
/****************************************************************************************/
// Function.Name:    Task_100ms_process()
// Function.Content: 100ms Task process
// Function.Editor:  cwx
// Function.Time:    2021/04/30
/****************************************************************************************/
void Task_100ms_process(void)
{
	switch (T_timer_100ms)
	{
	case 1:
		if (T_timer_1S < 9)
		{
			T_timer_1S++;
			Task_1S_process();
		}
		else
		{
			T_timer_1S = 0;
		}

		if (T_Connect_send_time < 600)
		{
			T_Connect_send_time++;
		}
		break;
	case 2:
		Sleep_Handler();
		break;
	case 3:
		Connect_Reset_Protocol();
		break;
	case 4:
		if(write_flash_bit)
		{
			write_flash_bit = false;
			EEPROM_Write_page();
		}
		break;
	default:
		break;
	}
}

/****************************************************************************************/
// Function.Name:    Task_500ms_process()
// Function.Content: 500ms Task process
// Function.Editor:  cwx
// Function.Time:    2021/04/30
/****************************************************************************************/
void Task_500ms_process(void)
{
	switch (T_timer_500ms)
	{
	case 1:
		Master_command_handle_process();
		if (flag_communication_fault)
		{
			T_first_power_on_03_commnd = 0;

			Flag_function_06_command = 0;
			Flag_function_10_command = 0;
		}
		break;
	case 2:
		if (UART_MODBUS_OK) //(T_first_power_on_03_commnd	>	2	)//&&	!(Para_pack[PARA_Manual] & 0X30))				//���õ�ůԤ�Ȼ��ߵ�ַ���ʱ���ж�ʱ��Ч
		{					// ͨѶ��������ִ�ж�ʱ����
			TIME_ICO = 0;
			Timer_run_process();
		}
		break;
	case 3:
		Icon_driver_process(); // ��ʾͼ��
		break;
	case 4:
		HostModeProcessing();
		HostPowerProcessing();
		break;
	case 5:
		read_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
		read_dgus_vp(EEPROM_TEMP, (u8 *)&Read_dgus_buf[1], 1);
		break;
	default:
		break;
	}
}

/****************************************************************************************/
// Function.Name:    Task_1S_process()
// Function.Content: 1S Task process
// Function.Editor:  cwx
// Function.Time:    2021/04/30
/****************************************************************************************/
void Task_1S_process(void)
{
	switch (T_timer_1S)
	{
	case 1:
	{
		if (Time_communication_30S)
		{
			Time_communication_30S--;
			flag_communication_fault = 0;
		}
		else
		{
			flag_communication_fault = 1;
		}

		if (Time_first_power_on > 0)
		{
			Time_first_power_on--;
		}
		if (Sleep_Count < 59)
		{
			Sleep_Count++;
		}
		else
		{
			Sleep_Count = 0;
			if (Sleep_Count_M < 60)
				Sleep_Count_M++;
		}
		if (T_timer_3S > 2)
		{
			T_timer_3S = 0;
			//				wifi_all_data_update();
		}
		else
		{
			T_timer_3S++;
		}

		if (T_BEEP_TIME < 30)
		{
			T_BEEP_TIME++;
		}
		
		if(Flag_holiday_mode == 1)
		{
			if (T_holiday_TIME < 10)
			{
				T_holiday_TIME++;
			}
			if(T_holiday_TIME == 3)
			{
				if(Read_VP(TEMPERATURE_CONTROL)==0)
				{
					Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, ON);
				}
				else
				{
					Wite_VP(INDOOR_POWER,ON);
				}
			}
		}
		
		if (Para_pack[PARA_H06_99] == 0)	// WIFI模式下不下发
		{
			if (rdIndoor_Temp_TIME < 500)
			{
				rdIndoor_Temp_TIME++;
			}
			if(rdIndoor_Temp_TIME > 120)
			{
				Send_modebus_command_06(STATE_TEMP_INDOOR + PARAMETER_20_START_ADDRESS, Read_VP(TEMP_REAL));
				Send_modebus_command_06(STATE_TEMP_DHT83 + PARAMETER_20_START_ADDRESS, Read_VP(DHT83_REAL));
				rdIndoor_Temp_TIME = 0;
			}
		}
	
		if (T_Connect_4G_wifi_time < 300)
		{
			T_Connect_4G_wifi_time++;
		}
		else if (send_modbus_4g == 1)
		{
			send_modbus_4g = 0;
			Send_modebus_command_06(PARA_H06_99 + PARAMETER_92_START_ADDRESS, 0); // �л�WiFi�����·�������
		}

		if (T_Connect_Send_State_time < 9999) // Send_State_Time)
		{
			T_Connect_Send_State_time++;
		}
		if (T_Connect_Send_Advance_time < 9999) // Send_Advance_Time)
		{
			T_Connect_Send_Advance_time++;
		}
		if (T_Tip_time > 0)
		{
			T_Tip_time--;
		}
		else
		{
			Flag_Tip = 0;
		}
		
		if(T_Network_Offline < 5000)
		{
			T_Network_Offline++;
			if (T_Network_Offline > 300)
				SET_BIT(TIPS_ICON, 0);
			else
				CLR_BIT(TIPS_ICON, 0);
		}
		write_dgus_vp(DISP_PWD_TIPS, (u8*)&TIPS_ICON, 1);

		if(T_Detection_time<5000)
		{
			T_Detection_time++;
		}
		if (!GET_BIT(Read_dgus_buf[0], 4) && Type_Interface != SYSTEM_PASSWORD && Type_Interface != 104)
		{ // �̼�ģʽǿ���˳�
			if (T_TSET_TIME_S < 59)
			{
				T_TSET_TIME_S++;
			}
			else
			{
				T_TSET_TIME_S = 0;
				if (T_TSET_TIME_Min == 0)
				{
					Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF); // �·��ػ�ָ��
					write_PIC_Set(SYSTEM_PASSWORD);
				}
				else
				{
					T_TSET_TIME_Min--;
				}
			}
		}
//		if(Read_VP(EEPROM_Brand) == ENERGY && GET_BIT(Read_dgus_buf[0],3) && !(GET_BIT(Read_dgus_buf[0],4)))
//		{
//			Wite_VP(DISP_brand, ON);
//		}
//		else
//		{
//			Wite_VP(DISP_brand, OFF);
//		}
	}
	break;

	case 2:
	{
		{
			static u16 lastElecCurvePage = 0xFFFF;
			u16 page = read_PIC_Now();

			if (page == 198 || page == 199 || page == 200 || page == 201 || page == 203 || page == 206 || page == 207)
			{
				if (page != lastElecCurvePage)
				{
					lastElecCurvePage = page;
					Display_Electricity_Curve();
					Display_Electricity_Stats();
				}
			}
			else
			{
				lastElecCurvePage = 0xFFFF;
			}
		}
		Advanced_function_process();
		Connect_Overting_Handler();
	}
	break;

	case 3:
	{
		Time_Update();
		Fault_process();
		Connect_WiFi_EN_Protocol();
	}
	break;

	case 4:
		if (T_timer_1Min > 59)
		{
			T_timer_1Min = 0;
			Task_1Min_process();
		}
		else
		{
			T_timer_1Min++;
		}
		break;

	case 5:
	{
		if (monitor_mode == 1)
		{
			Read_dgus_buf[2] = monitor_timer; // ��ؼ��ʱ��
			if (Ttimer_monitor_time < monitor_time)
			{
				Ttimer_monitor_time++;
			}
			else
			{
				monitor_mode = 0;
			}
		}
		else
		{
			Ttimer_monitor_time = 0;
			read_dgus_vp(EEPROM_Connect_UP_TIME, (u8 *)&Read_dgus_buf[2], 1);
		}
	}
	break;

	case 6:
		clearFaultAferSevenDays();
		break;
	case 7:
		Send_Cache_Process();
		Brand_Select();
		break;	
	case 8:
		Electricity_process();
		IndoorTempControl();
		Display_Mute_Room_Temp_ICO();
		break;	
	default:
		break;
	}
}

void Task_1Min_process(void)
{
	static uint8_t T_timer_10Min = 0;
	static uint8_t T_timer_1H = 0;
	//static uint8_t COUNT = 0;

	if (T_timer_10Min > 9)
	{
		T_timer_10Min = 0;
		WriteTemperatureToVPBuffer();
		if (!GET_BIT(Read_dgus_buf[0], 4) && Type_Interface == SYSTEM_PASSWORD)
		{																			  // δ���ܲ��ҵ�������������ܣ�ÿ10min�·�һ�ιػ�ָ��
			Send_modebus_command_06(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF); // �·��ػ�ָ��
		}
		//a point is updated every 10 minutes there are a total of 2 016 points in 7 days and the curve can display a total of 2047 points
		// if (usElectricity < 0xEfff)
		// {
		// 	usElectricity += State_pack[STATE_KWH];
		// }

		// if (T_timer_1H > 4)
		// {
		// 	T_timer_1H = 0;
		// 	if (ulNumberEl < 0x6ffffff0)
		// 	{
		// 		ulNumberEl += usElectricity;
		// 	}
		// 	write_dgus_vp(DISP_KWH, (u8 *)&usElectricity, 1);
		// 	write_dgus_vp(EEPROM_KWH, (u8 *)&ulNumberEl, 2);
		// 	usElectricity = 0;
		// 	if (COUNT > 4)
		// 	{
		// 		COUNT = 0;
		// 		T5L_Flash(WRITE, (EEPROM_KWH), (EEPROM_KWH)-MAGIC_NUMBER, 2);
		// 	}
		// 	else
		// 	{
		// 		COUNT++;
		// 	}
		//}
		else
		{
			T_timer_1H++;
		}
	}
	else
	{
		T_timer_10Min++;
	}
}
