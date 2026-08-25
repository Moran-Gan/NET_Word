#ifndef _user_global_h_
#define _user_global_h_

#define Uart0 0
#define Uart1 1
#define Uart2 2
#define Uart3 3
#define Uart4 4
#define Uart5 5

#define TYPE_DATA_1 1
#define TYPE_DATA_10 2
#define TEMP_1 3
#define TEMP_10 4
#define C_MODE 0
#define F_MODE 1
#define NULL_MODE 3

#define STATUS 0
#define PARA 1
#define EEPROM 2
#define KEY 3

#define XKQ_VERSION_ADRESS 18

#define OFF 0
#define NO 1
#define CHECK 0x7A11

#define READ_PARA 0X5A
#define NO_READ_PARA 0XA5
// #define						UNIT_TEMP												Para_pack[PARA_N04_1027]
// #define						NULL														0

#define ANALYZE_FUNCTION_06_LENGTH 8
#define MASTER_03_COMMAND 0x03
#define MASTER_06_COMMAND 0x06
#define MASTER_10_COMMAND 0x10
#define MASTER_03_COMMAND_CRC_LENGTH 6
#define MASTER_06_COMMAND_CRC_LENGTH 6

#define SLAVE_ADRESS 0x01
#define MODBUS_PACK 100

#define PARAMETER_1000_START_ADDRESS 1000
#define PARAMETER_2000_START_ADDRESS 2000
// #define 					PARAMETER_1001_START_ADDRESS  	1001
// #define 					PARAMETER_1101_START_ADDRESS  	1101
// #define 					PARAMETER_1201_START_ADDRESS  	1201
// #define 					PARAMETER_1301_START_ADDRESS  	1301
// #define 					PARAMETER_1401_START_ADDRESS  	1401
// #define 					PARAMETER_1501_START_ADDRESS  	1501
// #define 					PARAMETER_1601_START_ADDRESS  	1601
// #define 					STATE_2001_START_ADDRESS  	 		2001

// #define 					PARAMETER_1000_START_ADDRESS  	1000
// #define 					PARAMETER_1091_START_ADDRESS  	1091
// #define 					PARAMETER_1181_START_ADDRESS  	1181
// #define 					PARAMETER_1271_START_ADDRESS  	1271
// #define 					PARAMETER_1361_START_ADDRESS  	1361
// #define 					PARAMETER_1451_START_ADDRESS  	1451
// #define 					PARAMETER_1541_START_ADDRESS  	1541
// #define 					PARAMETER_2000_START_ADDRESS  	2000

#define MODBUS_81_PACK 81
#define PARAMETER_92_START_ADDRESS 92
#define MODBUS_72_PACK 72
#define PARAMETER_20_START_ADDRESS 20
#define MODBUS_33_PACK 33
#define MODBUS_06_PACK 6
#define PARAMETER_200_START_ADDRESS 200
#define PARAMETER_250_START_ADDRESS 250
#define PARAMETER_174_START_ADDRESS 174
#define MODBUS_20_PACK 20
#define PARAMETER_PACK_NUM 120
#define STATE_PACK_NUM 100

#define MODBUS_RXBUFF 250
#define MODBUS_TXBUFF 250
#define Connect_RXBUFF 256
#define Connect_TXBUFF 256
#define MODBUS_BUFF_06 20

#define OFF 0
#define ON 1

#define READ 0X5A
#define WRITE 0XA5

#define TIME_HEAT 0
#define TIME_COOL 1
#define TIME_WATER 2

#define HEATING 2
#define COOLING 1
#define AUTO 0

#define MODE_POWER_OFF 0
#define MODE_HEATING 1
#define MODE_HEATING_ECO 2
#define MODE_COOLING 3
#define MODE_COOLING_ECO 4

#define ECO 2

#define STATE_WATER 1
#define STATE_HEATING 2
#define STATE_COOLING 3
#define STATE_Recover 6
#define STATE_Defrost 7
#define STATE_Preheat 8

#define LEVEL0 0
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3
// #define						STATE_2024											23
// #define						STATE_T01_2041									40
// #define						STATE_T02_2042									41
// #define						STATE_T03_2043									42
// #define						STATE_T08_2048									47
// #define						STATE_T10_2053									52

