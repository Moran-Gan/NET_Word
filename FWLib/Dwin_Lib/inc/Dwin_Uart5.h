#ifndef	_Dwin_Uart5_h_
#define	_Dwin_Uart5_h_

typedef enum {
	UART5_FLAG_RI = 0X01,		//接收中断标志位RI
	UART5_FLAG_TI = 0X02	  	//发送中断标志位TI
} UART5_Flag_Typedef;

extern	FlagStatus UART5_RX_GetFlagStatus(void);
extern	FlagStatus UART5_TX_GetFlagStatus(void);
extern	void UART5_RX_ClearFlag(void);
extern	void UART5_TX_ClearFlag(void);


extern	void	UART5_Init(uint32_t My_BaudRate);
extern	void UART5_TWI_SendData(uint8_t Data);

extern	uint8_t UART5_TWI_ReceiveData(void);
extern	FlagStatus UART5_GetFlagStatus(UART5_Flag_Typedef UART5_Flag);
extern	void UART5_ClearFlag(UART5_Flag_Typedef UART5_Flag);

extern	void UART5_SendData(uint8_t Data);
extern	void	UART5_SendStr(uint8_t *pstr,uint16_t strlen);

#endif