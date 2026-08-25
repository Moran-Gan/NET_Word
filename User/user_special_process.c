#include "Dwin_CONF.h"
#include "user_include.h"

uint8_t code Txet_Wire_Cont_Hardware[20] = "DW-M4.3-XKQ-WIFI-WT";
uint8_t Txet_Wire_Cont_ENERGY[20]="NET-DK-L1001-DB-V1.5"; /* 纽恩泰: NET-DK-L1001-DB-V*.*, 次版本由 XKQ_VERSION_ADRESS 组成为 x.y(如 16->1.6) */
uint8_t code Txet_Wire_Cont_NEUTRAL[20] = "NET-DK-L1001-AT-V1.7"; // 中性
uint8_t code Txet_Wire_Cont_XIAOYOU[20] = "NET-DK-L1001-AZ-V1.7"; // 小游
uint8_t code Txet_Wire_Cont_MSD[20] = "NET-DK-L1001-BE-V1.7";	  // 美诗盾
bit Clear_Fault_Bit = 0;										  // �����ʷ���ϱ�־λ
static u16 lastHourElectricity = 0;   // 上一小时用电（对外 0.1kWh）
static u16 yesterdayElectricity = 0;  // 昨日用电（对外 0.1kWh）
static u16 lastMonthElectricity = 0;  // 上月用电（对外 0.1kWh）
static u8 kwhFrac = 0;                // 总用电百分位余数 0~9（0.01kWh）
static void Persist_Elec_Stats_VP(void);
static u16 Elec_ToOldU16(u32 v01);    // 内部 0.01kWh -> 对外 0.1kWh

/******************************************
  函数名  : void Init_Txet_Wire_Cont_ENERGY(void)
  功能    : 按 XKQ_VERSION_ADRESS 填充纽恩泰线控器软件字串(索引 17~19 为 V 主.次)
  说明    : 如 XKQ_VERSION_ADRESS=16 则显示为 ...-V1.6
 ******************************************/
void Init_Txet_Wire_Cont_ENERGY(void)
{
	Txet_Wire_Cont_ENERGY[17] = (uint8_t)(XKQ_VERSION_ADRESS / 10u) + (uint8_t)'0';
	Txet_Wire_Cont_ENERGY[18] = (uint8_t)'.';
	Txet_Wire_Cont_ENERGY[19] = (uint8_t)(XKQ_VERSION_ADRESS % 10u) + (uint8_t)'0';
}

/******************************************
 �� �� ��  : void my_itoa(u8 str[])
��������  : �����ϵ�ʱ��
 �������  :	��
 �������  :
 �޸���ʷ  :
  1.��    ��   :2021��7��7��
	��    ��   : qiuxiaobin
	�޸�����   : ����
******************************************/
void my_itoa(u8 str[])
{

	str[0] = 2 + '0';
	str[1] = 0 + '0';
	str[2] = system.year / 10 + '0';
	str[3] = system.year % 10 + '0';
	str[4] = system.month / 10 + '0';
	str[5] = system.month % 10 + '0';
	str[6] = system.date / 10 + '0';
	str[7] = system.date % 10 + '0';
	str[8] = '-';
	str[9] = system.hour / 10 + '0';
	str[10] = system.hour % 10 + '0';
	str[11] = ':';
	str[12] = system.min / 10 + '0';
	str[13] = system.min % 10 + '0';
	str[14] = 0;
	str[15] = 0;

	//	if(system.date < 10)
	//	{
	//		str[0] = 0 + '0';
	//		str[1] = system.date%10 + '0';
	//	}
	//	else
	//	{
	//		str[0] = system.date/10 + '0';
	//		str[1] = system.date%10 + '0';
	//	}
	//	str[2] = '-';
	//
	//	if(system.month < 10)
	//	{
	//		str[3] = 0 + '0';
	//		str[4] = system.month%10 + '0';
	//	}
	//	else
	//	{
	//		str[3] = system.month/10 + '0';
	//		str[4] = system.month%10 + '0';
	//	}
	//
	//	str[5] = ' ';
	//
	//	str[6] = ' ';
	//	str[7] = ' ';
	//	if(system.hour < 10)
	//	{
	//		str[8] = 0 + '0';
	//		str[9] = system.hour%10 + '0';
	//	}
	//	else
	//	{
	//		str[8] = system.hour/10 + '0';
	//		str[9] = system.hour%10 + '0';
	//	}
	//
	//	str[10] = ':';
	//
	//	if(system.min < 10)
	//	{
	//		str[11] = 0 + '0';
	//		str[12] = system.min%10 + '0';
	//	}
	//	else
	//	{
	//		str[11] = system.min/10 + '0';
	//		str[12] = system.min%10 + '0';
	//	}
	//	str[13] = ':';
	//	if(system.sec < 10)
	//	{
	//		str[14] = 0 + '0';
	//		str[15] = system.sec%10 + '0';
	//	}
	//	else
	//	{
	//		str[14] = system.sec/10 + '0';
	//		str[15] = system.sec%10 + '0';
	//	}
}

/****************************************************************************************/
// Function.Name:    Fault_process()
// Function.Content: Fault interface processing process
// Function.Editor:  cwx
// Function.Time:    2021/04/30
/****************************************************************************************/
void Fault_process(void)
{
	uint16_t Err_code[30];
	uint16_t dgus_Err_code[3];
	uint16_t fault_data[270];
	uint16_t fault_save_data[270];
	static int16_t dgus_Err_code1[14] = {0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
										 0xffff, 0xffff, 0xffff, 0xffff};
//	uint16_t fault_freeze = 0;
	uint8_t fault_display_count = 0;
	uint8_t fault_display_count1 = 0;
	uint16_t fault_display_ico;
	uint8_t t;
	uint16_t i;
	u8 fgDISP_Level1;
	u16 usDISP_Level1;
	fgDISP_Level1 = 0;
//	if (GET_BIT(State_pack[STATE_SYS_STARE1], 15))
//		SET_BIT(fault_freeze, 0);
//	if (GET_BIT(State_pack[STATE_SYS_STARE2], 0))
//		SET_BIT(fault_freeze, 1);
//	if (GET_BIT(State_pack[STATE_SYS_STARE2], 1))
//		SET_BIT(fault_freeze, 2);

	memset(dgus_Err_code, 0, sizeof(dgus_Err_code));

	// State_pack[STATE_E_SERIES_00] &= 0x01FF; // 25bit9~bit13�޹���
	// State_pack[STATE_E_SERIES_01] &= 0x100B; // 26bit9�޹���
	// State_pack[STATE_E_SERIES_02] &= 0x0001; // 27bit12~bit15�޹���
	// State_pack[STATE_E_SERIES_03] &= 0xFF87; // 28bit3��bit6�޹���
	// State_pack[STATE_E_SERIES_04] &= 0x000F; // 29bit4~bit13�޹���
	// State_pack[STATE_E_SERIES_05] &= 0x000C; // 30bit1��bit3~bit15�޹���			//����ˮ·����ͨ���쳣
	// State_pack[STATE_E_SERIES_06] &= 0;		 // 31bit0~bit15�޹���
	// State_pack[STATE_E_SERIES_07] &= 0xB200; // 32bit14�޹���
	// State_pack[STATE_E_SERIES_08] &= 0;		 // 33bit0~bit15�޹���
	// State_pack[STATE_E_SERIES_09] &= 0;		 // 34bit0~bit15�޹���
	// State_pack[STATE_E_SERIES_10] &= 0;		 // 35bit0~bit2��bit4~bit5��bit12~bit14�޹���
	// State_pack[STATE_E_SERIES_11] &= 0;		 // 36bit0~bit2��bit4~bit5��bit12~bit14�޹���
	//	State_pack[STATE_E_SERIES_12]	&=	0;							//����������

	if (flag_communication_fault == 1)
	{
		SET_BIT(State_pack[STATE_E_SERIES_01], 0); // 通信故障
		write_dgus_vp(STATE_E_SERIES_01 + PARAMETER_20_START_ADDRESS + 0x5000, (u8 *)&State_pack[STATE_E_SERIES_01], 1);
	}

	for (t = 0; t < 9; t++)
	{
		for (i = 0; i < 16; i++)
		{
			if (GET_BIT(State_pack[STATE_E_SERIES_00 + t], i) && (Failure_Level[t][i] == 1))
			{
				if (Fault_Time_Cnt[t][i] < 5000)
				{
					Fault_Time_Cnt[t][i]++;
				}
				if (Fault_Time_Cnt[t][i] > 30 * 60)
				{
					fgDISP_Level1 = 1;
				}
			}
			else
			{
				Fault_Time_Cnt[t][i] = 0;
			}

			#if defined(Brand_ENERGY)
						{
							if (GET_BIT(Read_dgus_buf[0], 4))
							{
								if (GET_BIT(State_pack[STATE_E_SERIES_00 + t], i) && fault_display_count < 3 && (Failure_Level[t][i] == LEVEL3))
								{
									dgus_Err_code[fault_display_count++] = (i + (16 * t)) + 1; //(((STATE_E_SERIES_00 + t)	- STATE_E_SERIES_01*16) + i) + 3;
								}
							}
							else if (GET_BIT(Read_dgus_buf[0], 3))
							{
								if (GET_BIT(State_pack[STATE_E_SERIES_00 + t], i) && fault_display_count < 3)
								{
									dgus_Err_code[fault_display_count++] = (i + (16 * t)) + 1;
								}
							}
						}
			#else
						{
							if (T_Detection_time > 3600)
							{
								if (GET_BIT(State_pack[STATE_E_SERIES_00 + t], i) && fault_display_count < 3 && (Failure_Level[t][i] == LEVEL3))
								{
									dgus_Err_code[fault_display_count++] = (i + (16 * t)) + 1; //(((STATE_E_SERIES_00 + t)	- STATE_E_SERIES_01*16) + i) + 3;
								}
							}
							else
							{
								if (GET_BIT(State_pack[STATE_E_SERIES_00 + t], i) && fault_display_count < 3)
								{
									dgus_Err_code[fault_display_count++] = (i + (16 * t)) + 1;
								}
							}
						}
			#endif					
			if (GET_BIT(State_pack[STATE_E_SERIES_00 + t], i))
			{
				if (GET_BIT(dgus_Err_code1[t], i) && ((Failure_Level[t][i] == LEVEL3) || (Failure_Level[t][i] == LEVEL2) || ((Failure_Level[t][i] == LEVEL1) && (Fault_Time_Cnt[t][i] > 30 * 60))))
				{
					CLR_BIT(dgus_Err_code1[t], i);
					if (fault_display_count1 < 3)
					{
						Err_code[fault_display_count1++] = (i + (16 * t)) + 1; //(((STATE_E_SERIES_01 + t)	- STATE_E_SERIES_01*16) + i) + 3;
					}
				}
			}
			else if (!GET_BIT(dgus_Err_code1[t], i))
			{
				SET_BIT(dgus_Err_code1[t], i);
			}
		}
	}

	for (i = 0; i < 16; i++)
	{
		if (fault_display_count < 3 && GET_BIT(State_pack[STATE_E_SERIES_12], i))
		{
			dgus_Err_code[fault_display_count++] = (i + (16 * 12)) + 1; //(((STATE_E_SERIES_00 + t)	- STATE_E_SERIES_01*16) + i) + 3;
		}
		if (GET_BIT(State_pack[STATE_E_SERIES_12], i)) // 联机通信故障
		{

			if (GET_BIT(dgus_Err_code1[12], i))
			{
				CLR_BIT(dgus_Err_code1[12], i);
				if (fault_display_count1 < 30)
				{
					Err_code[fault_display_count1++] = (i + (16 * 12)) + 1; //(((STATE_E_SERIES_01 + t)	- STATE_E_SERIES_01*16) + i) + 3;
				}
			}
		}
		else if (!GET_BIT(dgus_Err_code1[12], i))
		{
			SET_BIT(dgus_Err_code1[12], i);
		}

//		if (fault_display_count < 3 && (i == 2) && (GET_BIT(fault_freeze, i)))
//		{
//			dgus_Err_code[fault_display_count++] = (i + (16 * 13)) + 1; //(((STATE_E_SERIES_00 + t)	- STATE_E_SERIES_01*16) + i) + 3;
//		}
//		if (GET_BIT(fault_freeze, i)) // 防冻
//		{

//			if (GET_BIT(dgus_Err_code1[13], i) && (i == 1 || i == 2))
//			{
//				CLR_BIT(dgus_Err_code1[13], i);
//				if (fault_display_count1 < 30)
//				{
//					Err_code[fault_display_count1++] = (i + (16 * 13)) + 1; //(((STATE_E_SERIES_01 + t)	- STATE_E_SERIES_01*16) + i) + 3;
//				}
//			}
//		}
//		else if (!GET_BIT(dgus_Err_code1[13], i))
//		{
//			SET_BIT(dgus_Err_code1[13], i);
//		}
	}

	if (fault_display_count)
	{
		if (Read_dgus_buf[0] & 0x04 && T_BEEP_TIME > 29)
		{
			T_BEEP_TIME = 0;
			write_Button(500);
		}
		write_dgus_vp(DISP_Fault_VALUE, (u8 *)&dgus_Err_code, 2);
		fault_display_ico = 1;
	}
	else
	{
		if (fault_ico == 1)
		{
			write_dgus_vp(DISP_Fault_VALUE, (u8 *)&dgus_Err_code, 2);
		}
		fault_display_ico = 0;
	}
	fault_ico = fault_display_ico;
	write_dgus_vp(DISP_FAULT_ICO, (u8 *)&fault_display_ico, 1);

	if (fault_display_count1 > 0)
	{
		memset(fault_data, 0x00, sizeof(fault_data));
		memset(fault_save_data, 0x00, sizeof(fault_save_data));

		if (fault_display_count1 >= 30)
		{ // ��ֹ����Χ
			fault_display_count1 = 30;
		}
		for (i = 0; i < fault_display_count1; i++)
		{
			fault_data[i * 9] = Err_code[i];	   // ���ϱ��
			my_itoa((u8 *)&fault_data[i * 9 + 1]); // ʱ��
		}
		read_dgus_vp(EEPROM_FAULT, (u8 *)fault_save_data, 270); // ��ȡ��ʷ����
		for (i = 0; i < 270 - (fault_display_count1 * 9); i++)	// 270
		{
			fault_data[i + (fault_display_count1 * 9)] = fault_save_data[i];
		}
		write_dgus_vp(EEPROM_FAULT, (u8 *)fault_data, 270);
		//T5L_Flash(0xA5, EEPROM_FAULT, (EEPROM_FAULT - MAGIC_NUMBER), 270);
		EEPROM_Write_page();
	}

	if (Clear_Fault_Bit)
	{ // �����ʷ����
		Clear_Fault_Bit = false;
		memset(fault_save_data, 0x00, sizeof(fault_save_data));
		fault_display_count1 = 0;
		write_dgus_vp(EEPROM_FAULT, (u8 *)fault_save_data, 270);
		//T5L_Flash(0xA5, EEPROM_FAULT, (EEPROM_FAULT - MAGIC_NUMBER), 270);
		EEPROM_Write_page();
	}

	usDISP_Level1 = Read_VP(DISP_Level1);
	if (fgDISP_Level1 == 1)
	{
		SET_BIT(usDISP_Level1, 3);
		Wite_VP(DISP_Level1, usDISP_Level1);
	}
	else
	{
		CLR_BIT(usDISP_Level1, 3);
		Wite_VP(DISP_Level1, usDISP_Level1);
	}

	Generate_recover_fault(); // WiFi模式下，产生、消除故障，实时上传平台
}

