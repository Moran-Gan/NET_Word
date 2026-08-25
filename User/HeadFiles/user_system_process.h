#ifndef	_user_system_process_h_
#define	_user_system_process_h_
extern bit write_flash_bit;
int16_t EXCHANGE_C_F(int16_t temp_CV,int16_t type_CV);
int16_t EXCHANGE_F_C(int16_t temp_CV,int16_t type_CV);
void Interface_running_process(void);
void	Parm_Set_Function(void);
void 	Sleep_Handler(void);
void	Return_Home(void);
void	EEPROM_Read_page(void);
void	Parameter_setting_process(void);
void advan_password_confirm(u16 Addr,u32 passwordByte,u8 interface,u8 Read_Byte);
void	Time_Temp_Mode(uint16_t	addr,uint16_t	addr2);
void	Get_time_temp(void);
u32 PasswordCalculation(void);
void	EXIT_VACATION(void);
u32 L610_PasswordCalculation(uint8_t	*IMEI_CODE);
void EEPROM_Write_page(void);
void EEPROM_Reset_page(void);
void HostModeProcessing(void);
void HostPowerProcessing(void);
void Exit_Timer_Interface(void);
#endif