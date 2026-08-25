#ifndef	_user_special_process_h_
#define	_user_special_process_h_

void Var_Init(void);
void Fault_process(void);
void Advanced_function_process(void);
void Icon_driver_process(void);
void Timer_run_process(void);
//void	Temp_Curve_process(uint8_t	eeprom_ok);
void	Display_TIP_ICO(uint8_t	value);
void	Clear_Fault_process(void);
//void	Clear_Temp_Curve_process(void);
//void	Display_Curve_page(uint8_t	page);
//void	Temp_Curve_process(uint8_t	eeprom_ok,int16_t	TEMP1,int16_t	TEMP2,int16_t	TEMP3,int16_t	TEMP4);
//void	Init_Display_Curve(void);
int  calculateDaysSinceYear2000(int year, int month, int day);
void Wite_VP(u16 addr, u16 vpdata);
u16 Read_VP(u16 addr);
void clearFaultAferSevenDays(void);
void Brand_Select(void);
void Generate_recover_fault(void);
void GraphicRendering(u16 inputHeights[7], u16 segments);
void Electricity_Display(u8 mode, u8 page);
void Electricity_process(void);
void WriteTemperatureToVPBuffer(void);
void Display_Electricity_Curve(void);
void Test_CH4_Hour_Curve_Display(void);
void Test_CH4_Day_Curve_Display(void);
void Test_CH4_Month_Curve_Display(void);
void Display_Electricity_Stats(void);
void Clear_Electricity_Stats(void);
void Clear_Trend_Electricity(void);
void IndoorTempControl(void);
void Display_Mute_Room_Temp_ICO(void);
void Send_Cache_Process(void);
void Send_Cache_Write(uint16_t address, uint16_t send_data);
void Set_Elec_Hide_Select(u16 select);
#endif