void checkFault(void)
{
}

void Clear_Fault_process(void)
{
	Clear_Fault_Bit = true;
}

void Generate_recover_fault(void)
{
	u8 i;
	for (i = 0; i < 9; i++)
	{
		if (State_pack[STATE_E_SERIES_00 + i] != usFaultStorageBuf[i])
		{
			usFaultStorageBuf[i] = State_pack[STATE_E_SERIES_00 + i];
			T_Connect_Send_State_time = Send_State_Time;
		}
	}
}
/****************************************************************************************/
// Function.Name:    Icon_driver_process()
// Function.Content: ����״̬���־λ��Ӧ����
// Function.Editor:
// Function.Time:
/****************************************************************************************/
void Icon_driver_process(void)
{
	// 快速排序

}

void Display_TIP_ICO(uint8_t value)
{
	Flag_Tip = value;
	T_Tip_time = 6;
}

/****************************************************************************************/
// Function.Name:    Advanced_function_process()
// Function.Content: �߼����ܴ�������
// Function.Editor:  cwx
// Function.Time:    2021/04/30
/****************************************************************************************/
void Advanced_function_process(void)
{
	int16_t TEMP[4];
	int16_t indoor_temp = 0;
	int16_t rdIndoor_Temp = 0;
	u16  T_env; 
  uint16_t H_env, T_8310, H_8310;
  int16_t indoor_humidity_compensation = 0;
	if (Para_pack[PARA_MODE] == COOLING)
	{
		TEMP[0] = 1;
	}
	else
	{
		TEMP[0] = 0;
	}
	write_dgus_vp(DISP_mode_ico, (u8 *)&TEMP[0], 1); // ��ʾ״̬ģʽ

	TEMP[0] = 0;
	if (Para_pack[PARA_H04_97] == 0 || (Para_pack[PARA_H04_97] == 1 && Para_pack[PARA_F24_128] == 0))
	{
		if (Para_pack[PARA_F5_122] == ON || Para_pack[PARA_F5_122] == 2)
		{
			TEMP[0] |= 0x01;
		}

		if (Para_pack[PARA_F19_123] == ON)
		{
			TEMP[0] |= 0x02;
		}
	}
	write_dgus_vp(DISP_linkage_ico, (u8 *)&TEMP[0], 1); // ��ʾ״̬ģʽ
	

	H_env = DHT83_MeasurementResult(&T_8310, &H_8310, T_env, sys_params.K0);
	//H_8310 += ((Read_VP(INDOOR_HUMIDITY_COMPENSATION))*10);
	//write_dgus_vp(DHT83_REAL, (u8*)&H_8310, 1);

	indoor_humidity_compensation = Read_VP(INDOOR_HUMIDITY_COMPENSATION);
	Wite_VP(DHT83_REAL, H_8310+(indoor_humidity_compensation*10));
	// if (GET_BIT(State_pack[STATE_SYS_STARE2], 5))
	// { // Ԥ��
	// 	TEMP[0] = 7;
	// }
	// else if (Para_pack[PARA_F5_122] == ON && Para_pack[PARA_H04_97] == 0)
	// { // �����ػ���������������ʱ������������Ч
	// 	if ((State_pack[STATE_SWITCH] & 0X01))
	// 	{
	// 		TEMP[0] = 5;
	// 		//			if(Para_pack[PARA_MODE]	==	COOLING)
	// 		//			{
	// 		//				TEMP[0]	=	6;
	// 		//			}
	// 		//			else
	// 		//			{
	// 		//				TEMP[0]	=	7;
	// 		//			}
	// 	}
	// 	else
	// 	{
	// 		if (Para_pack[PARA_MODE] == COOLING)
	// 		{
	// 			if (Para_pack[PARA_MUTE] == ECO)
	// 				TEMP[0] = 4; // ��������
	// 			else
	// 				TEMP[0] = 3; // ǿ������
	// 		}
	// 		else
	// 		{
	// 			if (Para_pack[PARA_MUTE] == ECO)
	// 				TEMP[0] = 2;
	// 			else
	// 				TEMP[0] = 1;
	// 		}
	// 	}
	// }
	// else if (Para_pack[PARA_F5_122] == 2 && Para_pack[PARA_H04_97] == 0 && (State_pack[STATE_SWITCH] & 0X01))
	// {
	// 	TEMP[0] = 5;
	// 	//		if(Para_pack[PARA_MODE]	==	COOLING)
	// 	//			{
	// 	//				TEMP[0]	=	6;
	// 	//			}
	// 	//			else
	// 	//			{
	// 	//				TEMP[0]	=	7;
	// 	//			}
	// }
	// else
	// {
	// 	if (Para_pack[PARA_POWER_ON] == OFF)
	// 	{
	// 		TEMP[0] = 6;
	// 		//			if(Para_pack[PARA_MODE]	==	COOLING)
	// 		//			{
	// 		//				TEMP[0]	=	8;
	// 		//			}
	// 		//			else
	// 		//			{
	// 		//				TEMP[0]	=	9;
	// 		//			}
	// 	}
	// 	else
	// 	{
	// 		if (Para_pack[PARA_MODE] == COOLING)
	// 		{
	// 			if (Para_pack[PARA_MUTE] == ECO)
	// 				TEMP[0] = 4;
	// 			else
	// 				TEMP[0] = 3;
	// 		}
	// 		else
	// 		{
	// 			if (Para_pack[PARA_MUTE] == ECO)
	// 				TEMP[0] = 2;
	// 			else
	// 				TEMP[0] = 1;
	// 		}
	// 	}
	// }
	// write_dgus_vp(DISP_linkage_ico, (u8 *)&TEMP[0], 1); // ��ʾ״̬ģʽ
	if(Para_pack[PARA_F19_123]==0)
	{
		if (Para_pack[PARA_MODE] == COOLING)
		{
			if (Para_pack[PARA_MUTE] == ECO)
				TEMP[0] = 3;
			else
				TEMP[0] = 2;
		}
		else
		{
			if (Para_pack[PARA_MUTE] == ECO)
				TEMP[0] = 1;
			else
				TEMP[0] = 0;
		}
	}
	else
	{
		if(State_pack[STATE_SWITCH] & 0X80)	
		{
			if (Para_pack[PARA_MUTE] == ECO)
				TEMP[0] = 3;
			else
				TEMP[0] = 2;
			if(read_PIC_Now() == 0x04)
			{
				write_PIC_Set(5);
			}
		}	
		else
		{
			if (Para_pack[PARA_MUTE] == ECO)
				TEMP[0] = 1;
			else
				TEMP[0] = 0;
			if(read_PIC_Now() == 0x05)
			{
				write_PIC_Set(4);
			}
		}
	}
	write_dgus_vp(DISP_STATE_MODE, (u8 *)&TEMP[0], 1); //the main interface mode is displayed

	if(Read_VP(TEMPERATURE_CONTROL)==0)
	{
		// if (Para_pack[PARA_F5_122]==0)//linkage switch disable
		// {
		// 	if (Para_pack[PARA_POWER_ON] == 1)
		// 	{
		// 		TEMP[0] = 2;//linkage shutdown
		// 	}
		// 	else if (Para_pack[PARA_POWER_ON] == 0)
		// 	{
		// 		TEMP[0] = 1;//power off
		// 	}
			
		// }
		// else if (Para_pack[PARA_F5_122])//linkage switch enable
		// {
		// 	if (State_pack[STATE_SWITCH] & 0X01)
		// 	{
		// 		TEMP[0] = 0;//linkage shutdown
		// 	}
		// 	else
		// 	{
		// 		TEMP[0] = 2;//power on
		// 	}
		// }
		TEMP[0] = HostPower;
	}
	else
	{
		if (Read_VP(INDOOR_POWER) == ON)//control the power on and off at room temperature 
		{
			TEMP[0] = 2;
		}
		else
		{
			TEMP[0] = 1;
		}
	}
	write_dgus_vp(DISP_POWER, (u8 *)&TEMP[0], 1); //the main interface power is displayed

	if(Read_VP(TEMPERATURE_CONTROL)==0)
	{
		if (HostMode == COOLING)
		{
			if (Para_pack[PARA_H08_101] == ON || Read_dgus_buf[1] == 0)
			{
				TEMP[0] = State_pack[STATE_TEMP_OUT] / 10;
				TEMP[1] = State_pack[STATE_TEMP_IN] / 10;
				TEMP[3] = 0;
			}
			else
			{
				TEMP[1] = State_pack[STATE_TEMP_OUT] / 10;
				TEMP[0] = State_pack[STATE_TEMP_IN] / 10;
				TEMP[3] = 1;
			}
			write_dgus_vp(DISP_SET_TEMP, (u8 *)&Para_pack[PARA_COOLING_TEMP], 1); // ӳ������Ŀ���¶�
			write_dgus_vp(DISP_COOLING_WATER, (u8 *)&TEMP[3], 1); // ӳ������Ŀ���¶�
		}
		else
		{
			if (Para_pack[PARA_H07_100] == ON || Read_dgus_buf[1] == 0)
			{
				TEMP[0] = State_pack[STATE_TEMP_OUT] / 10;
				TEMP[1] = State_pack[STATE_TEMP_IN] / 10;
				TEMP[3] = 0;
			}
			else
			{
				TEMP[0] = State_pack[STATE_TEMP_IN] / 10;
				TEMP[1] = State_pack[STATE_TEMP_OUT] / 10;
				TEMP[3] = 1;
			}
			write_dgus_vp(DISP_SET_TEMP, (u8 *)&Para_pack[PARA_HEATING_TEMP], 1); // ӳ��������Ŀ���¶�
			write_dgus_vp(DISP_HEATING_WATER, (u8 *)&TEMP[3], 1); // ӳ������Ŀ���¶�
		}	
		write_dgus_vp(INTERFACE_TEMP, (u8 *)&State_pack[STATE_TEMP_OUT], 1);
	}
	else
	{
		indoor_temp = Read_VP(INDOOR_TEMP_SET);
		write_dgus_vp(DISP_SET_TEMP, (u8 *)&indoor_temp, 1);
		rdIndoor_Temp = Read_VP(0x5000+62);
		write_dgus_vp(INTERFACE_TEMP, (u8 *)&rdIndoor_Temp, 1);
	}
	if (State_pack[STATE_SYS_STARE1] & 0xF00)
	{ // ��˪��־λ
		SET_BIT(TIME_ICO, 3);
	}

	write_dgus_vp(DISP_TEMP_MAX, (u8 *)TEMP, 2);

	write_dgus_vp(DISP_TIP_ICO, (u8 *)&Flag_Tip, 1);
	write_dgus_vp(DISP_TIMER_ICO, (u8 *)&TIME_ICO, 1);

	if (State1_pack[21] == 1)
	{
		if (State1_pack[22] > 0 && State1_pack[22] < 10)
		{
			TEMP[0] = 1;
		}
		else if (State1_pack[22] >= 10 && State1_pack[22] < 20)
		{
			TEMP[0] = 2;
		}
		else if (State1_pack[22] >= 20 && State1_pack[22] < 30)
		{
			TEMP[0] = 3;
		}
		else
		{
			TEMP[0] = 4;
		}
	}
	else
	{
		TEMP[0] = 0;
	}

	write_dgus_vp(DISP_4G_ICO, (u8 *)&TEMP[0], 1);

	//	if(wifi_work_state	==	4)
	//	{
	//		if(WIFI_dBM	<=	-88)
	//		{
	//			TEMP[0]	=	1;
	//		}
	//		else	if(WIFI_dBM	<=	-55	&&	WIFI_dBM	>	-88)
	//		{
	//			TEMP[0]	=	2;
	//		}
	//		else
	//		{
	//			TEMP[0]	=	3;
	//		}
	////		TEMP[0]	=	3;
	//	}
	//	else	if(wifi_work_state	==	0	||	wifi_work_state	==	1	||	wifi_work_state	==	7)
	//	{
	//		TEMP[0]	=	4;
	//	}
	//	else
	//	{
	//		TEMP[0]	=	0;
	//	}
	TEMP[0] = 0;
	if (GET_BIT(Flag_Connect_EN, 1))
	{
		TEMP[0] = 4;
	}
	else if (GET_BIT(Flag_Connect_EN, 2))
	{
		if (WIFI_dBM <= -88)
		{
			TEMP[0] = 1;
		}
		else if (WIFI_dBM <= -55 && WIFI_dBM > -88)
		{
			TEMP[0] = 2;
		}
		else
		{
			TEMP[0] = 3;
		}
		//		TEMP[0]	=	3;
	}
	write_dgus_vp(DISP_WIFI_ICO, (u8 *)&TEMP[0], 1);

	if(Para_pack[PARA_H06_99]	!= 0)
	{
		if(GET_BIT(Flag_Connect_EN, 0))
		{
			TEMP[0] = 1; // WiFi Restart
		}
		else if(GET_BIT(Flag_Connect_EN, 1))
		{
			TEMP[0] = 2; // WiFi DistributionNetwork
		}
		else if(GET_BIT(Flag_Connect_EN, 2))
		{
			TEMP[0] = 3; // WiFi OnLine
		}
		else
		{
			TEMP[0] = 0; // WiFi Offline
		}
	}
	else
	{
		if (State1_pack[STATE_NETWORK_STATUS])
			TEMP[0] = 3;
		else
			TEMP[0] = 0;
	}
		
	write_dgus_vp(NETWORK_STATUS_ICON, (u8 *)&TEMP[0], 1);
	write_dgus_vp(DISP_test_UART, (u8 *)&UART_TSET, 1);

	if (Para_pack[PARA_H04_97] == ON)
	{
		if (Para_pack[PARA_F24_128] == OFF)
		{
			TEMP[0] = 1;
		}
		else
		{
			TEMP[0] = 2;
		}
	}
	else
	{
		TEMP[0] = 0;
	}
	write_dgus_vp(0x7055, (u8 *)&TEMP[0], 1);
	
	if (Enable_Host_Unlock == 1 && State2_pack[STATE_FLAG_UNLOCK] == 0 && (SYSTEM_PASSWORD != read_PIC_Now()))		// 通讯恢复后，未解密情况
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
	else if (Enable_Host_Unlock == 1 && State2_pack[STATE_FLAG_UNLOCK] == 1 && (SYSTEM_PASSWORD == read_PIC_Now()))		// 通讯恢复后，已解密情况
	{
		Return_Home();
		if (!GET_BIT(Read_dgus_buf[0], 4))
		{
			SET_BIT(Read_dgus_buf[0], 4);
			write_dgus_vp(EEPROM_PARA, (u8 *)&Read_dgus_buf[0], 1);
			EEPROM_Write_page();
		}
	}
	
	if (WIFI_SOP_INTERFACE != read_PIC_Now() && Para_pack[PARA_H06_99] == 0)		// 退出SOP后，未启用WIFI时屏蔽WIFI
	{
		SOP_MODE = 0;
	}

	write_dgus_vp(DISP_Tset_WiFi, (u8 *)&WIFI_TESET_VAILE, 1);
}	