// #define						PARA_ROOM_TEMP									1
#define PARA_POWER_ON (92 - 92)
#define PARA_MODE (93 - 92)
#define PARA_LOCK (94 - 92)
#define PARA_RESET (95 - 92)
#define PARA_RESET_FAULT (96 - 92)
#define PARA_H04_97 (97 - 92)
#define PARA_H06_99 (99 - 92)
#define PARA_H07_100 (100 - 92)
#define PARA_H08_101 (101 - 92)
#define PARA_HEATING_TEMP (105 - 92)
#define PARA_COOLING_TEMP (106 - 92)
#define PARA_WATER_TEMP (107 - 92)
#define PARA_HEATING_MAX (114 - 92)
#define PARA_COOLING_MIN (115 - 92)
#define PARA_WATER_MAX (116 - 92)
#define PARA_DEF (119 - 92)
#define PARA_F5_122 (109 - 92)
#define PARA_F18_122 (122 - 92)
#define PARA_F19_123 (123 - 92)
#define PARA_F24_128 (128 - 92)
#define PARA_MUTE (131 - 92)
#define PARA_F32_136 (136 - 92)
#define PARA_MUTE_ENABLE (174 - 92)
#define PARA_TIMER 7

#define STATE_PARA (70 - 20)

#define STATE_Load_2 2

#define STATE_POWER_ON 15

#define STATE_SWITCH (20 - 20)
#define STATE_E_SERIES_00 (25 - 20)
#define STATE_E_SERIES_01 (26 - 20)
#define STATE_E_SERIES_02 (27 - 20)
#define STATE_E_SERIES_03 (28 - 20)
#define STATE_E_SERIES_04 (29 - 20)
#define STATE_E_SERIES_05 (30 - 20)
#define STATE_E_SERIES_06 (31 - 20)
#define STATE_E_SERIES_07 (32 - 20)
#define STATE_E_SERIES_08 (33 - 20)
#define STATE_E_SERIES_09 (34 - 20)
#define STATE_E_SERIES_10 (35 - 20)
#define STATE_E_SERIES_11 (36 - 20)
#define STATE_SYS_STARE1 (41 - 20)
#define STATE_SYS_STARE2 (42 - 20)
#define STATE_E_SERIES_12 (44 - 20)
#define STATE_POWER (45 - 20)
#define STATE_MODE (46 - 20)
#define STATE_TEMP_OUT (58 - 20)
#define STATE_TEMP_IN (50 - 20)
#define STATE_TEMP_INDOOR (62 - 20)
#define STATE_TEMP_DHT83 (63 - 20)
#define STATE_KWH (71 - 20)
#define STATE_XKQ_VERSION (85 - 20)
#define STATE_ELEC_HOUR (88 - 20)
#define STATE_ELEC_DAILY (89 - 20)
#define STATE_ELEC_MON_H (90 - 20)
#define STATE_ELEC_MON_L (91 - 20)

#define STATE_NETWORK_STATUS (221 - 200)
#define STATE_LONGITUDE_H (229 - 200)
#define STATE_LONGITUDE_L (230 - 200)
#define STATE_LATITUDE_H (231 - 200)
#define STATE_LATITUDE_L (232 - 200)
#define STATE_4G_RESET (250 - 250)
#define STATE_GET_LOCATION (251 - 250)
#define STATE_ENABLE_UNLOCK (253 - 250)
#define STATE_FLAG_UNLOCK (254 - 250)
#define STATE_LOCATION_STATUS (255 - 250)

// #define						TEMP_CURVE_1										0X1000		//��һ������
// #define						TEMP_CURVE_2										0X1800		//�ڶ�������
// #define						TEMP_CURVE_3										0X2000		//����������
// #define						TEMP_CURVE_4										0X2800		//����������

#define EEPROM_LENGTH 406 //

