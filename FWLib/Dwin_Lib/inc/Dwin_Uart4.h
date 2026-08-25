#ifndef	_Dwin_Uart4_h_
#define	_Dwin_Uart4_h_

typedef enum {
	UART4_FLAG_RI = 0X01,		//接收中断标志位RI
	UART4_FLAG_TI = 0X02	  	//发送中断标志位TI
} UART4_Flag_Typedef;


extern	FlagStatus UART4_RX_GetFlagStatus(void);
extern	FlagStatus UART4_TX_GetFlagStatus(void);
extern	void UART4_RX_ClearFlag(void);
extern	void UART4_TX_ClearFlag(void);

extern	void	UART4_Init(uint32_t My_BaudRate);
extern	void UART4_TWI_SendData(uint8_t Data);
extern	uint8_t UART4_TWI_ReceiveData(void);
extern	FlagStatus UART4_GetFlagStatus(UART4_Flag_Typedef UART4_Flag);
extern	void UART4_ClearFlag(UART4_Flag_Typedef UART4_Flag);

extern	void UART4_SendData(uint8_t Data);
extern	void	UART4_SendStr(uint8_t *pstr,uint16_t strlen);

#endif