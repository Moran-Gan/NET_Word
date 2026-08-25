#ifndef	_Dwin_Uart3_h_
#define	_Dwin_Uart3_h_

typedef enum {
	UART3_FLAG_RI = 0X01,		//接收中断标志位RI
	UART3_FLAG_TI = 0X02	  	//发送中断标志位TI
} UART3_Flag_Typedef;

extern	void	UART3_Init(uint32_t My_BaudRate);
extern	void UART3_TWI_SendData(uint8_t Data);
extern	uint8_t UART3_TWI_ReceiveData(void);
extern	FlagStatus UART3_GetFlagStatus(UART3_Flag_Typedef UART3_Flag);
extern	void UART3_ClearFlag(UART3_Flag_Typedef UART3_Flag);

extern	void UART3_SendData(uint8_t Data);
extern	void	UART3_SendStr(uint8_t *pstr,uint16_t strlen);

#endif