#define MAGIC_NUMBER 0X4000
#define EEPROM_PARA 0X4000			  //	BIT0:����������BIT1:����������BIT2�澯����,BIT3�Ƿ�ʹ�ù��̼����룬BIT4�Ƿ��Ѿ�������
#define EEPROM_TIMER_MUTE 0X4001	  //	��ʱ����ռ��7����ַ��3*2+1
#define EEPROM_TIMER_POWER 0X4008	  //	��ʱ����ռ��7����ַ��3*2+1
#define EEPROM_TIMER_TEMP 0X400F	  //	��ʱ����ռ��12����ַ��3*3+1+2 ������2��Ϊ�����ʱ����ǰĿ���¶�
#define EEPROM_VACATION 0x401B		  //	离家温度3个地址
#define EEPROM_FAULT 0x401E			  //	��ʷ����ռ�õ�ַ270����ַ:	30*9
#define EEPROM_TEMP 0x412C			  //	 ��ʾ����ˮ�¶ȱ�־λ
#define EEPROM_KWH 0x412E			  //	 ����4λ����ͳ��
#define EEPROM_Connect_UP_TIME 0x4131 //	 �����ϴ����ʱ��
#define EEPROM_Brand 0x4132
#define EEPROM_CHECK 0x4134	
#define DISP_Electricity_H1 0x4136//
#define DISP_Electricity_D1 0x414E//
#define DISP_Electricity_M1 0x416D//
#define TEMPERATURE_CONTROL 0x417A//temperature control page turning jump
#define CONTROL_HYSTERESIS 0x417B//temperature control hysteresis
#define INDOOR_TEMP_SET 0x417C//indoor temperature setting
#define INDOOR_POWER 0x417D

#define INDOOR_TEMP_COMPENSATION 0x4181//室内温度补偿值
#define INDOOR_HUMIDITY_COMPENSATION 0x4182//室内湿度补偿值
#define TIMER_MUTE_ENABLE 0x4183//定时静音使能
#define TIMER_MUTE_START_TIME1 0x4184//定时1静音开 时-分
#define TIMER_MUTE_START_TIME2 0x4185//定时2静音开 时-分
#define TIMER_MUTE_START_TIME3 0x4186//定时3静音关 时-分
#define TIMER_MUTE_END_TIME1 0x4187//定时1静音关 时-分
#define TIMER_MUTE_END_TIME2 0x4188//定时2静音关 时-分
#define TIMER_MUTE_END_TIME3 0x4189//定时3静音关 时-分
#define EEPROM_ELEC_LAST_HOUR 0x418A //上一小时用电掉电保存
#define EEPROM_ELEC_YESTERDAY 0x418B //昨日用电掉电保存
#define EEPROM_ELEC_LAST_MONTH 0x418C //上月用电掉电保存
#define ELECTRICITY_STATISTICS_YEAR 0x4190//电量统计掉电 年
#define ELECTRICITY_STATISTICS_MONTH 0x4191//电量统计掉电 月
#define ELECTRICITY_STATISTICS_DAY 0x4192//电量统计掉电 日
#define EEPROM_CRC (EEPROM_LENGTH - 1)

//
// #define						EEPROM_TIMER_MUTE								0X4001		//	��ʱ����ռ��25����ַ��3*8+1
// #define						EEPROM_TIMER_POWER							0X401A		//	��ʱ����ռ��25����ַ��3*8+1
// #define						EEPROM_TIMER_TEMP								0X4033		//	��ʱ����ռ��22����ַ��4*4+1 + 5������2��Ϊ�����ʱ����ǰĿ���¶���ģʽ
// #define						EEPROM_TIMER_Steri							0X4049		//	��ʱɱ��ռ�õ�ַ2����ַ
// #define						EEPROM_TIMER_MAIN_PUMP					0X404C		//	��ʱ��ѭ��ˮ��ռ�õ�ַ9����ַ
// #define						EEPROM_TIMER_PUMP								0X4055		//	��ʱ�»�ˮ��ռ�õ�ַ9����ַ
// #define						EEPROM_TIMER_VACATION						0X405E		//	����ģʽռ�õ�ַ6����ַ
// #define						EEPROM_FAULT										0x4064		//	��ʷ����ռ�õ�ַ434����ַ
// #define						EEPROM_CURVE										0X4216		//  ����ռ�õ�ַ508

