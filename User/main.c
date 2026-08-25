#include "Dwin_CONF.h"
#include "HeadFiles\user_include.h"

int main(void)
{
	EA = false;
	Dwin_int();
	Var_Init();
	WIFI_EN = 1;
	EA = true;
	while (1)
	{
		if (T_timer_2ms > 1)
		{
			T_timer_2ms = 0;
			if (T_timer_10ms < 4)
			{
				T_timer_10ms++;
				Task_10ms_process();
			}
			else
			{
				T_timer_10ms = 0;
			}

			if (T_receive_stop_time > 0)
			{
				T_receive_stop_time--;
				if (T_receive_stop_time == 0)
				{
					if (SLAVE_ADRESS == modbus_Rxbuf[0])
					{
						Flag_modbus_receive_ok = 1;
						Rx_CRC_length = T_modbus_rx_count;
					}
					T_modbus_rx_count = 0;
				}
			}

			if (T_receive_Connect_stop_time > 0)
			{
				T_receive_Connect_stop_time--;
				if (T_receive_Connect_stop_time == 0)
				{
					Flag_Connect_receive_ok = 1;
					T_Connect_rx_count = 0;
					//					Rx_Connect_length	=	T_Connect_rx_count;
				}
			}

			Rs485_envelope_time_process(); // 485 enable end envelope
		}
		else if (T_Connect_send_time > 40) // T_Connect_send_time����500����
		{
			T_Connect_send_time = 0; // 4S��ʱ
			Connect_AT_Protocol();
		}
		Get_Temperature();
		connect_Master_command_handle_process();
		Master_send();
		if ((Flag_Connect_receive_ok))
		{
			Flag_Connect_receive_ok = 0;
			SET_BIT(Uart_Rx_Status, 4);
			Connect_AT_Read_Handler();
		}
		if ((Flag_modbus_receive_ok) && (!Flag_master_tx_command))
		{
			Master_receive_handle_process();
			Flag_modbus_receive_ok = 0;
		}
	}
}
