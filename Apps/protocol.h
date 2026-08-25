
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#define			Connect_length						3
#define			Send_State_Time						Read_dgus_buf[2]//300


void	Connect_Master_receive_handle_process(uint8_t *tx_data_buf,uint8_t	tx_len);
void 	connect_Master_command_handle_process(void);
void	connect_Analyze_function_03_data(uint8_t *tx_data_buf);
void	connect_Analyze_function_06_data(uint8_t *tx_data_buf);
void	Connect_Para_Data_C_F(uint16_t	addr,int16_t connect_send[]);
void Connect_Send_modebus_command_06(int16_t address_06,int16_t data_06);
//extern	uint8_t	UPDATA_STPE;

//#define		read				0
//#define		send				1	


//extern	void WiFi_Send_Protocol(void);
//extern	void	WiFi_Protocol_Handler(void);
//extern	void	WIFI_MODBUS_03_SEND_Value(uint16_t	addr,uint16_t	Data);
//extern	void	WIFI_MODBUS_06_SEND_Value(uint16_t	addr,uint16_t	Data,uint8_t	Bit);
//extern	void	Uart3_WIFI_Modbus_Fun_06(uint8_t *tx_data_buf);
//extern	void	Uart3_WIFI_SET_DATA_Handler(int8_t tx_data_buf[]);
//extern	void	Uart3_WIFI_data_Read_Protocol(uint8_t *tx_data_buf,uint8_t	tx_len);
//extern	void	UART3_WIFI_UPDATA_Protocol(void);
//extern	uint8_t	data_send_cmd;
#endif