// #define						SET_POWER_A_TEMP								0X4FFF		//A���趨Ŀ���¶�
#define TOUCH_EVENT_FLAG 0X5000 // �׵�ַ

#define MODBUS_IMEI 0x50CB // 16���ֽ�
#define MODBUS_CCID 0x50D3 // 21���ֽ�

#define IMEI 0x7E00
#define CCID 0x7E11
#define KEY_PASSWORD_1 0X5F00 // ���뿪������
#define KEY_PASSWORD_2 0X5F02 // �߼��������롢SOP����
#define KEY_PASSWORD_3 0X5F04 // �ͻ���������
#define KEY_PASSWORD_4 0X5F06 // ��ʷ��������
#define KEY_PASSWORD_5 0X5F08 // �ָ�������������
#define KEY_SET_RTC 0x5F0A	  // ʱ���趨
// #define						KEY_TEMP1_TEMP									0x5F0C
// #define						KEY_TEMP2_TEMP									0x5F0D
// #define						KEY_TEMP3_TEMP									0x5F0E
// #define						KEY_TEMP4_TEMP									0x5F0F

#define DISP_POWER_ICO 0x7000	// ����ͼ�궯̬��ʾ
#define DISP_WIFI_ICO 0x7002	// WiFiͼ����ʾ
#define DISP_RTC 0x7004			// ʱ��ռ7����ַ �� �� �� ʱ �� �� ����
#define DISP_FAULT_ICO 0x700B	// ͼ��ͼ��
#define DISP_Fault_VALUE 0x700D // �����ʾ18��ʵʱ����
#define DISP_TIMER_ICO 0x701F	// ��ʱͼ��
#define DISP_TIP_ICO 0x7020		// ������ʾ
#define DISP_TEMP_MAX 0x7021	// �¶ȴ�����
#define DISP_TEMP_MIN 0x7022	// �¶�С����
#define DISP_STATE_MODE 0X7023	// ״̬ģʽ
#define DISP_TIME_TIMER 0X7024	// ʱ�䵹��ʱ
#define DISP_NET 0x7025			// ��ʾŦ��̩


#define DISP_HEATING_WATER 0X7027
#define DISP_COOLING_WATER 0X7028
#define DIPS_INIT_MODGUS_PARA 0X7029
#define DISP_SET_TEMP 0X702A
#define DISP_READ_ICO 0X702C // 2���ֽ�
#define DISP_Wire_Cont_Hardware 0x702E
#define DISP_Wire_Cont_Software 0X7038

#define DISP_test_wifi1 0X7050
#define DISP_test_wifi2 0X7051
#define DISP_test_UART 0x7052
#define DISP_KWH 0x7053
#define DISP_linkage_ico 0x7054	   // ��������
#define DISP_mode_ico 0x7057	   // ��������
#define DISP_wifi_dbm_state 0x7058 // WiFi�ź�ǿ��
#define DISP_4G_ICO 0x7059		   // WiFiͼ����ʾ
#define DISP_Tset_WiFi 0x705B	   // WiFi���ԣ�BIT0

#define DISP_DeviceName 0X706A
#define DISP_MAC 0x7081
#define DISP_SSID 0x708A
#define DISP_BSSID 0x709A
#define DISP_Signal_strength 0x70A3

#define DISP_brand 0x70A4
#define DISP_Level1 0x70A5

#define DISP_POWER 0x70A6 //on off display in main interface

#define ELECTRICITY_PAGE 0x70A7//power statistics page turning jump