/****************************************************************************************/
// Function.Name:
// Function.Content:
// Function.Editor:
// Function.Time:
/****************************************************************************************/
u8 fgTimerTime;
void Timer_run_process(void)
{
	uint16_t Hour_Min = system.hour << 8 | system.min;

	uint16_t timer_value[15];
	//	uint16_t	timer_start = 0xFFFF,timer_end = 0xFFFF;
	uint16_t time_temp_value;
	bit time_en = 0;
	//	static	bit	timer_temp_en = 0;
	uint16_t muteEnable = 0;
	uint8_t i;

	write_dgus_vp(0x7056, (u8 *)&Hour_Min, 1);

	/*
		Timing temperature control
	*/
	read_dgus_vp(EEPROM_TIMER_MUTE, (u8 *)&timer_value[0], 7);
	if (timer_value[0] & 0x007E)
	{
		SET_BIT(TIME_ICO, 0);
		for (i = 0; i < 6; i++)
		{
			if (GET_BIT(timer_value[0], (i + 1)))
			{
				//if (timer_value[i * 2 + 1] != timer_value[i * 2 + 2])
				//{
					if (timer_value[i + 1] == Hour_Min && system.sec == 3&& (i < 3))
					{
						if(HostMode == HEATING)
						{
							Send_Cache_Write(PARA_MUTE + PARAMETER_92_START_ADDRESS, ECO);
							Send_Cache_Write(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);
						}
						else
						{
							Send_Cache_Write(PARA_MUTE + PARAMETER_92_START_ADDRESS, ECO);
							Send_Cache_Write(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);
						}
					}
					else if (timer_value[i  + 1] == Hour_Min && system.sec == 3&& (i >= 3))
					{
						if(HostMode == HEATING)
						{
							Send_Cache_Write(PARA_MUTE + PARAMETER_92_START_ADDRESS, OFF);
							Send_Cache_Write(PARA_MODE + PARAMETER_92_START_ADDRESS, HEATING);
						}
						else
						{
							Send_Cache_Write(PARA_MUTE + PARAMETER_92_START_ADDRESS, OFF);
							Send_Cache_Write(PARA_MODE + PARAMETER_92_START_ADDRESS, COOLING);
						}
					}
				//}
			}
		}
	}
	/*
	静音定时
	*/
	read_dgus_vp(TIMER_MUTE_ENABLE, (u8 *)&timer_value[0], 7);
	//静音使能)&&(muteEnable==1)
	muteEnable = Read_VP(0x50AE);
	if (timer_value[0] & 0x003F)
	{
		SET_BIT(TIME_ICO, 2);
		for (i = 0; i < 6; i++)
		{
			if (GET_BIT(timer_value[0], i))
			{
				if (timer_value[i + 1] == Hour_Min && system.sec == 6 && (i < 3))//
				{
					if(Read_VP(0x50AF)==0)
					{
						Wite_VP(0x50AF,1);
						Send_Cache_Write(175, ON);
					}
				}
				else if (timer_value[i + 1] == Hour_Min && system.sec == 6 && (i >= 3))//
				{
					if(Read_VP(0x50AF)==1)
					{
						Wite_VP(0x50AF,0);
						Send_Cache_Write(175, OFF);
					}
				}
			}
		}
	}

	/*
	timing switcher
	*/
	read_dgus_vp(EEPROM_TIMER_POWER, (u8 *)&timer_value[0], 7);
	if (timer_value[0] & 0x003F)
	{
		SET_BIT(TIME_ICO, 2);
		for (i = 0; i < 6; i++)
		{
			if (GET_BIT(timer_value[0], i))
			{
				//if (timer_value[i * 2 + 1] != timer_value[i * 2 + 2]) //&&	GET_BIT(timer_value[i*3+3],system.week))
				//{
					if (timer_value[i  + 1] == Hour_Min && system.sec == 1 && (i < 3))
					{
						if(Read_VP(TEMPERATURE_CONTROL)==0)
						{
							Send_Cache_Write(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, ON);
						}
						else
						{
							Wite_VP(INDOOR_POWER,ON);
						}
					}
					else if (timer_value[i + 1] == Hour_Min && system.sec == 1 && (i >= 3))
					{
						if(Read_VP(TEMPERATURE_CONTROL)==0)
						{
							Send_Cache_Write(PARA_POWER_ON + PARAMETER_92_START_ADDRESS, OFF);
						}
						else
						{
							Wite_VP(INDOOR_POWER,OFF);
						}
					}
				//}
			}
		}
	}

	/*
	��ʱ����
	ʹ�������жϣ����������ص�ʱ����,ִֻ������ִ�в�������
	*/

	read_dgus_vp(EEPROM_TIMER_TEMP, (u8 *)&timer_value[0], 12);
	if (timer_value[0] & 0x0007)
	{
		SET_BIT(TIME_ICO, 1);
		for (i = 0; i < 3; i++)
		{
			if (GET_BIT(timer_value[0], i))
			{
				if (timer_value[i * 3 + 1] != timer_value[i * 3 + 2])
				{ // ��Чʱ����
					if (timer_value[i * 3 + 1] > timer_value[i * 3 + 2])
					{ // ����
						if ((Hour_Min == timer_value[i * 3 + 1]) && (fgTimerTime != i * 3 + 1) && (system.sec == 8))
						{
							fgTimerTime = i * 3 + 1;
							time_en = 1;
							time_temp_value = timer_value[i * 3 + 3];
						}
					}
					// else
					// { // ����
					// 	if ((Hour_Min >= timer_value[i * 3 + 1] && Hour_Min < timer_value[i * 3 + 2]) && (fgTimerTime != i * 3 + 2)&&(system.sec <=5))
					// 	{
					// 		fgTimerTime = i * 3 + 2;
					// 		time_en = 1;
					// 		time_temp_value = timer_value[i * 3 + 3];
					// 	}
					// }
				}
			}
		}
	}
	if (system.sec > 8 && system.sec < 10)
	{
		fgTimerTime = 0;
	}
	if (time_en == 1)
	{
		time_en = false;
		if (!GET_BIT(timer_value[0], 15))
		{
			CPL_BIT(timer_value[0], 15); // ���浱ǰ�¶��趨
			//			if(Para_pack[PARA_MODE]	==	HEATING)
			//			{																										//����
			write_dgus_vp(0x4019, (u8 *)&Para_pack[PARA_HEATING_TEMP], 1);
			//			}else
			//			{																										//����
			write_dgus_vp(0x401a, (u8 *)&Para_pack[PARA_COOLING_TEMP], 1);
			//			}
			write_dgus_vp(EEPROM_TIMER_TEMP, (u8 *)&timer_value[0], 1);
			//T5L_Flash(WRITE, EEPROM_TIMER_TEMP, EEPROM_TIMER_TEMP - MAGIC_NUMBER, 2);
			//T5L_Flash(WRITE, 0x4018, 0x4018 - MAGIC_NUMBER, 4);
			EEPROM_Write_page();
		}
		if(Read_VP(TEMPERATURE_CONTROL)==0)
		{
			if (HostMode == HEATING)
			{ // ����
				if (time_temp_value >= Para_init_DATA[PARA_HEATING_TEMP][0] && time_temp_value <= Para_pack[PARA_HEATING_MAX] && time_temp_value != Para_pack[PARA_HEATING_TEMP])
				{
					Send_Cache_Write(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS, time_temp_value);
					write_dgus_vp(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS + TOUCH_EVENT_FLAG, (u8 *)&Para_pack[PARA_HEATING_TEMP], 1);
				}
			}
			else
			{ // ����

				if (time_temp_value >= Para_pack[PARA_COOLING_MIN] && time_temp_value <= Para_init_DATA[PARA_COOLING_TEMP][1] && time_temp_value != Para_pack[PARA_COOLING_TEMP])
				{
					Send_Cache_Write(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS, time_temp_value);
					write_dgus_vp(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS + TOUCH_EVENT_FLAG, (u8 *)&Para_pack[PARA_COOLING_TEMP], 1);
				}
			}
		}
		else
		{
			if (time_temp_value >= 16 && time_temp_value <= 32 )
			{
				Wite_VP(INDOOR_TEMP_SET,time_temp_value);
			}
		}
	}
	// else if (GET_BIT(timer_value[0], 15))
	// {								 // �˳���ʱ����
	// 	CPL_BIT(timer_value[0], 15); // ���浱ǰ�¶��趨
	// 	write_dgus_vp(EEPROM_TIMER_TEMP, (u8 *)&timer_value[0], 1);
	// 	T5L_Flash(WRITE, EEPROM_TIMER_TEMP, EEPROM_TIMER_TEMP - MAGIC_NUMBER, 2);
	// 	if (Para_pack[PARA_MODE] == HEATING)
	// 	{ // ����
	// 		if (timer_value[10] >= Para_init_DATA[PARA_HEATING_TEMP][0] && timer_value[10] <= Para_pack[PARA_HEATING_MAX])
	// 		{
	// 			Send_modebus_command_06(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS, timer_value[10]);
	// 			write_dgus_vp(PARA_HEATING_TEMP + PARAMETER_92_START_ADDRESS + TOUCH_EVENT_FLAG, (u8 *)&timer_value[10], 1);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// ����
	// 		if (timer_value[11] >= Para_pack[PARA_COOLING_MIN] && timer_value[11] <= Para_init_DATA[PARA_COOLING_TEMP][1])
	// 			Send_modebus_command_06(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS, timer_value[11]);
	// 		write_dgus_vp(PARA_COOLING_TEMP + PARAMETER_92_START_ADDRESS + TOUCH_EVENT_FLAG, (u8 *)&timer_value[11], 1);
	// 	}
	// }
}

