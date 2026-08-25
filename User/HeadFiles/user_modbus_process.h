#ifndef	_user_modbus_process_h_
#define	_user_modbus_process_h_

void Analyze_function_03_data(uint8_t rx_03_buff[]);
void Analyze_function_06_data(uint8_t rx_06_buff[]);
void Analyze_function_10_data(uint8_t rx_10_buff[]);
void Master_receive_handle_process(void);
void Master_command_handle_process(void);
void R_User_modbus_receive(void);
void Rs485_envelope_time_process(void);
void Master_send(void);

void Master_Modbus_03_Fountion(uint16_t start_adress,uint16_t tx_length);
void Master_Modbus_06_Fountion(int16_t start_adress,int16_t tx_data);
void Master_Modbus_10_Fountion(int16_t start_adress,uint16_t tx_data_buf[],uint16_t tx_length);
void Send_modebus_command_06(int16_t address_06,int16_t data_06);

#endif