#define R1_TEMP  0x70A8
#define R2_TEMP  0x70A9
#define TEMP_REAL  0x70AA
#define	NETWORK_STATUS_ICON 0x70AB
#define DHT83_REAL  0x70AC
#define DISP_PWD_TIPS		0X70AD
#define INTERFACE_TEMP	0X70AE //关怀模式下显示出水/室温
#define DISP_MUTE_ROOM_TEMP_ICO 0x70AF//静音与室温图标地址,决定是否显示
#define DISP_MUTE_ICO 0x70B0//静音图标地址 0x70B0
/* 电量统计显示：0x70B1~0x70BA 为空闲显示区。总用电/本年用电为32位(高字在前) */
/* 内部累计单位 0.01kWh；VP/显示/下发/上报仍为 0.1kWh（写出/10，月下发再/10） */
#define ELEC_HOUR_MAX_001  2500U
#define ELEC_DAY_MAX_001   60000U
#define ELEC_MONTH_MAX_001 120000UL
#define DISP_ELEC_TOTAL 0x70B1		//总用电 32位 占0x70B1~0x70B2
#define DISP_ELEC_TODAY 0x70B3		//今日用电
#define DISP_ELEC_HOUR_NOW 0x70B4	//当前小时用电
#define DISP_ELEC_HOUR_LAST 0x70B5	//上一小时用电
#define DISP_ELEC_MONTH_NOW 0x70B6	//本月用电
#define DISP_ELEC_YESTERDAY 0x70B7	//昨日用电
#define DISP_ELEC_YEAR 0x70B8		//本年用电 32位 占0x70B8~0x70B9
#define DISP_ELEC_MONTH_LAST 0x70BA	//上月用电
#define DISP_ELEC_ENABLE   0x70BB	/* 电量点位启用：1~67 对应 0x7100~0x7142 */

/* 电量点位隐藏：写 0xFF00 则对应数据在屏幕上不显示。共 67 字：24小时+31天+12个月 */
#define ELEC_HIDE_VALUE    0xFF00
#define ELEC_SHOW_VALUE    1
#define DISP_ELEC_HIDE_H1  0x7100	/* 24小时隐藏 0x7100~0x7117 */
#define DISP_ELEC_HIDE_D1  0x7118	/* 31天隐藏   0x7118~0x7136 */
#define DISP_ELEC_HIDE_M1  0x7137	/* 12个月隐藏 0x7137~0x7142 */
#define ELEC_HIDE_COUNT    67		/* 24+31+12 */



#define MSD 4
#define XIAOYOU 2
#define ENERGY 1
#define NEUTRAL 0
// #define						DISP_STATE_ICO									0x701F					//״̬����
// #define						DISP_HEATING_ICO								0x7020					//����ͼ��

// #define						DISP_POWER_A_TEMP								0x7024					//A���¶���ʾ
// #define						DISP_POWER_A_MODE_END						0x7024					//A��ĩ����ʾ
// #define						DISP_POWER_A_MODE_END						0x7024					//A��ĩ����ʾ
// #define						DISP_POWER_A_MODE								0x7025					//A��ģ����ʾ

// #define						DISP_POWER_B_MODE								0x7027					//B��ģʽ��ʾ

// #define						DISP_POWER_WATER_MODE						0x70E3					//��ˮ

//
// #define						DISP_VACATION_TEMP1							0x702A					//
// #define						DISP_VACATION_TEMP2							0x702B
// #define						DISP_VACATION_TEMP3							0x702C
// #define						DISP_VACATION_TEMP4							0x702D

// #define						DISP_Inrernal_version						0X7042
// #define						DISP_External_version						0X704C
// #define						DISP_DSP_version								0X7056
// #define						DISP_PFC_version								0X7060
// #define						DISP_DeviceName									0X706A
// #define						DISP_MAC												0x7071
// #define						DISP_SSID												0x707A
// #define						DISP_BSSID											0x708A
// #define						DISP_Signal_strength						0x7093
// #define						DISO_test_Text1									0x7096
// #define						DISO_test_Text2									0x70B6
// #define						DISO_test_Text3									0x70D6
// #define						DISO_test_UART									0x70E1
// #define						DISO_test_WIFI_Status						0x70E2					//bit0:����״̬��bit1:MQTT״̬
/*
70e3~70e4��ˮͼ���޷�ʹ��
*/
/*
 *	0X7A00~7D1E7						//����SOP��Ļ��������
 */