uint8_t code SignalInot[3] = "   ";

static void Init_Elec_Hide_VP(void)
{
	u8 i;

	for (i = 0; i < ELEC_HIDE_COUNT; i++)
	{
		Wite_VP(DISP_ELEC_HIDE_H1 + i, ELEC_HIDE_VALUE);
	}
	Wite_VP(DISP_ELEC_ENABLE, 0);
}

void Set_Elec_Hide_Select(u16 select)
{
	static u8 last_idx = 0xFF;
	u8 new_idx;

	if ((select < 1) || (select > ELEC_HIDE_COUNT))
	{
		return;
	}

	new_idx = (u8)(select - 1);
	if ((last_idx < ELEC_HIDE_COUNT) && (last_idx != new_idx))
	{
		Wite_VP(DISP_ELEC_HIDE_H1 + last_idx, ELEC_HIDE_VALUE);
	}
	Wite_VP(DISP_ELEC_HIDE_H1 + new_idx, ELEC_SHOW_VALUE);
	last_idx = new_idx;
}

void Var_Init(void)
{
	u16 savedYear;
	u16 savedMonth;
	u16 savedDay;
	u16 elecVpTmp;
	T_timer_2ms = 0;
	T_timer_10ms = 0;
	T_timer_100ms = 0;
	T_timer_500ms = 0;
	T_timer_1S = 0;
	T_timer_1Min = 0;
	Time_first_power_on = 5;
	modbus_send_06_length = 0;
	Flag_Sleep = 0;
	Sleep_Count = 0;
	Sleep_Count_M = 0;
	Flag_Interface = 0;
	T_Curve_time = 0;
	T_modbus_rx_count = 0;
	T_Connect_rx_count = 0;
	flag_communication_fault = 0;
	T_BEEP_TIME = 0;
	T_holiday_TIME = 0;
	Flag_holiday_mode = 0;
	rdIndoor_Temp_TIME = 0;
	//	Flag_read_all_data				=	1;
	T_first_power_on_03_commnd = 0;
	Flag_function_06_command = 0;
	Flag_function_10_command = 0;
	Flag_Connect_function_03_command = 0;
	T_Connect_Send_State_time = 0;
	T_Connect_Send_Advance_time = 0;
	T_TSET_TIME_S = 0;
	T_TSET_TIME_Min = 0;
	usElectricity = 0;
	//	ulNumberEl							=	0;
	UART_TSET = 0;
	WIFI_dBM = 0;
	UART_MODBUS_OK = false;

	Adress_Connect_06_command_buf = 0;
	Connect.at_cmd = NOP;
	//	password_value				=	0;
	Connect_test = 0;
	T_Connect_send_time = 0; // 4S��ʱ
	T_Detection_time = 0;	 // 商检故障显示时间
	monitor_mode = false;
	monitor_timer = 5;
	monitor_time = 300;
	WIFI_TESET_VAILE = 0;
	T_Connect_4G_wifi_time = 300;
	send_modbus_4g = 0;
	Connect_master_tx_command = 0;

	memset(State_pack, 0, STATE_PACK_NUM);

	memset(Para_pack, 0, sizeof(Para_pack));
	memset(State1_pack, 0, sizeof(State1_pack));
	memset(usFaultStorageBuf, 0, sizeof(usFaultStorageBuf));
	EEPROM_Read_page();

	Init_Elec_Hide_VP();

	write_dgus_vp(DISP_POWER_ICO, (u8 *)&T_Curve_time, 1); // ��̬��ʾ����
	write_dgus_vp(DISP_READ_ICO, (u8 *)&T_Curve_time, 1);  // ��̬��ʾ��ȡ
	//	T_Curve_time	=	1;
	write_dgus_vp(DISP_NET, (u8 *)&T_Curve_time, 1);			  // ��ʾŦ��̩����
	write_dgus_vp(DIPS_INIT_MODGUS_PARA, (u8 *)&T_Curve_time, 1); // ��ʾŦ��̩����
	//	T_Curve_time = 0;
	write_dgus_vp(DISP_Wire_Cont_Hardware, Txet_Wire_Cont_Hardware, 10); // ��ʾӲ���汾��
// write_dgus_vp(DISP_Wire_Cont_Software, Txet_Wire_Cont_Software, 10); // ��ʾ�����汾��
#if defined(Brand_NEUTRAL)
	write_dgus_vp(DISP_Wire_Cont_Software, Txet_Wire_Cont_NEUTRAL, 10);
#endif
#if defined(Brand_ENERGY)
	Init_Txet_Wire_Cont_ENERGY();
	write_dgus_vp(DISP_Wire_Cont_Software, Txet_Wire_Cont_ENERGY, 10);
#endif
#if defined(Brand_XIAOYOU)
	write_dgus_vp(DISP_Wire_Cont_Software, Txet_Wire_Cont_XIAOYOU, 10);
#endif

#if defined(Brand_MSD)
	write_dgus_vp(DISP_Wire_Cont_Software, Txet_Wire_Cont_MSD, 10);
#endif

	//	wifi_protocol_init();
	//T5L_Flash(0x5A, EEPROM_FAULT, (EEPROM_FAULT - MAGIC_NUMBER), 270);
	Brand_Select(); // 品牌选择
	Time_communication_30S = 30;

	write_dgus_vp(DISP_Signal_strength, SignalInot, 3);

	// read total electricity
	Time_Update();
	totalHour = 0;
	Wite_VP(DISP_Electricity_H1+system.hour,totalHour);
	read_dgus_vp(DISP_Electricity_D1+system.date-1, (u8 *)&totalDay, 1);
	read_dgus_vp(DISP_Electricity_M1+system.month-1, (u8 *)&elecVpTmp, 1);
	totalMonth = elecVpTmp;

	SOP_MODE = 0;
	Enable_Host_Unlock = 0;
	Flag_Host_Unlock = 0;
	T_Network_Offline = 0;
	TIPS_ICON = 0;
	read_dgus_vp(DISP_Electricity_H1, (u8 *)&dailyElectricityNum, 24);//读出每小时电量
	read_dgus_vp(DISP_Electricity_D1, (u8 *)&monthlyElectricityNum, 31);//读出每天电量
	read_dgus_vp(DISP_Electricity_M1, (u8 *)&yearlyElectricityNum, 12);//读出每月电量
	lastHourElectricity = Read_VP(EEPROM_ELEC_LAST_HOUR);
	yesterdayElectricity = Read_VP(EEPROM_ELEC_YESTERDAY);
	lastMonthElectricity = Read_VP(EEPROM_ELEC_LAST_MONTH);
	savedYear = Read_VP(ELECTRICITY_STATISTICS_YEAR);
	savedMonth = Read_VP(ELECTRICITY_STATISTICS_MONTH);
	savedDay = Read_VP(ELECTRICITY_STATISTICS_DAY);
	//判断掉电保存的年/月/日与当前是否一致；按优先级处理，避免年变时再重复执行月、日分支
	if(savedYear != system.year)
	{
		lastHourElectricity = dailyElectricityNum[23];
		if (savedDay >= 1 && savedDay <= 31)
		{
			yesterdayElectricity = monthlyElectricityNum[savedDay - 1];
		}
		if (savedMonth >= 1 && savedMonth <= 12)
		{
			lastMonthElectricity = yearlyElectricityNum[savedMonth - 1];
		}
		totalHour = 0;
		totalDay = 0;
		totalMonth = 0;
		memset(dailyElectricityNum, 0, sizeof(dailyElectricityNum));
		memset(monthlyElectricityNum, 0, sizeof(monthlyElectricityNum));
		memset(yearlyElectricityNum, 0, sizeof(yearlyElectricityNum));
		write_dgus_vp(DISP_Electricity_H1, (u8 *)&dailyElectricityNum, 24);
		write_dgus_vp(DISP_Electricity_D1, (u8 *)&monthlyElectricityNum, 31);
		write_dgus_vp(DISP_Electricity_M1, (u8 *)&yearlyElectricityNum, 12);
	}
	else if(savedMonth != system.month)
	{
		//月不同：清空小时与当月日曲线（D1），保留本年每月（M1）
		lastHourElectricity = dailyElectricityNum[23];
		if (savedDay >= 1 && savedDay <= 31)
		{
			yesterdayElectricity = monthlyElectricityNum[savedDay - 1];
		}
		if (savedMonth >= 1 && savedMonth <= 12)
		{
			lastMonthElectricity = yearlyElectricityNum[savedMonth - 1];
		}
		totalHour = 0;
		totalDay = 0;
		totalMonth = 0;
		memset(dailyElectricityNum, 0, sizeof(dailyElectricityNum));
		memset(monthlyElectricityNum, 0, sizeof(monthlyElectricityNum));
		write_dgus_vp(DISP_Electricity_H1, (u8 *)&dailyElectricityNum, 24);
		write_dgus_vp(DISP_Electricity_D1, (u8 *)&monthlyElectricityNum, 31);
	}
	else if(savedDay != system.date)
	{
		//日不同：清空当日小时（H1），并清零当日在月度日曲线中的累计（D1 对应当日槽）
		lastHourElectricity = dailyElectricityNum[23];
		if (savedDay >= 1 && savedDay <= 31)
		{
			yesterdayElectricity = monthlyElectricityNum[savedDay - 1];
		}
		totalDay = 0;
		memset(dailyElectricityNum, 0, sizeof(dailyElectricityNum));
		write_dgus_vp(DISP_Electricity_H1, (u8 *)&dailyElectricityNum, 24);
		write_dgus_vp(DISP_Electricity_D1, (u8 *)&monthlyElectricityNum, 31);
	}
	else
	{
		if (system.hour > 0)
		{
			lastHourElectricity = dailyElectricityNum[system.hour - 1];
		}
		if (system.date > 1)
		{
			yesterdayElectricity = monthlyElectricityNum[system.date - 2];
		}
		if (system.month > 1)
		{
			lastMonthElectricity = yearlyElectricityNum[system.month - 2];
		}
	}

	/* 此时 total* 仍是 VP 旧格式(0.1kWh)，先按旧上限清异常值，再×10 进入内部 0.01kWh */
	if (totalHour > 250)
	{
		totalHour = 0;
		dailyElectricityNum[system.hour] = 0;
		Wite_VP(DISP_Electricity_H1 + system.hour, 0);
	}
	if (totalDay > 6000)
	{
		totalDay = 0;
		monthlyElectricityNum[system.date - 1] = 0;
		Wite_VP(DISP_Electricity_D1 + system.date - 1, 0);
	}
	if (totalMonth > 12000UL)
	{
		totalMonth = 0;
		yearlyElectricityNum[system.month - 1] = 0;
		Wite_VP(DISP_Electricity_M1 + system.month - 1, 0);
	}
	totalHour = (u16)(totalHour * 10U);
	totalDay = (u16)(totalDay * 10U);
	totalMonth = totalMonth * 10UL;

	Send_modebus_command_06(STATE_ELEC_HOUR + PARAMETER_20_START_ADDRESS, dailyElectricityNum[system.hour]);
	Send_modebus_command_06(STATE_ELEC_DAILY + PARAMETER_20_START_ADDRESS, monthlyElectricityNum[system.date-1]);
	Send_modebus_command_06(STATE_ELEC_MON_H + PARAMETER_20_START_ADDRESS, 0);
	Send_modebus_command_06(STATE_ELEC_MON_L + PARAMETER_20_START_ADDRESS, yearlyElectricityNum[system.month-1] / 10);
	Display_Electricity_Curve();
	Display_Electricity_Stats();
	Persist_Elec_Stats_VP();
	//清零sendCache
	memset(sendCache, 0, sizeof(uint16_t) * 20 * 2);
}

