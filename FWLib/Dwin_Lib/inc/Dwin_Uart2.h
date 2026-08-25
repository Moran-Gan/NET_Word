#ifndef	_Dwin_Uart2_h_
#define	_Dwin_Uart2_h_


typedef enum {
	UART2_FLAG_RI = 0X01,		//接收中断标志位RI
	UART2_FLAG_TI = 0X02	  	//发送中断标志位TI
} UART2_Flag_Typedef;



extern	void	UART2_Init(uint32_t My_BaudRate);
extern	void UART2_TWI_SendData(uint8_t Data);
extern	uint8_t UART2_TWI_ReceiveData(void);
extern	FlagStatus UART2_GetFlagStatus(UART2_Flag_Typedef UART2_Flag);
extern	void UART2_ClearFlag(UART2_Flag_Typedef UART2_Flag);

extern	void	UART2_SendStr(uint8_t *pstr,uint16_t strlen);


#endif