typedef enum
{
	POWER_ON_INTERFACE = 0,
	SYSTEM_PASSWORD = 2,
	SYSTEM_CONNECT = 1,
	SYATEM_LOCK_INTERFACE = 3,
	POWER_HOME_INTERFACE = 4,
	MODE_COOLING_INTERFACE = 5,
	READ_DARA_INTERFACE = 22,
	RESET_DATA_INTERFACE = 23,
	SYSTEM_SOP_INTERFACE = 111,
	WIFI_SOP_INTERFACE = 112,

	//	page_preheat												=	10,									//Ԥ��ģʽ����
	page_current_fault = 12, // ���ϵ�ǰ����
	page_mode_heat_cool = 8, // ��ůģʽ
	page_mode_heating = 9,	 // ����ģʽ
							 //	para_time_C													=	25,									//����ʱ�����϶�
							 //	para_time_F													=	26,									//����ʱ�����϶�
} Enum_page;

typedef enum
{
	key_beep = 0x01,
	key_return = 0x02,
	key_fault = 0x03,
	key_reset_fault = 0x04,
	//	key_exit_fault						=	0x05,
	//	key_preheat								=	0x04,
	//	key_Normal								=	0x05,
	//	key_Fast									=	0x06,
	//	key_power_a								=	0x07,
	//	key_power_b								=	0x08,
	//	key_power_water						=	0x09,
	key_power_OFF = 0x08,
	key_power_ON = 0x09,
	key_power = 0x0A,
	key_mode = 0x0B,
	key_cooling = 0x0C,
	key_heating = 0x0D,
	key_cooling_ceo = 0x0E,
	key_heating_ceo = 0x0f,
	key_reset_para = 0x10,
	key_user_para = 0x11,
	key_eco = 0x12,
	//	key_auto									=	0x0E,
	//	key_lock_on								=	0x0F,
	//	key_lock_off							=	0x10,
	//	key_exit_holiday					= 0x11,
	//	key_mute_level						=	0x12,
	//	key_mute									=	0x13,
	ket_exit_timer_mute = 0x14,
	key_time = 0x15,
	key_time_ok = 0x16,
	key_exit_timer_holiday = 0x1e,
	//	key_timer_steri						=	0x17,
	//	key_exit_timer_steri			=	0x18,
	//	key_timer_pump1						=	0x19,
	//	key_exit_timer_pump1			=	0x1a,
	//	key_timer_pump2						=	0x1b,
	//	key_exit_timer_pump2			=	0x1c,
	//	key_timer_holiday					=	0x1d,
	//	key_exit_timer_holiday		=	0x1e,
	key_exit_timer_power = 0x1f,
	key_exit_timer_temp = 0x20,
	key_exit_sop = 0x21,
	key_exit_set = 0x22,

	//	key_temp1_mode						=	0x21,
	//	key_temp2_mode						=	0x22,
	//	key_temp3_mode						=	0x23,
	//	key_temp4_mode						=	0x24,
	//	key_holiday_heat					=	0x25,
	//	key_holiday_water					=	0x26,
	//	key_holiday_steri					=	0x27,
	//	key_Auxi_heat							=	0x28,
	//	key_Elec_heat							=	0x29,
	//	key_Unde_heat							=	0x3a,
	//	key_Unde_Dryi							=	0x2b,
	//	key_curve									=	0x2c,
	//	key_clear_curve						=	0x2d,
	//	key_unit_C								=	0x2e,
	//	key_unit_F								=	0x2f,
	key_clear_kwh = 0x2d,
	key_timer_temp = 0x2e,
	key_exit_timer_eco = 0x2f,
	/*
	Ԥ���߹������л�    30~36
	*/
	key_get_wifi = 0x36,
	key_network_details = 0x37,
	key_wifi_connect = 0x38,
	key_clear_fault = 0x39,
	//	key_factory_set						=	0x3a,
	key_refeigerant = 0x3b,
	key_Def = 0x3c,
	key_tset_beep = 0x3d,

	key_4G = 0X40,
	key_wifi = 0x41,
//  key_brand = 0x42,
//	key_back = 0x43,
	key_contact = 0x44,
	key_cleanCharge = 0x50, //清除电量数据
	key_room_temp = 0x51,//室温控制
	key_confirm_room_temp = 0x52,//确认室温控制
	key_clear_curve = 0x53,//清除温度曲线
	key_set_temp_sub= 0x54,//设定温度减
	key_set_temp_add= 0x55,//设定温度加
	key_room_temp0 = 0x56,//室温控制
	key_wifi_sop = 0x57,//sop wifi测试
	key_manual_get_location = 0x58,// 手动刷新位置信息
	key_manual_update_net = 0x59,// 手动更新网络状态
	key_wifi_status_refresh = 0x5a,// 刷新wifi信号
	key_read_mode_holiday = 0x5b,//离家模式跳转
	key_timer_mute = 0x5c,//定时静音跳转
	key_refresh_electricity = 0x5d,//电量统计刷新按钮
	key_electricity_daily_interface = 0x5e,//电量日报界面跳转
	key_electricity_monthly_interface = 0x5f,//电量月报界面跳转
	key_electricity_yearly_interface = 0x60,//电量年报界面跳转
	key_electricity_trend_interface = 0x61,//电量趋势界面跳转
	key_electricity_trend_hour_interface = 0x62,//电量趋势小时界面跳转
	key_electricity_trend_day_interface = 0x63,//电量趋势天界面跳转
	key_electricity_trend_month_interface = 0x64,//电量趋势月界面跳转
	key_electricity_trend_year_interface = 0x65,//电量趋势年界面跳转
} Enum_KEY;