// 判断是否为闰年
int isLeapYear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return 1; // 闰年
	return 0;	  // 非闰年
}

// 获取指定月份的天数
int getDaysInMonth(int year, int month)
{
	int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	days[1] = 28 + isLeapYear(year);
	return days[month - 1];
}

// 计算从2000年1月1日到指定日期的天数
int calculateDaysSinceYear2000(int year, int month, int day)
{
	int days = 0;
	int i;

	for (i = 2000; i < year; i++)
	{
		days += 365 + isLeapYear(i);
	}

	for (i = 1; i < month; i++)
	{
		days += getDaysInMonth(year, i);
	}
	days += day;
	// write_dgus_vp(0xF910, (u8 *)&days, 1);
	return days;
}

void clearFaultAferSevenDays(void)
{
	u16 oldyear;
	u16 oldmonth;
	u16 oldday;
	u8 oldstr[16];
	if (system.hour == 1 && system.min == 0)
	{
		if (Read_VP(0x401E))
		{
			read_dgus_vp(0x401F, oldstr, 8);
			oldyear = 2000 + (oldstr[2] - '0') * 10 + (oldstr[3] - '0');
			oldmonth = (oldstr[4] - '0') * 10 + (oldstr[5] - '0');
			oldday = (oldstr[6] - '0') * 10 + (oldstr[7] - '0');
			if ((calculateDaysSinceYear2000(system.year + 2000, system.month, system.date) - calculateDaysSinceYear2000(oldyear, oldmonth, oldday)) >= 7)
			{
				Wite_VP(0x401E, false);
				memset(oldstr, 0, 16);
				write_dgus_vp(0x401F, oldstr, 8);
			}
		}
		if (Read_VP(0x4027))
		{
			read_dgus_vp(0x4028, oldstr, 8);
			oldyear = 2000 + (oldstr[2] - '0') * 10 + (oldstr[3] - '0');
			oldmonth = (oldstr[4] - '0') * 10 + (oldstr[5] - '0');
			oldday = (oldstr[6] - '0') * 10 + (oldstr[7] - '0');
			if ((calculateDaysSinceYear2000(system.year + 2000, system.month, system.date) - calculateDaysSinceYear2000(oldyear, oldmonth, oldday)) >= 7)
			{
				Wite_VP(0x4027, false);
				memset(oldstr, 0, 16);
				write_dgus_vp(0x4028, oldstr, 8);
			}
		}
		if (Read_VP(0x4030))
		{
			read_dgus_vp(0x4031, oldstr, 8);
			oldyear = 2000 + (oldstr[2] - '0') * 10 + (oldstr[3] - '0');
			oldmonth = (oldstr[4] - '0') * 10 + (oldstr[5] - '0');
			oldday = (oldstr[6] - '0') * 10 + (oldstr[7] - '0');
			if ((calculateDaysSinceYear2000(system.year + 2000, system.month, system.date) - calculateDaysSinceYear2000(oldyear, oldmonth, oldday)) >= 7)
			{
				Wite_VP(0x4030, false);
				memset(oldstr, 0, 16);
				write_dgus_vp(0x4031, oldstr, 8);
			}
		}
	}
}

void Wite_VP(u16 addr, u16 vpdata)
{
	write_dgus_vp(addr, (u8 *)&vpdata, 1);
}

u16 Read_VP(u16 addr)
{
	u16 vpdata;
	read_dgus_vp(addr, (u8 *)&vpdata, 1);
	return vpdata;
}

void Brand_Select(void)
{
#if defined(Brand_NEUTRAL)
	brand_message = NEUTRAL;
	Wite_VP(DISP_POWER_ICO, 3); // 映射 3
	Wite_VP(DISP_NET, NEUTRAL);
	Read_dgus_buf[0] = Read_VP(EEPROM_PARA);
	SET_BIT(Read_dgus_buf[0], 4);
	Wite_VP(EEPROM_PARA, Read_dgus_buf[0]);
	Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 1);
	State2_pack[STATE_FLAG_UNLOCK] = 1;
#endif

#if defined(Brand_ENERGY)
	brand_message = ENERGY;
	Wite_VP(DISP_POWER_ICO, ENERGY);
	Wite_VP(DISP_NET, ENERGY);
#endif

#if defined(Brand_XIAOYOU)
	brand_message = XIAOYOU;
	Wite_VP(DISP_POWER_ICO, XIAOYOU);
	Wite_VP(DISP_NET, XIAOYOU);
	Read_dgus_buf[0] = Read_VP(EEPROM_PARA);
	SET_BIT(Read_dgus_buf[0], 4);
	Wite_VP(EEPROM_PARA, Read_dgus_buf[0]);
	Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 1);
	State2_pack[STATE_FLAG_UNLOCK] = 1;
#endif

#if defined(Brand_MSD)
	brand_message = MSD;
	Wite_VP(DISP_POWER_ICO, MSD);
	Wite_VP(DISP_NET, MSD - 1);
	Read_dgus_buf[0] = Read_VP(EEPROM_PARA);
	SET_BIT(Read_dgus_buf[0], 4);
	Wite_VP(EEPROM_PARA, Read_dgus_buf[0]);
	Send_modebus_command_06(STATE_FLAG_UNLOCK + PARAMETER_250_START_ADDRESS, 1);
	State2_pack[STATE_FLAG_UNLOCK] = 1;
#endif
	//	if (brand_message == XIAOYOU || brand_message == NEUTRAL) // Other brands force clear the power-on password
	//	{
	//		Read_dgus_buf[0] = Read_VP(EEPROM_PARA);
	//		SET_BIT(Read_dgus_buf[0], 4);
	//		Wite_VP(EEPROM_PARA, Read_dgus_buf[0]);
	//		T5L_Flash(WRITE, EEPROM_PARA, EEPROM_PARA - EEPROM_PARA, 2);
	//	}
}


