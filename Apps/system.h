

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

typedef enum
{
	NOP	=	0x00,
	WF_QMTCFG_VERSION	=	0X01,
	WF_QMTCFG_SESSION,
	WF_QMTCFG_KEEPALIVE,
	WF_QMTDISC,
	WF_QMTSIGN,
	WF_QMTOPEN,
	WF_QMTCONN,
	WF_QMTSUB_UP,
	WF_QMTSUB_DN,
	WF_QBLENAME,
	WF_QBLEINIT,
	WF_QMTPUBRM,
	WF_DATA,
	WF_WAIT_SEND,
	UPDATA_PARA,
	WF_QMTOTA,
	WF_QMTFWINFOREAD,
	WF_WAIT_OTA,
	WF_WAIT_READ_OTA_DATA,
	WF_QMTFWREAD,
	WF_WAIT_QMTFWREAD,
	WF_QMTCONN_READ,
	WF_QMTINFO,
	WF_QMTCONN_READ1,
	WF_QGETWIFISTATE,
	WF_QWLMAC,
	SEND_DATA,
	Connect_wifi,
	QMTINFO,
	CONNECT,
	Connect_wifi_sop,
//WF_QGETWIFISINGLE,
	
	
	
	WF_ATINIT	=	0xA0,			//+++,
  WF_A,
	WF_TCPLK,
	WF_WSLQ,
	WF_EXCMD,
	WF_BLENAME,
  WF_SMARTBTTYPE,
  WF_SMARTBTSTART,
	WF_WAIT,
	WF_DEVICE,
	WF_PRODUCT,
	WF_TOPIC,
	WF_NETP,
	WF_MAC,
	WF_EQUIPMENT,
	WF_Z,
	
	WF_QPRDINFOSET	=	0XC0,
	WF_QDEVREG,
	WF_QMQTTCONN,
	WF_QMQTTSUB,
	WF_QMQTTPUBRAW,
	
}Enum_Connect;

void	Connect_AT_Protocol(void);
void	Connect_AT_Read_Handler(void);
void	Connect_reset_time(void);
void	Connect_Reset_Protocol(void);
void	Connect_Overting_Handler(void);
uint8_t	Connect_return_addr(uint8_t	addr,uint8_t	Data,uint16_t	len);
uint8_t	Connect_Send_Data(uint8_t	Fun,uint16_t	modbus_add,uint16_t	len,int16_t	*Value);
void removeColon(char* mac_address, char* new_mac_address);
void	Connect_WiFi_EN_Protocol(void);


#endif
  
  