extern struct
{ // 4G����WiFi
	uint8_t at_cmd;

} Connect;

extern struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t week;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t res;
} system;

extern const int16_t code Para_init_DATA[][3];
extern const uint8_t code Status_DATA[];

extern code uint16_t Connect_Para_Data[][2];
extern code uint8_t Connect_Status_Data[];

extern uint8_t T_timer_2ms;	  // 1ms Time count
extern uint8_t T_timer_10ms;  // 10ms Time count
extern uint8_t T_timer_100ms; // 100ms Time count
extern uint8_t T_timer_500ms; // 500ms Time count
extern uint8_t T_timer_1S;	  // 1S Time count
extern uint8_t T_timer_1Min;  // 1S Time count

extern uint8_t Sleep_Count; // ����ʱ�����
extern uint8_t Sleep_Count_M;
extern uint8_t Flag_Sleep;
//extern uint8_t Sleep_Mode;
extern uint8_t Flag_LOCK;
extern uint8_t Time_first_power_on; // Power on for the first time
extern uint8_t Flag_Interface;
extern uint16_t T_Curve_time;

extern uint16_t Flag_Tip;
extern int8_t T_Tip_time;

extern uint16_t Read_dgus_buf[10];

extern uint8_t Connect_RXbuf[Connect_RXBUFF];
extern uint8_t Connect_TXbuf[Connect_TXBUFF];

extern int8_t T_receive_Connect_stop_time;
extern uint8_t Flag_Connect_receive_ok;
// extern	uint16_t  Rx_Connect_length;											//modbus crc data length rx
extern uint16_t T_Connect_rx_count;
extern uint16_t Connect_master_tx_command;
extern uint16_t T_Connect_ota_time;
extern uint16_t Connect_ota_length;
extern uint8_t Flag_Connect_03_command;
extern uint8_t Flag_Connect_function_03_command;
extern uint16_t Adress_Connect_06_command_buf;
extern int16_t Data_Connect_06_command_buf;
extern int16_t T_Connect_send_time; // ���ͼ��ʱ��
extern uint16_t T_Connect_Send_State_time;
extern uint16_t T_Connect_Send_Advance_time;
extern uint8_t Connect_Send_fault_Count;
extern uint8_t Flag_Connect_EN;