u16 segments = 7;      // 显示的矩形段数
void GraphicRendering(u16 inputHeights[7], u16 segments)
{
    u16 DrawAdot_Array[8]; // 用于绘制矩形的数组
    u16 baseX = 250;       // 矩形的初始X坐标
    u16 baseY = 408;       // 矩形的初始Y坐标
    u16 width = 40;        // 矩形的宽度
    u16 spacing = 30;      // 矩形之间的间距
    
    u16 i;

    DrawAdot_Array[0] = 4;       // 4: 矩形填充
    DrawAdot_Array[1] = segments; // 图形个数

    for (i = 0; i < segments; i++)
    {
        DrawAdot_Array[4] = baseX + (i * (width + spacing)); // 左上角X坐标
        DrawAdot_Array[5] = baseY;                          // 左上角Y坐标
        DrawAdot_Array[2] = DrawAdot_Array[4] - width;      // 右下角X坐标
        DrawAdot_Array[3] = DrawAdot_Array[5] - inputHeights[i]; // 右下角Y坐标，使用输入数组控制每段高度
        DrawAdot_Array[6] = 0x2C1C;                    // 颜色
        DrawAdot_Array[7] = 0xFF00;                         // 填充
		if(DrawAdot_Array[3] == 0)
		{
			DrawAdot_Array[3] = 1; // 避免高度为0的情况
		}
        write_dgus_vp((0x8000 + 2) + (i * 5), (u8 *)&DrawAdot_Array[2], 6); // 写入每段矩形数据
    }

    // 写入矩形填充命令和总图形个数
    write_dgus_vp(0x8000, (u8 *)&DrawAdot_Array, 2);
}


// 找出数组的最大值
u16 findMaxValue(u16 arr[], u8 arr_size) {
    u16 max = arr[0]; // 假设第一个元素为最大值
		u8 i;
    for (i = 1; i < arr_size; i++) {
        if (arr[i] > max) {
            max = arr[i]; // 更新最大值
        }
    }
    return max;
}
//mode:0: 24 hours, 1: 31 days, 2: 12 months
//page: 0-6
// 0: 1-7, 1: 8-14, 2: 15-21, 3: 22-24/31/12
void Electricity_Display(u8 mode, u8 page)
{
    u16 dailyElectricity[24] = {1}; 	// 每小时电量数据
    u16 monthlyElectricity[31] = {1};	// 每天电量数据
    u16 yearlyElectricity[12] = {1};	// 每月电量数据
    u16 displayData[68] = {0}; // 用于存储当前页面显示的7段电量数据
    u8 i, maxPages = 0;
	u16 maxValue = 0; // 用于存储当前页面的最大值
	u32 temp;
    // 根据模式计算最大页数
    switch (mode)
    {
    case 0: // 每天24小时
        maxPages = (24 + 6) / 7; // 向上取整
        break;
    case 1: // 每个月31天
        maxPages = (31 + 6) / 7;
        break;
    case 2: // 每年12个月
        maxPages = (12 + 6) / 7;
        break;
    default:
        return; // 无效模式
    }

    // 限制页码范围
    if (page >= maxPages)
        page = maxPages - 1;

    // 根据模式和页面选择数据
    switch (mode)
    {
    case 0: // 每天24小时
		memset(dailyElectricity, 0, sizeof(dailyElectricity));
		read_dgus_vp(DISP_Electricity_H1, (u8 *)&dailyElectricity, 24); // 读取电量数据
		maxValue = 200;//findMaxValue(dailyElectricity, 24); // 找出最大值
        for (i = 0; i < 7; i++)
        {
            if (page * 7 + i < 24)
			{
				if (dailyElectricity[page * 7 + i]>200)
				{
					dailyElectricity[page * 7 + i] = 200; // 限制电量数据最大值为20
				}			
				temp = (unsigned long)dailyElectricity[page * 7 + i] * 265; // 计算电量数据// 提升为32位
                displayData[i] = (u16)(temp / maxValue); // 除法后再转换为16位
				segments = 7;
			}
            else
			{
				displayData[i] = 1; // 超出范围填充
				segments = 3;
			}
        }
        break;
    case 1: // 每个月31天
		memset(monthlyElectricity, 0, sizeof(monthlyElectricity));
		read_dgus_vp(DISP_Electricity_D1, (u8 *)&monthlyElectricity, 31); // 读取电量数据
		maxValue = 4000;//findMaxValue(monthlyElectricity, 31); // 找出最大值
        for (i = 0; i < 7; i++)
        {
            if (page * 7 + i < 31)
			{
				if (monthlyElectricity[page * 7 + i]>4000)
				{
					monthlyElectricity[page * 7 + i] = 4000; // 限制电量数据最大值为500
				}
				temp = (unsigned long)monthlyElectricity[page * 7 + i] * 265; // 计算电量数据// 提升为32位
				displayData[i] = (u16)(temp / maxValue); // 除法后再转换为16位
				segments = 7;
			}
            else
			{
				displayData[i] = 1; // 超出范围填充
				segments = 3;
			}
        }
        break;
    case 2: // 每年12个月
		memset(yearlyElectricity, 0, sizeof(yearlyElectricity));	
		read_dgus_vp(DISP_Electricity_M1, (u8 *)&yearlyElectricity, 12); // 读取电量数据
		maxValue = 60000;//findMaxValue(yearlyElectricity, 12); // 找出最大值
        for (i = 0; i < 7; i++)
        {
            if (page * 7 + i < 12)
			{
				if (yearlyElectricity[page * 7 + i]>60000)
				{
					yearlyElectricity[page * 7 + i] = 60000; // 限制电量数据最大值为60000
				}
				temp = (unsigned long)yearlyElectricity[page * 7 + i] * 265; // 计算电量数据// 提升为32位
				displayData[i] = (u16)(temp / maxValue); // 除法后再转换为16位
				segments = 7;
			}
            else
			{
				displayData[i] = 1; // 超出范围填充0
				segments = 5;
			}
        }
        break;
    }
    // 调用GraphicRendering函数绘制电量图形
    GraphicRendering(displayData,segments);
}
u16 totalHour = 0; // 每小时电量（内部 0.01kWh，超限 2500）
u16 totalDay = 0;  // 每天电量（内部 0.01kWh，超限 60000）
u32 totalMonth = 0; // 每月电量（内部 0.01kWh，超限 120000）
u16 dailyElectricityNum[24] = {0};   // 每小时电量（当日日历）
u16 monthlyElectricityNum[31] = {0}; // 每天电量（当月日历）
u16 yearlyElectricityNum[12] = {0};  // 每月电量（当年日历）
// 近24小时/31天/12个月趋势（仅RAM，不写VP；[0]最旧，末槽为当前周期）
u16 trendHourElectricity[24] = {0};
u16 trendDayElectricity[31] = {0};
u16 trendMonthElectricity[12] = {0};

static void Trend_Shift_Push(u16 *buf, u8 count, u16 value)
{
	u8 i;
	for (i = 0; i < (u8)(count - 1); i++)
	{
		buf[i] = buf[i + 1];
	}
	buf[count - 1] = value;
}

/* 内部 0.01kWh -> 存储/显示/下发 0.1kWh */
static u16 Elec_ToOldU16(u32 v01)
{
	return (u16)(v01 / 10UL);
}

void Clear_Trend_Electricity(void)
{
	memset(trendHourElectricity, 0, sizeof(trendHourElectricity));
	memset(trendDayElectricity, 0, sizeof(trendDayElectricity));
	memset(trendMonthElectricity, 0, sizeof(trendMonthElectricity));
}

static void Write_U32_VP(u16 addr, u32 value)
{
	write_dgus_vp(addr, (u8 *)&value, 2);
}

static void Persist_Elec_Stats_VP(void)
{
	Wite_VP(EEPROM_ELEC_LAST_HOUR, lastHourElectricity);
	Wite_VP(EEPROM_ELEC_YESTERDAY, yesterdayElectricity);
	Wite_VP(EEPROM_ELEC_LAST_MONTH, lastMonthElectricity);
	write_dgus_vp(EEPROM_KWH, (u8 *)&ulNumberEl, 2);
}

void Display_Electricity_Stats(void)
{
	u8 i;
	u32 yearSum = 0;
	u16 lastHour;
	u16 yesterday;
	u16 lastMonth;

	for (i = 0; i < 12; i++)
	{
		yearSum += (u32)yearlyElectricityNum[i];
	}

	if (system.hour > 0)
	{
		lastHour = dailyElectricityNum[system.hour - 1];
	}
	else
	{
		lastHour = lastHourElectricity;
	}
	if (system.date > 1)
	{
		yesterday = monthlyElectricityNum[system.date - 2];
	}
	else
	{
		yesterday = yesterdayElectricity;
	}
	if (system.month > 1)
	{
		lastMonth = yearlyElectricityNum[system.month - 2];
	}
	else
	{
		lastMonth = lastMonthElectricity;
	}

	Write_U32_VP(DISP_ELEC_TOTAL, ulNumberEl);
	Wite_VP(DISP_ELEC_TODAY, Elec_ToOldU16(totalDay));
	Wite_VP(DISP_ELEC_HOUR_NOW, Elec_ToOldU16(totalHour));
	Wite_VP(DISP_ELEC_HOUR_LAST, lastHour);
	Wite_VP(DISP_ELEC_MONTH_NOW, Elec_ToOldU16(totalMonth));
	Wite_VP(DISP_ELEC_YESTERDAY, yesterday);
	Write_U32_VP(DISP_ELEC_YEAR, yearSum);
	Wite_VP(DISP_ELEC_MONTH_LAST, lastMonth);
}

void Clear_Electricity_Stats(void)
{
	totalHour = 0;
	totalDay = 0;
	totalMonth = 0;
	ulNumberEl = 0;
	usElectricity = 0;
	kwhFrac = 0;
	lastHourElectricity = 0;
	yesterdayElectricity = 0;
	lastMonthElectricity = 0;
	memset(dailyElectricityNum, 0, sizeof(dailyElectricityNum));
	memset(monthlyElectricityNum, 0, sizeof(monthlyElectricityNum));
	memset(yearlyElectricityNum, 0, sizeof(yearlyElectricityNum));
	Clear_Trend_Electricity();
	write_dgus_vp(DISP_Electricity_H1, (u8 *)&dailyElectricityNum, 24);
	write_dgus_vp(DISP_Electricity_D1, (u8 *)&monthlyElectricityNum, 31);
	write_dgus_vp(DISP_Electricity_M1, (u8 *)&yearlyElectricityNum, 12);
	Persist_Elec_Stats_VP();
	Display_Electricity_Stats();
	Display_Electricity_Curve();
	/* 清零后立即下发给主控（小时/日/月电量置 0） */
	Send_modebus_command_06(STATE_ELEC_HOUR + PARAMETER_20_START_ADDRESS, 0);
	Send_modebus_command_06(STATE_ELEC_DAILY + PARAMETER_20_START_ADDRESS, 0);
	Send_modebus_command_06(STATE_ELEC_MON_H + PARAMETER_20_START_ADDRESS, 0);
	Send_modebus_command_06(STATE_ELEC_MON_L + PARAMETER_20_START_ADDRESS, 0);
}

