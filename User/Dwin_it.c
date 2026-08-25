#include	"Dwin_CONF.h"
#include	"user_include.h"

/**********************************************
*		�ж�Դ����					*			�жϱ��				*
*		�ⲿ�ж�0						*			interrupt	0			*
*		Uart3�ж�						*			interrupt	16		*
*		T0��ʱ���ж�				*			interrupt	1			*
*		CANͨ���ж�					*			interrupt	9			*
*		�ⲿ�ж�1						*			interrupt	2			*
*		Uart3�����ж�				*			interrupt	10		*
*		T1��ʱ���ж�				*			interrupt	3			*
*		Uart4�����ж�				*			interrupt	11		*
*		Uart2�ж�						*			interrupt	4			*
*		Uart5�����ж�				*			interrupt	12		*
*		T2��ʱ���ж�				*			interrupt	5			*
*		Uart5�����ж�				*			interrupt	13		*
***********************************************/

void UART2_ISR_PC(void)				interrupt 4
{
	uint8_t	res;
	if(SCON0 & UART2_FLAG_RI)
	{
		res = SBUF0;
		SCON0 &=	(~UART2_FLAG_RI);
	}
	
	if(SCON0 & UART2_FLAG_TI)
	{
		SCON0 &=	(~UART2_FLAG_TI);
		CLR_BIT(Uart_TX_Status,Uart2);
	}
}

void T2_ISR_PC(void)					interrupt 5
{
	TF2 = false;	
	if(T_timer_2ms < 0xFF)
	{
			T_timer_2ms ++;
	}
	else
	{
		T_timer_2ms = 0;
	}
}

//void UART4_TX_ISR_PC(void)    interrupt 10
//{
////	if(UART4_TX_GetFlagStatus())
////	{
////		UART4_TX_ClearFlag();
////		CLR_BIT(UART_TX_Status,Uart4);
////	}
//}

//void UART4_RX_ISR_PC(void) 		interrupt 11
//{
//	uint8_t	res;
//	//if(UART4_RX_GetFlagStatus())
//	if(SCON2R & 0x01)
//	{
//		res = SBUF2_RX;//UART4_TWI_ReceiveData();
//		SCON2R &=	(~0x01);
//		//UART4_RX_ClearFlag();
//	}
//}

void UART5_TX_ISR_PC(void)    interrupt 12
{
	static uint8_t tx_num = 0;
	if(SCON3T & 0x01)
	{
		SCON3T &=	(~0x01);
		
		if(3 == Flag_master_tx_command)
		{
			if(Tx_senddata_num)
			{
				Tx_senddata_num --;
				SBUF3_TX = modbus_TXbuf[tx_num];
				tx_num ++;
			}
			else
			{
				Flag_master_tx_command = 4;
				tx_num = 0;
			}
		}
	}

}

void UART5_RX_ISR_PC(void)    interrupt 13
{

	if(SCON3R & 0x01)
	{
		SCON3R &= (~0x01);
		UART_TSET	|=	0x01;
		T_receive_stop_time	=	10;
		modbus_Rxbuf[T_modbus_rx_count++] = SBUF3_RX;
		if(T_modbus_rx_count >= MODBUS_RXBUFF)
		{
			T_modbus_rx_count = 0;
		}
	}
}

void	UART3_ISR_PC(void)			interrupt 16
{

	if(SCON1 & UART3_FLAG_RI)
	{
		SCON1 &=	(~UART3_FLAG_RI);																	//���SCON1��־λ��Ҫ������ն���,����鱸ע
		SCON1 &=	(~UART3_FLAG_RI);
//		uart_receive_input(SBUF1);
		UART_TSET	|=	0x02;
		T_receive_Connect_stop_time	=	5;
		Connect_RXbuf[T_Connect_rx_count] = SBUF1;
		if(T_Connect_rx_count < Connect_RXBUFF)
		{
			T_Connect_rx_count++;
		}
	}
	
	if((SCON1 & UART3_FLAG_TI) ==	UART3_FLAG_TI)
	{
		SCON1 &=	(~UART3_FLAG_TI);																	//���SCON1��־λ��Ҫ������ն���,����鱸ע
		SCON1 &=	(~UART3_FLAG_TI);
		CLR_BIT(Uart_TX_Status,Uart3);
	}
}