extern uint8_t modbus_TXbuf[MODBUS_TXBUFF];	  // modbus tx buff
extern uint8_t modbus_Rxbuf[MODBUS_RXBUFF];	  // modbus rx buff
extern int16_t Para_pack[PARAMETER_PACK_NUM]; // Parameter pack
extern int16_t Para_pack_174[MODBUS_20_PACK]; // Parameter pack
extern int16_t State_pack[STATE_PACK_NUM];	  // State parameter package
extern int16_t State1_pack[MODBUS_33_PACK];	  // State parameter package
extern int16_t State2_pack[MODBUS_06_PACK];		// State parameter package
extern int16_t modbus_send_06[MODBUS_BUFF_06][2];
extern int8_t modbus_send_06_length;
extern uint8_t Rx_CRC_length;			 // modbus crc data length rx
extern uint8_t Tx_CRC_length;			 // modbus crc data length tx
extern uint16_t T_modbus_rx_count;		 // modbus rx data length count
extern uint16_t T_modbus_tx_count;		 // modbus tx data length count
extern uint8_t Time_communication_30S;	 // the time of communication fault
extern uint8_t flag_communication_fault; // the flag of communication fault
extern uint8_t Uart_Rx_Status;
extern uint8_t Uart_TX_Status;
// extern	uint8_t		Flag_read_all_data;
extern int8_t Flag_function_03_count;
extern int16_t Adress_function_06_command;	   // modbus 06 command Write address
extern int16_t Data_function_06_command;	   // modbus 06 command Write data
extern int16_t Adress_function_06_command_buf; // modbus 06 command Write address remmber
extern int16_t Data_function_06_command_buf;   // modbus 06 command Write data remmber
extern int16_t Adress_function_10_command;	   // modbus 10 command Write address
extern int16_t Length_function_10_command;	   // modbus 10 command Write length
extern uint8_t Flag_function_03_command;	   // modbus 03 command flag
extern uint8_t Flag_function_06_command;	   // modbus 06 command flag
extern uint8_t Flag_function_10_command;	   // modbus 10 command flag
extern uint8_t T_first_power_on_03_commnd;	   // modbus 03 command first power on ,qurey mainboard para
extern uint8_t Flag_master_tx_command;		   // modbus master tx command start
extern uint8_t Tx_senddata_num;				   // �������ݳ���
extern uint8_t Rx_CRC_length;				   // modbus crc data length rx
extern uint8_t Tx_CRC_length;				   // modbus crc data length tx
extern uint8_t T_485_DE_envelope_time;		   // 485 enbale control
extern int8_t T_receive_stop_time;
extern uint8_t Flag_modbus_receive_ok; // modbus rx is ok flag

extern uint8_t T_TSET_TIME_S;
extern uint8_t T_TSET_TIME_Min;
extern uint16_t Type_Interface;

extern uint16_t Painting_cmd_byte;
extern uint16_t TIME_ICO;
extern uint16_t usElectricity;
extern uint32_t ulNumberEl;
extern uint8_t fault_ico;
extern uint16_t UART_TSET;
extern uint8_t T_BEEP_TIME;
extern int16_t WIFI_dBM;
extern bit UART_MODBUS_OK;
extern uint16_t Connect_test;
extern uint16_t T_holiday_TIME;
extern uint8_t Flag_holiday_mode;
extern uint16_t rdIndoor_Temp_TIME;

extern uint8_t monitor_mode;
extern uint16_t monitor_timer;
extern uint16_t monitor_time;
extern uint16_t WIFI_TESET_VAILE;
extern uint16_t T_Connect_4G_wifi_time;
extern bit send_modbus_4g;
extern bit Clear_Fault_Bit;
extern uint16_t brand_message;
extern const u8  Failure_Level[9][16];
extern u16 Fault_Time_Cnt[9][16];
extern uint16_t usFaultStorageBuf[9];
extern uint16_t T_Detection_time;
extern uint16_t electricityPage;
// extern	uint32_t	password_value;
extern uint16_t HostMode;
extern uint16_t HostPower;
extern u16 totalHour;
extern u16 totalDay;
extern u32 totalMonth;
extern uint8_t SOP_MODE;

extern u16 dailyElectricityNum[24];   // 每小时电量
extern u16 monthlyElectricityNum[31]; // 每天电量
extern u16 yearlyElectricityNum[12];  // 每月电量
extern u16 trendHourElectricity[24];  // 近24小时趋势
extern u16 trendDayElectricity[31];   // 近31天趋势
extern u16 trendMonthElectricity[12]; // 近12个月趋势

extern uint8_t Enable_Host_Unlock;
extern uint8_t Flag_Host_Unlock;
extern uint16_t T_Network_Offline;
extern uint16_t TIPS_ICON;

extern uint16_t sendCache[20][2];

#endif