void Electricity_process(void)
{
    // static u16 dailyElectricity[24] = {0};   // 每小时电量
    // static u16 monthlyElectricity[31] = {0}; // 每天电量
    // static u16 yearlyElectricity[12] = {0};  // 每月电量

	static u8 saveHourCounter = 0; // 用于记录 6 小时计数
	static u8 hourRolloverDone = 0;
	static u8 dayRolloverDone = 0;
	static u8 monthRolloverDone = 0;
    // 获取当前时间
	u8 currentYear = system.year; // 当前年份
	u8 currentMonth = system.month; // 当前月份
	u8 currentDate = system.date; // 当前日期
	u8 currentHour = system.hour; // 当前小时
	u8 currentMinute = system.min; // 当前分钟
	u8 currentSecond = system.sec; // 当前秒
	static u8 SaveTimeFg = 0;
	u16 electricQuantity = 0;
	if(currentMinute%10 == 9 && currentSecond<=3)
	{
		SaveTimeFg = 1;
	}
	//Wite_VP(0x9000,SaveTimeFg);
	if(SaveTimeFg == 1 && currentSecond >=5)// 每10分钟保存一次电量数据  
	{
		//Wite_VP(0x9001,2);
		SaveTimeFg = 0;
		// 未输入开机密码或通信故障时不统计电量
		if (GET_BIT(Read_dgus_buf[0], 4) && (flag_communication_fault == 0))
		{
			// STATE_KWH 为 0.01kWh；>366 或负值视为异常，本轮按 0 累计
			if ((State_pack[STATE_KWH] < 0) || (State_pack[STATE_KWH] > 366))
			{
				electricQuantity = 0;
			}
			else
			{
				electricQuantity = (u16)State_pack[STATE_KWH];
			}
			// 累计小时电量（内部 0.01kWh）
			totalHour = totalHour+ electricQuantity;
			//Wite_VP(0x9003,totalHour);
			totalDay = totalDay+ electricQuantity;
			totalMonth = totalMonth+ electricQuantity;
			if (ulNumberEl < 0x6FFFFFF0UL)
			{
				ulNumberEl += (u32)(electricQuantity / 10);
				kwhFrac += (u8)(electricQuantity % 10);
				if (kwhFrac >= 10)
				{
					if (ulNumberEl < 0x6FFFFFF0UL)
					{
						ulNumberEl++;
					}
					kwhFrac -= 10;
				}
			}
			if (totalHour > ELEC_HOUR_MAX_001)
			{
				totalHour = 0;
			}
			if (totalDay > ELEC_DAY_MAX_001)
			{
				totalDay = 0;
			}
			if (totalMonth > ELEC_MONTH_MAX_001)
			{
				totalMonth = 0;
			}
			// 累计电量数据到对应数组（数组/VP 仍为 0.1kWh）
			dailyElectricityNum[currentHour] = Elec_ToOldU16(totalHour);
			monthlyElectricityNum[currentDate-1] = Elec_ToOldU16(totalDay);
			yearlyElectricityNum[currentMonth-1] = Elec_ToOldU16(totalMonth);
			// 近周期趋势：末槽跟当前累计，曲线实时可见（0.1kWh 量程）
			trendHourElectricity[23] = Elec_ToOldU16(totalHour);
			trendDayElectricity[30] = Elec_ToOldU16(totalDay);
			trendMonthElectricity[11] = Elec_ToOldU16(totalMonth);
			
			if (Para_pack[PARA_H06_99] == 0)	// WIFI模式下不下发
			{
				Send_modebus_command_06(STATE_ELEC_HOUR + PARAMETER_20_START_ADDRESS, dailyElectricityNum[currentHour]);
				Send_modebus_command_06(STATE_ELEC_DAILY + PARAMETER_20_START_ADDRESS, monthlyElectricityNum[currentDate-1]);
				Send_modebus_command_06(STATE_ELEC_MON_H + PARAMETER_20_START_ADDRESS, 0);
				Send_modebus_command_06(STATE_ELEC_MON_L + PARAMETER_20_START_ADDRESS, yearlyElectricityNum[currentMonth-1] / 10);
			}

			write_dgus_vp(DISP_Electricity_H1,(u8*)&dailyElectricityNum,24);//把每小时电量写入VP中
			write_dgus_vp(DISP_Electricity_D1,(u8*)&monthlyElectricityNum,31);//把每天电量写入VP中
			write_dgus_vp(DISP_Electricity_M1,(u8*)&yearlyElectricityNum,12);//把每月电量写入VP中
			Display_Electricity_Curve();
			Display_Electricity_Stats();
			
			if(saveHourCounter<100)
			{
				saveHourCounter++;
			}
			if (saveHourCounter >= 36)// 每 6 小时保存一次电量数据到 EEPROM 每十分钟累加一次
			{
				saveHourCounter = 0; // 重置计数器
				//write_dgus_vp(ELECTRICITY_STATISTICS_YEAR, (u8 *)&system.year, 1);
				Wite_VP(ELECTRICITY_STATISTICS_YEAR,system.year);
				//write_dgus_vp(ELECTRICITY_STATISTICS_MONTH, (u8 *)&system.month, 1);
				Wite_VP(ELECTRICITY_STATISTICS_MONTH,system.month);
				//write_dgus_vp(ELECTRICITY_STATISTICS_DAY, (u8 *)&system.date, 1);
				Wite_VP(ELECTRICITY_STATISTICS_DAY,system.date);
				Persist_Elec_Stats_VP();
				EEPROM_Write_page(); // 保存电量数据到 EEPROM
			}
		}
	}
	if(currentMinute == 0 && currentSecond<=3)
	{
		if (hourRolloverDone == 0)
		{
			hourRolloverDone = 1;
			// 整点00分：近24小时左移，压入刚结束的一小时，末槽留给新小时
			lastHourElectricity = Elec_ToOldU16(totalHour);
			Trend_Shift_Push(trendHourElectricity, 24, lastHourElectricity);
			trendHourElectricity[23] = 0;
			totalHour = 0;
			Persist_Elec_Stats_VP();
			Display_Electricity_Curve();
			Display_Electricity_Stats();
		}
	}
	else
	{
		hourRolloverDone = 0;
	}
	if(currentHour ==0 && currentMinute == 0 && currentSecond<=3)
	{
		if (dayRolloverDone == 0)
		{
			dayRolloverDone = 1;
			// 跨日：近31天左移，压入刚结束的一天，末槽留给新一天
			yesterdayElectricity = Elec_ToOldU16(totalDay);
			Trend_Shift_Push(trendDayElectricity, 31, yesterdayElectricity);
			trendDayElectricity[30] = 0;
			totalHour = 0;
			totalDay = 0; 
			memset(dailyElectricityNum, 0, sizeof(dailyElectricityNum)); // 清空小时电量数组
			write_dgus_vp(DISP_Electricity_H1,(u8*)&dailyElectricityNum,24);//把每小时电量写入VP中
			Persist_Elec_Stats_VP();
			Display_Electricity_Curve();
			Display_Electricity_Stats();
			EEPROM_Write_page(); // 保存电量数据到 EEPROM
		}
	}
	else
	{
		dayRolloverDone = 0;
	}
	if(currentDate == 1 && currentHour == 0 && currentMinute == 0&& currentSecond<=3)
	{
		if (monthRolloverDone == 0)
		{
			monthRolloverDone = 1;
			// 跨月：近12个月左移，压入刚结束的一月，末槽留给新一月
			lastMonthElectricity = Elec_ToOldU16(totalMonth);
			Trend_Shift_Push(trendMonthElectricity, 12, lastMonthElectricity);
			trendMonthElectricity[11] = 0;
			totalDay = 0;
			totalMonth = 0; 
			memset(monthlyElectricityNum, 0, sizeof(monthlyElectricityNum)); // 清空天电量数组
			write_dgus_vp(DISP_Electricity_D1,(u8*)&monthlyElectricityNum,31);//把每天电量写入VP中
			Persist_Elec_Stats_VP();
			Display_Electricity_Curve();
			Display_Electricity_Stats();
			EEPROM_Write_page(); // 保存电量数据到 EEPROM
		}
	}
	else if (!(currentDate == 1 && currentHour == 0 && currentMinute == 0))
	{
		monthRolloverDone = 0;
	}	
	if(currentMonth == 1 && currentDate == 1 && currentHour == 0 && currentMinute == 0&& currentSecond<=3)
	{
		totalMonth = 0; // 重置年电量
		memset(yearlyElectricityNum, 0, sizeof(yearlyElectricityNum)); // 清空月电量数组
		write_dgus_vp(DISP_Electricity_M1,(u8*)&yearlyElectricityNum,12);//把每月电量写入VP中
		Persist_Elec_Stats_VP();
		Display_Electricity_Curve();
		Display_Electricity_Stats();
		EEPROM_Write_page(); // 保存电量数据到 EEPROM
	}
}


//2047 
void WriteTemperatureToVPBuffer(void)
{
    uint16_t outWaterTemp = 0; // outlet water temperature
    uint16_t inWaterTemp = 0;  // inlet water temperature
	uint16_t outdoorTempe = 0; // outdoor temperature
	uint16_t indoorTempe = 0;  // indoor temperature
	uint8_t initArray[] = {0x5A, 0xA5, 0x04, 0x00, 0x00, 0x01, 0x03, 0xE8, 0x01, 0x01, 0x03, 0xE8,0x02, 0x01, 0x03, 0xE8, 0x03, 0x01, 0x03, 0xE8};
    outWaterTemp = Read_VP(0x503A)+400; // read the outlet water temperature from address 0x503a and convert the temperature to a positive number
    inWaterTemp =  Read_VP(0x5032)+400;  // read the inlet water temperature from address 0x5032 and convert the temperature to a positive number
	outdoorTempe = Read_VP(0x5037)+400; // read the outdoor temperature from address 0x5037
	indoorTempe =  Read_VP(0x503E)+400;  // read the indoor temperature from address 0x503e
    // 将出水温度写入 initArray[6] 和 initArray[7]
    initArray[6] = (uint8_t)((outWaterTemp >> 8) & 0xFF); // 高字节
    initArray[7] = (uint8_t)(outWaterTemp & 0xFF);        // 低字节
    // 将进水温度写入 initArray[10] 和 initArray[11]
    initArray[10] = (uint8_t)((inWaterTemp >> 8) & 0xFF); // 高字节
    initArray[11] = (uint8_t)(inWaterTemp & 0xFF);        // 低字节
	//将室外温度写入 initArray[14] 和 initArray[15]
	initArray[14] = (uint8_t)((outdoorTempe >> 8) & 0xFF); // 高字节
	initArray[15] = (uint8_t)(outdoorTempe & 0xFF);        // 低字节
	//将室内温度写入 initArray[18] 和 initArray[19]
	initArray[18] = (uint8_t)((indoorTempe >> 8) & 0xFF); // 高字节
	initArray[19] = (uint8_t)(indoorTempe & 0xFF);        // 低字节
	write_dgus_vp(0x0310, (u8 *)&initArray, sizeof(initArray)); // 发送初始化数据到 VP 缓冲区	
}

// CH4固定点写入：先清指针/长度再整表写入，避免追加成趋势滚动
void Write_CH4_Electricity_Curve(u16 *Eledata, u8 count)
{
	u8 i;
	u8 t;
	u16 curveCmd;
	static u8 curveBuf[70]; // 最多31点：3字帧头 + 31点

	if (Eledata == 0 || count == 0 || count > 31)
	{
		return;
	}

	for (t = 0; t < 20; t++)
	{
		read_dgus_vp(0x0310, (u8 *)&curveCmd, 1);
		if (curveCmd != 0x5AA5)
		{
			break;
		}
	}

	Wite_VP(0x0308, 0); // CH4存储指针回到槽0
	Wite_VP(0x0309, 0); // CH4有效长度清0

	curveBuf[0] = 0x5A;
	curveBuf[1] = 0xA5;
	curveBuf[2] = 0x01;
	curveBuf[3] = 0x00;
	curveBuf[4] = 0x04; // 通道04
	curveBuf[5] = count;
	for (i = 0; i < count; i++)
	{
		curveBuf[6 + i * 2] = (u8)(Eledata[i] >> 8);
		curveBuf[7 + i * 2] = (u8)(Eledata[i] & 0xFF);
	}
	write_dgus_vp(0x0310, curveBuf, (u16)(3 + count)); // 帧头3字 + N点
}

/* 页201测试：24点同时覆盖“显示正常”和“超范围”
 * 量程按 0~250（0~25 kWh）。测完后改回 Write_CH4_Electricity_Curve(dailyElectricityNum, 24);
 *
 * 小时  值     期望
 * 0~10  0→250  左下到贴顶的斜线（正常铺满）
 * 11    250    贴顶（满量程）
 * 12    251    刚超，应被裁在顶或略冲出
 * 13    300    超约 20%
 * 14    500    2倍量程
 * 15    1000   4倍量程
 * 16    0xFFFF 无符号最大（若当有符号会变负/贴底）
 * 17    0      超量程后应能回到贴底
 * 18    125    半高（12.5 kWh，应对 VD_Central）
 * 19    250    再贴顶
 * 20    0      贴底
 * 21    375    超 50%
 * 22    125    半高
 * 23    250    右端贴顶
 */
void Test_CH4_Hour_Curve_Display(void)
{
	static u16 testData[24];
	u8 i;

	for (i = 0; i < 11; i++)
	{
		testData[i] = (u16)(((u32)i * 250UL) / 10UL);
	}

	testData[11] = 250;
	testData[12] = 251;
	testData[13] = 300;
	testData[14] = 500;
	testData[15] = 1000;
	testData[16] = 0xFFFF;
	testData[17] = 0;
	testData[18] = 125;
	testData[19] = 250;
	testData[20] = 0;
	testData[21] = 375;
	testData[22] = 125;
	testData[23] = 250;

	Write_CH4_Electricity_Curve(testData, 24);
}

/* 页203测试：31点同时覆盖“显示正常”和“超范围”
 * 量程按 0~6000（0~600 kWh，与 totalDay>6000 清零、刻度0~600一致）。
 * 测完后改回 Write_CH4_Electricity_Curve(monthlyElectricityNum, 31);
 *
 * 日(0起)  值      期望
 * 0~14     0→6000  左下到贴顶的斜线（正常铺满）
 * 15       6000    贴顶（满量程）
 * 16       6001    刚超，应被裁在顶或略冲出
 * 17       7200    超约 20%
 * 18       12000   2倍量程
 * 19       24000   4倍量程
 * 20       0xFFFF  无符号最大（若当有符号会变负/贴底）
 * 21       0       超量程后应能回到贴底
 * 22       3000    半高（300 kWh，应对 VD_Central）
 * 23       6000    再贴顶
 * 24       0       贴底
 * 25       9000    超 50%
 * 26       3000    半高
 * 27       6000    贴顶
 * 28       0       贴底
 * 29       3000    半高
 * 30       6000    右端贴顶
 */
void Test_CH4_Day_Curve_Display(void)
{
	static u16 testData[31];
	u8 i;

	for (i = 0; i < 15; i++)
	{
		testData[i] = (u16)(((u32)i * 6000UL) / 14UL);
	}

	testData[15] = 6000;
	testData[16] = 6001;
	testData[17] = 7200;
	testData[18] = 12000;
	testData[19] = 24000;
	testData[20] = 0xFFFF;
	testData[21] = 0;
	testData[22] = 3000;
	testData[23] = 6000;
	testData[24] = 0;
	testData[25] = 9000;
	testData[26] = 3000;
	testData[27] = 6000;
	testData[28] = 0;
	testData[29] = 3000;
	testData[30] = 6000;

	Write_CH4_Electricity_Curve(testData, 31);
}

/* 页206测试：12点同时覆盖“显示正常”和“超范围”
 * 量程按 0~12000（与 totalMonth>12000 清零、刻度0~12000一致）。
 * 测完后改回 Write_CH4_Electricity_Curve(yearlyElectricityNum, 12);
 *
 * 月(0起)  值       期望
 * 0~5      0→12000  左下到贴顶的斜线（正常铺满）
 * 6        12000    贴顶（满量程）
 * 7        12001    刚超，应被裁在顶或略冲出
 * 8        24000    2倍量程
 * 9        0xFFFF   无符号最大（若当有符号会变负/贴底）
 * 10       0        超量程后应能回到贴底
 * 11       6000     半高（应对 VD_Central=6000）
 */
void Test_CH4_Month_Curve_Display(void)
{
	static u16 testData[12];
	u8 i;

	for (i = 0; i < 6; i++)
	{
		testData[i] = (u16)(((u32)i * 12000UL) / 5UL);
	}

	testData[6] = 12000;
	testData[7] = 12001;
	testData[8] = 24000;
	testData[9] = 0xFFFF;
	testData[10] = 0;
	testData[11] = 6000;

	Write_CH4_Electricity_Curve(testData, 12);
}

// 按当前页面刷新电量曲线（共用CH4）
// 趋势(RAM)：198近24小时  199近31天  200近12个月
// 日历(VP)：201当天24小时  203当月31天  206/207当年12个月
void Display_Electricity_Curve(void)
{
	u16 page = read_PIC_Now();

	if (page == 198)
	{
		Write_CH4_Electricity_Curve(trendHourElectricity, 24);
	}
	else if (page == 199)
	{
		Write_CH4_Electricity_Curve(trendDayElectricity, 31);
	}
	else if (page == 200)
	{
		Write_CH4_Electricity_Curve(trendMonthElectricity, 12);
	}
	else if (page == 201)
	{
		Write_CH4_Electricity_Curve(dailyElectricityNum, 24);
	}
	else if (page == 203)
	{
		Write_CH4_Electricity_Curve(monthlyElectricityNum, 31);
	}
	else if (page == 206)
	{
		Write_CH4_Electricity_Curve(yearlyElectricityNum, 12);
	}
}

// 室温控制处理函数 room temperature control processing function
void IndoorTempControl(void)
{
   const uint16_t INDOOR = 1; // indoor temperature control
   uint16_t  rdMode, rdHeatSet, rdCoolSet, rdIndoorTemp, rdIndoorSet, rdPower, rdIndoorDiff, rdCoolMin, rdHeatMax;

   // 读取相关参数
   if (Read_VP(TEMPERATURE_CONTROL) == INDOOR)
   {
       //controlTemp = Read_VP(eeprom_IndoorSet);
       rdMode = Para_pack[PARA_MODE];
       rdHeatSet = Para_pack[PARA_HEATING_TEMP];
       rdCoolSet = Para_pack[PARA_COOLING_TEMP];
       rdIndoorTemp = (int16_t)Read_VP(0x5000+62);
       rdIndoorSet = (int16_t)Read_VP(INDOOR_TEMP_SET);
       rdPower = Para_pack[PARA_POWER_ON];
       rdIndoorDiff = Read_VP(CONTROL_HYSTERESIS);
       rdCoolMin = Para_pack[PARA_COOLING_MIN];
       rdHeatMax = Para_pack[PARA_HEATING_MAX];

       //rdIndoorSet *= 10; // 转换为实际温度值

       // adjust the set values for heating or cooling according to the mode
       if (rdMode == HEATING && rdHeatSet != rdHeatMax)
       {
           Send_modebus_command_06(PARA_HEATING_TEMP+PARAMETER_92_START_ADDRESS,rdHeatMax);
       }
       else if (rdMode == COOLING && rdCoolSet != rdCoolMin)
       {
           Send_modebus_command_06(PARA_COOLING_TEMP+PARAMETER_92_START_ADDRESS,rdCoolMin);
       }
			rdIndoorTemp = rdIndoorTemp / 10;
       // startup logic
       if (Read_VP(INDOOR_POWER) == 1) // 开机
       {
           if (rdMode == HEATING)
           {
               if (rdIndoorTemp >= rdIndoorSet && rdPower == 1)
               {
					Send_modebus_command_06(PARA_POWER_ON+PARAMETER_92_START_ADDRESS,0);// 关机
               }
               else if (rdIndoorTemp < (rdIndoorSet - rdIndoorDiff) && rdPower == 0)
               {
				   Send_modebus_command_06(PARA_POWER_ON+PARAMETER_92_START_ADDRESS,1);// 开机
               }
           }
           else if (rdMode == COOLING)
           {
               if (rdIndoorTemp <= rdIndoorSet && rdPower == 1)
               {
                   Send_modebus_command_06(PARA_POWER_ON+PARAMETER_92_START_ADDRESS,0);// 关机
               }
               else if (rdIndoorTemp > (rdIndoorSet + rdIndoorDiff) && rdPower == 0)
               {
				Send_modebus_command_06(PARA_POWER_ON+PARAMETER_92_START_ADDRESS,1);// 开机
               }
           }
       }
       else // when the room temperature control is off force the machine to shut down
       {
           if (rdPower == 1)
           {
			Send_modebus_command_06(PARA_POWER_ON+PARAMETER_92_START_ADDRESS,0);// 关机
           }
       }
   }
}

//静音与室温图标显示处理函数 mute and room temperature icon display processing function
void Display_Mute_Room_Temp_ICO(void)
{
	uint16_t muteRoomTempICO = 0;
	uint16_t temperatureControl = 0;
	uint16_t muteEnable = 0;
	uint16_t muteSwitch = 0;
	muteEnable = Read_VP(0x50AE);
	temperatureControl = Read_VP(TEMPERATURE_CONTROL);
	muteSwitch = Read_VP(0x50AF);
	if(muteEnable == 1)
	{
		if(temperatureControl == 0 && muteSwitch == 0)
		{
			muteRoomTempICO = 2;// 室温控制关闭，静音功能关闭
		}
		else if(temperatureControl == 0 && muteSwitch == 1)
		{
			muteRoomTempICO = 3;// 室温控制关闭，静音功能开启
		}
		else if(temperatureControl == 1 && muteSwitch == 0)
		{
			muteRoomTempICO = 4;// 室温控制开启，静音功能关闭
		}
		else if(temperatureControl == 1 && muteSwitch == 1)
		{
			muteRoomTempICO = 5;// 室温控制开启，静音功能开启
		}
	}
	else
	{
		
		if(temperatureControl == 0)
		{
			muteRoomTempICO = 0;
		}
		else
		{
			muteRoomTempICO = 1;
		}
	}
	write_dgus_vp(DISP_MUTE_ROOM_TEMP_ICO, (u8 *)&muteRoomTempICO, 1);

	//主界面静音图标显示 考虑静音功能&静音开关
	if(muteEnable == 1 && muteSwitch == 1)
	{
		muteRoomTempICO = 1;
	}
	else 
	{
		muteRoomTempICO = 0;
	}
	write_dgus_vp(DISP_MUTE_ICO, (u8 *)&muteRoomTempICO, 1);
}

//发送缓存写入函数,定义一个数组,长度为20,用于存储发送缓存
//当发送缓存写入时,将发送的Send_modebus_command_06的地址和数据缓存写入到数组中,数组内不为零的元素写入需要下发的数据
void Send_Cache_Write(uint16_t address, uint16_t send_data)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
	{
		if(sendCache[i][0] == 0)
		{
			sendCache[i][0] = address;
			sendCache[i][1] = send_data;
			break;
		}
	}
}

//发送缓存处理函数
//将sendCache下发给主控板,然后清零已下发的数据
void Send_Cache_Process(void)
{
	uint8_t i = 0;
	for(i = 0; i < 20; i++)
	{
		if(sendCache[i][0] != 0)
		{
			Send_modebus_command_06(sendCache[i][0], sendCache[i][1]);
			sendCache[i][0] = 0;
			sendCache[i][1] = 0;
		}
	}
}