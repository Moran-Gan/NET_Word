/**********************************Copyright (c)**********************************
**                       版权所有 (C), 
**
**                             http://
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  电控部
 * @version v1.0.0
 * @date    2023.08.14
 * @brief   模块AT指令发送以及指令解析，无需修改
 */
#include	"Dwin_CONF.h"
#include "HeadFiles\user_include.h"
#include <stdlib.h>
//#include	"user_include.h"

#if defined (tuya_WiFi)																						//涂鸦WiFi
#endif
#if defined (Ms_WiFi)																							//觅感WiFi

uint8_t code AT_BLENAME[] = "AT+BLENAME=ME,MSWB2213\r\n";					//开启蓝牙配网
uint8_t code AT_SMARTBTTYPE[] = "AT+SMARTBTTYPE=V2\r\n";
uint8_t code AT_SMARTBTSTART[] = "AT+SMARTBTSTART\r\n";

uint8_t AT_WSSSID[] = "AT+WSSSID=WIFI_TEST/r/n";               		//WiFi名称
uint8_t AT_WSKEY[] = "AT+WSKEY=WPA2PSK,AES,12345678/r/n"; 				//WiFi密码
uint8_t AT_WMODE[] = "AT+WMODE=STA/r/n";



uint8_t code AT_INIT[] = "+++"; 																	//申请指令集
uint8_t code AT_A[] = "A";																				//确认进入指令集
uint8_t AT_Z[] = "AT+Z\r\n"; 																			//退出指令模式 重启
uint8_t code AT_WSLQ[] = "AT+WSLQ\r\n";														//信号强度
uint8_t code AT_TCPLK[] = "AT+TCPLK\r\n";													//联网状态	
uint8_t code AT_EXCMD[] = "AT+EXCMD\r\n";													//指令模式转透传模式
uint8_t code AT_WFSTAMC[]	=	"AT+WFSTAMAC\r\n";										//查询MAC地址
																																	//连接阿里云服务器
uint8_t	code AT_NETP[] = "AT+NETP=ALI-IOT,1883,a5aqR1uU8m1.iot-as-mqtt.eu-central-1.aliyuncs.com\r\n";//"AT+NETP=ALI-IOT\r\n";
//uint8_t AT_DEVICE[] = "AT+DEVICE=WF20221205,O63cDTpjs9VCj6uD\r\n";//
//uint8_t AT_PRODUCT[] = "AT+PRODUCT= a5aqR1uU8m1/r/n"; /*ProductKey*/
//uint8_t AT_TOPIC[] = "AT+TOPIC=/a5aqR1uU8m1/WF20221205/user/update,/a5aqR1uU8m1/WF20221205/user/get\r\n";


uint8_t code 	RECV_SMARTBTSTART[] = "MSWBT2213";										//
uint8_t code 	RECV_DEVICE[]	=	"AT+DEVICE=\r\n";										//读取设备名
uint8_t code 	RECV_INIT[] = "a";																
uint8_t code	RECV_A[] = "+ok";
uint8_t code 	RECV_TCPLK[] = "+ok=on";														//配网成功
uint8_t code	RECV_EVENT[] = "+EVENT=DHCP_OK";										//STA 成功连接到路由器
uint8_t code	RECV_CON[] = "+EVENT=CON_OFF";											//STA 断网
uint8_t code	RECV_Z[] = "AT+Z/r/n/r+ok";
uint8_t	code	RECV_SOCKA[] = "+EVENT=SOCKA_ON";										//成功连接服务器
uint8_t	code	RECV_WSLQ[] = "AT+WSLQ";


#endif																														

#if defined (Quectel_WiFi)																				//移远WiFi_阿里云

uint8_t		QMTSIGN_LENGTH;

uint8_t code 	AT_QBLENAME[] = "AT+QBLENAME=ECO_HOME\r\n";					//把蓝牙名称修改成ECO_HOME
uint8_t code 	AT_QBLEINIT[] = "AT+QBLEINIT=3\r\n";								//模块开启蓝牙配网

uint8_t code 	AT_QMTCFG_version[] = "AT+QMTCFG=\"version\",1,4\r\n";				//设置 MQTT 协议的版本号为 V4
uint8_t code 	AT_QMTCFG_session[] = "AT+QMTCFG=\"session\",1,1\r\n";				//设置 MQTT 协议会话类型为 0，该连接则被认为是持久连接，所有的订阅主题都会被保存。
uint8_t code 	AT_QMTCFG_keepalive[] = "AT+QMTCFG=\"keepalive\",1,120\r\n";	//设置 QMTT 保活时间为10Min,心跳包为5Min
uint8_t	code	AT_QMTINFO[]	=	"AT+QMTINFO=0\r\n";														//读取三元组信息
uint8_t code 	AT_QMTCONN_READ[]	=		"AT+QMTCONN?\r\n";											//读取联网状态
uint8_t  	AT_QMTSIGN[] = "AT+QMTSIGN=\"a5aqR1uU8m1\",\"TS2212230466\",\"4ae23a1c8e8b3e70a727a7123456789f\"\r\n";	//计算三元组
uint8_t code 	AT_QMTOPEN[] = "AT+QMTOPEN=1,\"a5HMzjUHOtE.iot-as-mqtt.eu-central-1.aliyuncs.com\",1883\r\n";	//连接法兰克福服务器连接
																																																				//连接mqtt
uint8_t  	AT_QMTCONN[] = "AT+QMTCONN=1,\"a5aqR1uU8m1.TS2212230466|timestamp=2524608000000,securemode=2,signmethod=hmacsha256,lan=C|\",\"TS2212230466&a5aqR1uU8m1\",\"DFDB9C266311D2374B707A9DEA871009F2B3669ADC3C199D871AB6FA64857783\"\r\n";
uint8_t  	AT_QMTSUB_UP[] = "AT+QMTSUB=1,1,\"/a5aqR1uU8m1/TS2212230466/user/update\",1\r\n";							//订阅主题
uint8_t  	AT_QMTSUB_DN[] = "AT+QMTSUB=1,1,\"/a5aqR1uU8m1/TS2212230466/user/get\",1\r\n";								//订阅主题
uint8_t		AT_QMTPUBRAM[] = "AT+QMTPUBRAW=1,1,1,0,\"/a5aqR1uU8m1/TS2212230466/user/update\",999\r\n";		//通过MQTT服务器发送hex数据，6代表发送长度
uint8_t	code	AT_QGETWIFISTATE[]	=	"AT+QGETWIFISTATE\r\n";																							//查询WiFi信息
uint8_t	code	AT_QWLMAC[]	=	"AT+QWLMAC\r\n";

uint8_t	code	AT_QHTTPCFG_url[] = "AT+QHTTPCFG=\"url\",\"http://ehome.ne01.com/cloudservice/api/device/dtu/getIotDeviceDtuInfo\"\r\n";		//连接http
uint8_t	code	AT_QHTTPCFG_header[]	= "AT+QHTTPCFG=\"header\",\"Content-Type\",\"application/json;charset=UTF-8\"\r\n";
uint8_t	code	AT_QHTTPCFG_header1[]	= "AT+QHTTPCFG=\"header\",\"Accept\",\" */*\"\r\n";
uint8_t	code	AT_QHTTPCFG_output_1[]	=	"AT+QHTTPCFG=\"response/output\",1\r\n";
uint8_t	code	AT_QHTTPCFG_header_0[]	=	"AT+QHTTPCFG=\"response/header\",0\r\n";
uint8_t	code	AT_QHTTPPOST[]	=	"AT+QHTTPPOST=71,120,120\r\n";
uint8_t		AT_QHTTPPOST_DATA[]	=	"{\r\n\"dtuImeiMac\": \"b4:ed:d5:78:f7:78\",\r\n\"productKey\": \"a5HMzjUHOtE\"\r\n}\r\n";								//通过WiFi MAC和产品KEY
uint8_t	code	RECV_ProductKey[]	=	"a5HMzjUHOtE";																																													//判断产品KEY
uint8_t code RECV_QHTTPPOST[]	=	"CONNECT";
uint8_t code RECV_QHTTPURC[]	=	"+QHTTPURC: \"recv\"";

uint8_t	AT_WRITE_QMTINFO[]	=	"AT+QMTINFO=1,\"a5aqR1uU8m1\",\"TS2212230466\",\"4ae23a1c8e8b3e70a727a7123456789f\"\r\n";														//读取三元组信息








/*			
		OTA
*/
uint8_t	AT_QMTOTA[]	=	"AT+QMTOTA=1,\"1.0.0\"\r\n";																								//上传主控版本号

uint8_t	code	RECV_QMTOTASTATUS_DATA[]	  =	"+QMTOTASTATUS: 1,\"ENTERUPDATA\"";											//进入固件下载状态，1是<clientID>MQTT客户端标识符
uint8_t	code RECV_QMTOTASTATUS_OK[]				=	"+QMTOTASTATUS: 1,\"UPDATASUCCESS\"";									//下载完成
uint8_t	code RECV_QMTOTASTATUS_DATAFALL[]	=	"+QMTOTASTATUS: 1,\"UPDATAFAIL\"";	

uint8_t	code RECV_QMTFWINFO[]							=	"+QMTFWINFO:FW_VERSION";															//有固件
uint8_t	code RECV_ERROR[]									=	"ERROR";																							//错误
uint8_t	code RECV_NO_FW[]									=	"+QMTFWINFO:NO FW";																		//无固件

//uint8_t	AT_QMTOTA[]	=	"AT+QMTOTA=1,\"a5aqR1uU8m1\",\"TS2212230466\",\"4ae23a1c8e8b3e70a727a7123456789f\",\"a5aqR1uU8m1.iot-as-mqtt.eu-central-1.aliyuncs.com\",\"1.0.0\"\r\n";

uint8_t code RECV_OK[]	=	"OK";
uint8_t code RECV_QMTPUB[]	=	"+QMTPUB: 1,1,0";
uint8_t code RECV_QMTOPEN[]	=	"+QMTOPEN: 1,0";																										//应答连接法兰克福服务器连接
uint8_t code RECV_QMTCONN[]	=	"+QMTCONN: 1,0,0";																									//应答连接mqtt
uint8_t	code RECV_QMTSUB[]	=	"+QMTSUB: 1,1,0";																										//应答订阅主题
uint8_t	code RECV_QMTDISC[]	=	"+QMTDISC: 1,0";																										//断开mqtt成功
uint8_t	code RECV_QMTSTAT[]	=	"+QMTSTAT: 1,3";																										//已经连接上mqtt
uint8_t	code RECV_QMTSIGN[]	=	"+QMTSIGN:";																												//计算三元组
uint8_t	code RECV_QMTCONN_READ[]	=	"+QMTCONN: 1,3";																							//查询服务器连接情况
uint8_t	code RECV_QMTINFO[]	=	"+QMTINFO:";																												//读取三元组信息
uint8_t	code	RECV_QGETWIFISTATE[]	=	"+QGETWIFISTATE:";	
uint8_t	code	RECV_QWLMAC[]	=	"+QWLMAC:";	

uint8_t code 	AT_QMTDISC[]	=	"AT+QMTDISC=1\r\n";														//断开mqtt识别
//uint8_t  	AT_QMTSIGN[] = "AT+QMTSIGN=\"a5aqR1uU8m1\",\"TS2212230466\",\"4ae23a1c8e8b3e70a727a7123456789f\"\r\n";	//计算三元组


uint8_t	code	RECV_QMTOTA[]	=	"+QMTOTA: 1,0";

uint8_t	code	AT_QMTFWINFOREAD[] = "AT+QMTFWINFOREAD\r\n";																										//读取固件长度
uint8_t	AT_QMTFWREAD[] = "AT+QMTFWREAD=9999\r\n";		
uint8_t	code RECV_QSTASTAT[]	=	"+QSTASTAT:GOT_IP";	
uint8_t	code RECV_OTA[]	=	"+QMTRECV: 1,0,\"/ota/device/upgrade";	
uint8_t	code RECV_QMTRECV[]	=	"+QMTRECV:";

#endif																														






#if defined (Quectel_tx_WiFi)																			//移远WiFi_腾讯云
	
uint8_t code 	AT_QBLENAME[] = "AT+QBLENAME=ECO_HOME\r\n";					//把蓝牙名称修改成ECO_HOME
uint8_t code 	AT_QBLEINIT[] = "AT+QBLEINIT=3\r\n";								//模块开启蓝牙配网

u8 code cdWiFi_QWLMAC[] = "AT+QWLMAC\r\n";
u8 code cdWiFi_QDEVREG[] = "AT+QDEVREG\r\n";
u8 code cdWiFi_QMQTTCONN[] = "AT+QMQTTCONN=1,5000,240,1,1\r\n";
u8  cdWiFi_QMQTTSUB[] = "AT+QMQTTSUB=\"$thing/down/raw/M3GDPSSJOE/760209621368904\",1\r\n";
u8  cdWiFi_QMQTTPUBRAW[] = "AT+QMQTTPUBRAW=\"$thing/up/raw/M3GDPSSJOE/760209621368904\",1,9999\r\n";
u8  cdWiFi_QPRDINFOSET[] = "AT+QPRDINFOSET=1,\"M3GDPSSJOE\",\"hJU7wlQaPACwJglntkY9AB3m\",\"760209621368904\",\"china\"\r\n";


//uint8_t code 	AT_QMTCONN_READ[]	=		"AT+QMTCONN?\r\n";											//读取联网状态
uint8_t	code	AT_QGETWIFISTATE[]	=	"AT+QGETWIFISTATE\r\n";									//查询WiFi信息
uint8_t	code RECV_QMTCONN_READ[]	=	"+QMTCONN: 1,3";																							//查询服务器连接情况


uint8_t code RECV_OK[]	=	"OK";
uint8_t	code RECV_QSTASTAT[]	=	"+QSTASTAT:GOT_IP";																								//STA 获取到 IP执行一遍初始化

uint8_t code wRev_QPRDINFOSET[]="+QPRDINFOSET:OK";
uint8_t code wRev_QDEVREG[]="+QDEVREG:OK";
uint8_t code wRev_QMQTTCONN[]="+QMQTTCONN:OK";
uint8_t code wRev_QMQTTSUB[]="+QMQTTSUB:OK";
uint8_t code wRev_QWLMAC[]="+QWLMAC:";
uint8_t code RECV_QMTPUB[]	=	"+QMQTTPUBRAW:OK";//"+QMTPUB: 1,1,0";
uint8_t	code	RECV_QGETWIFISTATE[]	=	"+QGETWIFISTATE:";
//uint8_t	code RECV_QMTRECV[]	=	"+QMTRECV:";
uint8_t	code	RECV_TCMQTTRCVPUB[]	=	"+TCMQTTRCVPUB";

uint8_t	code	RECV_QSTAAPINFODEF[]	=	"+QSTASTATWLAN_CONNECTED";


uint8_t	code	ID[]	=	"C1OVAC6VS8";
uint8_t	code	ProductSecret[]	=	"2QGjuFaRRrWtmjTMnv0rrkn2";

uint8_t	code	cdQSTAAPINFODEF[] = "AT+QSTAAPINFODEF=53iq,53iq53iq\r\n";

#endif



/*
*函数：void	Connect_AT_Protocol(void)
*功能：串口发送AT指令集
*/
void	Connect_AT_Protocol(void)
{
//#if defined (tuya_WiFi)																							//涂鸦WiFi
//#endif
#if defined (Ms_WiFi)																								//觅感WiFi
	switch(Connect.at_cmd)
	{
		case	WF_ATINIT:
		{
			Connect_SendStr(AT_INIT,(sizeof(AT_INIT)-1));
		}
			break;
		
		case	WF_A:
		{
			Connect_SendStr(AT_A,(sizeof(AT_A)-1));
		}
			break;
		
		case	WF_TCPLK:
		{
			Connect_SendStr(AT_TCPLK,(sizeof(AT_TCPLK)-1));
		}
			break;
		
		case	WF_WSLQ:
		{																//信号强度
			Connect_SendStr(AT_WSLQ,(sizeof(AT_WSLQ)-1));
		}
			break;
			
		case	WF_EXCMD:
		{
			Connect_SendStr(AT_EXCMD,(sizeof(AT_EXCMD)-1));
		}
			break;
		
		case	WF_BLENAME:
		{
			Connect_SendStr(AT_BLENAME, (sizeof(AT_BLENAME)-1));
		}
			break;
		
		case	WF_SMARTBTTYPE:
		{
			Connect_SendStr(AT_SMARTBTTYPE, (sizeof(AT_SMARTBTTYPE)-1));
		}
			break;
		
		case	WF_SMARTBTSTART:
		{
			Connect_SendStr(AT_SMARTBTSTART, (sizeof(AT_SMARTBTSTART)-1));
		}
			break;
		
		case	WF_MAC:	
		{
			Connect_SendStr(AT_WFSTAMC, (sizeof(AT_WFSTAMC)-1));
		}
			break;			
		
//		case	WF_DEVICE:
//		{
//			Connect_SendStr(AT_DEVICE, (sizeof(AT_DEVICE)-1));
//		}
//			break;
//		
//		case	WF_PRODUCT:
//		{
//			Connect_SendStr(AT_PRODUCT, (sizeof(AT_PRODUCT)-1));
//		}
//			break;
//		
//		case	WF_TOPIC:
//		{
//			Connect_SendStr(AT_TOPIC, (sizeof(AT_TOPIC)-1));
//		}
//			break;
			
		case	WF_NETP:
		{
			Connect_SendStr(AT_NETP, (sizeof(AT_NETP)-1));
		}
			break;
		
		case	WF_EQUIPMENT:																											//读取设备号
		{
			Connect_SendStr(RECV_DEVICE, (sizeof(RECV_DEVICE)-1));
		}
			break;

//		case	WF_Z:																														//软件复位重启
//		{
//			Connect_SendStr(AT_Z, (sizeof(AT_Z)-1));
//		}
//			break;

		case	WF_DATA:																												//等待发送内容
		{
			Connect_SendStr(Connect_TXbuf,Connect_master_tx_command);
			Connect.at_cmd	=	WF_WAIT_SEND;																			//觅感发送数据不会应答默认成功
		}
			break;
		
		case	SEND_DATA:
		{
			Connect.at_cmd		=	WF_ATINIT;					//转进入指令模式
			Connect_reset_time();
		}
			break;
		
		case	Connect_wifi:
		{
			Connect.at_cmd = WF_ATINIT;
			Connect_reset_time();
			Flag_Connect_EN	=	0x02;
		}
			break;
		
		case	QMTINFO:
		{
			Connect.at_cmd = WF_ATINIT;
			Connect_reset_time();
			Flag_Connect_EN	|=	0x08;
		}
		break;

		default:
			break;
	}
#endif																														

#if defined (Quectel_WiFi)																					//移远WiFi_阿里云
	switch(Connect.at_cmd)
	{
		case	WF_QMTCFG_VERSION:
		{
			Connect_SendStr(AT_QMTCFG_version,(sizeof(AT_QMTCFG_version)-1));
		}
			break;
		
		case	WF_QMTCFG_SESSION:								//设置 MQTT 协议会话类型为 0，该连接则被认为是持久连接，所有的订阅主题都会被保存。
		{
			Connect_SendStr(AT_QMTCFG_session, (sizeof(AT_QMTCFG_session)-1));
		}
		break;
		
		case	WF_QMTCFG_KEEPALIVE:							//设置 QMTT 保活时间为10Min,心跳包为5Min
		{
			Connect_SendStr(AT_QMTCFG_keepalive, (sizeof(AT_QMTCFG_keepalive)-1));
		}
		break;
		
		case	WF_QMTDISC:												//断开MQTT
		{
			Connect_SendStr(AT_QMTDISC, (sizeof(AT_QMTDISC)-1));
		}
		break;
		
		case	WF_QMTINFO:												//读取三元组信息
		{
			Connect_SendStr(AT_QMTINFO, (sizeof(AT_QMTINFO)-1));
		}
			break;
		
		case	WF_QMTSIGN:												//计算三元组
		{
			Connect_SendStr(AT_QMTSIGN, (sizeof(AT_QMTSIGN)-1));
		}
		break;
		
		case	WF_QMTOPEN:												//连接法兰克福服务器连接
		{
			Connect_SendStr(AT_QMTOPEN, (sizeof(AT_QMTOPEN)-1));
		}
		break;
		
		case	WF_QMTCONN:												//连接MQTT
		{
			Connect_SendStr(AT_QMTCONN, QMTSIGN_LENGTH);
		}
		break;
		
		case	WF_QMTSUB_UP:												//订阅主题
		{
			Connect_SendStr(AT_QMTSUB_UP, (sizeof(AT_QMTSUB_UP)-1));
		}
		break;
		
		case	WF_QMTSUB_DN:												//订阅主题
		{
			Connect_SendStr(AT_QMTSUB_DN, (sizeof(AT_QMTSUB_DN)-1));
		}
		break;
		
		case	WF_QMTPUBRM:											//使用mqtt发送HEX数据
		{
			if(Connect_master_tx_command	<	10)
			{																			//发送长度0~9
				AT_QMTPUBRAM[61]	=	Connect_master_tx_command	+	'0';
				AT_QMTPUBRAM[62]	=	'\r';
				AT_QMTPUBRAM[63]	=	'\n';
				Connect_SendStr(AT_QMTPUBRAM, 64);
			}else	if(Connect_master_tx_command	<	100)
			{																			//发送长度10~99
				AT_QMTPUBRAM[61]	=	Connect_master_tx_command/10	+	'0';
				AT_QMTPUBRAM[62]	=	Connect_master_tx_command%10	+	'0';
				AT_QMTPUBRAM[63]	=	'\r';
				AT_QMTPUBRAM[64]	=	'\n';
				Connect_SendStr(AT_QMTPUBRAM, 65);
			}else
			{																			//发送长度100~999
				AT_QMTPUBRAM[61]	=	Connect_master_tx_command/100	+	'0';
				AT_QMTPUBRAM[62]	=	Connect_master_tx_command/10%10	+	'0';
				AT_QMTPUBRAM[63]	=	Connect_master_tx_command%10	+	'0';
				AT_QMTPUBRAM[64]	=	'\r';
				AT_QMTPUBRAM[65]	=	'\n';
				Connect_SendStr(AT_QMTPUBRAM, 66);
			}
		}
		break;
		
		case	WF_DATA:												//发送内容
		{
			Connect_SendStr(Connect_TXbuf,Connect_master_tx_command);
		}
		break;

		case	WF_QBLENAME:										//蓝牙名称修改成ECO_home
		{
			Connect_SendStr(AT_QBLENAME, (sizeof(AT_QBLENAME)-1));
		}
		break;
		
		case	WF_QBLEINIT:										//开启蓝牙配网
		{
			Connect_SendStr(AT_QBLEINIT, (sizeof(AT_QBLEINIT)-1));
		}
		break;
		
		case	WF_QMTOTA:											//上传主控版本号到云端
		{
			AT_QMTOTA[13] = State_pack[STATE_2020]/10	+	'0';
			AT_QMTOTA[15]	=	State_pack[STATE_2020]%10	+	'0';
			Connect_SendStr(AT_QMTOTA, (sizeof(AT_QMTOTA)-1));
		}
			break;
		
		case	WF_WAIT_OTA:										//等待下载程序完成
		{
			if(T_Connect_ota_time	>	300)
			{																		//等待10Min未下载完成默认重启
				Flag_Connect_EN	=	1;							
			}
		}
			break;
		
		case	WF_QMTFWINFOREAD:								//
		{
			Connect_SendStr(AT_QMTFWINFOREAD, (sizeof(AT_QMTFWINFOREAD)-1));
		}
			break;
		
		case	WF_QMTFWREAD:
		{
			
			if(Connect_ota_length	<	10)
			{																			//发送长度0~9
				AT_QMTFWREAD[13]	=	Connect_ota_length	+	'0';
				AT_QMTFWREAD[14]	=	'\r';
				AT_QMTFWREAD[15]	=	'\n';
				Connect_SendStr(AT_QMTFWREAD, 16);
			}else	if(Connect_ota_length	<	100)
			{																			//发送长度10~99
				AT_QMTFWREAD[13]	=	Connect_ota_length/10	+	'0';
				AT_QMTFWREAD[14]	=	Connect_ota_length%10	+	'0';
				AT_QMTFWREAD[15]	=	'\r';
				AT_QMTFWREAD[16]	=	'\n';
				Connect_SendStr(AT_QMTFWREAD, 17);
			}else	if(Connect_ota_length	<	1000)
			{																			//发送长度100~999
				AT_QMTFWREAD[13]	=	Connect_ota_length/100	+	'0';
				AT_QMTFWREAD[14]	=	Connect_ota_length/10%10	+	'0';
				AT_QMTFWREAD[15]	=	Connect_ota_length%10	+	'0';
				AT_QMTFWREAD[16]	=	'\r';
				AT_QMTFWREAD[17]	=	'\n';
				Connect_SendStr(AT_QMTFWREAD, 18);
			}else
			{
				AT_QMTFWREAD[13]	=	Connect_ota_length/1000	+	'0';
				AT_QMTFWREAD[14]	=	Connect_ota_length/100%10	+	'0';
				AT_QMTFWREAD[15]	=	Connect_ota_length/10%10	+	'0';
				AT_QMTFWREAD[16]	=	Connect_ota_length%10	+	'0';
				AT_QMTFWREAD[17]	=	'\r';
				AT_QMTFWREAD[18]	=	'\n';
				Connect_SendStr(AT_QMTFWREAD, 19);
			}

		}
			break;
		
		case	WF_QMTCONN_READ:																				//查询网络
		{
			Connect_SendStr(AT_QMTCONN_READ, (sizeof(AT_QMTCONN_READ)-1));
		}
		break;
		
		case	WF_QGETWIFISTATE:
		{
			Connect_SendStr(AT_QGETWIFISTATE, (sizeof(AT_QGETWIFISTATE)-1));
		}
			break;
		
		case	WF_QWLMAC:
		{
			Connect_SendStr(AT_QWLMAC, (sizeof(AT_QWLMAC)-1));
		}
		break;
		
		case	SEND_DATA:
		{
			Connect.at_cmd	=	WF_QMTCONN_READ;
			Connect_reset_time();
		}
			break;
		
		case	Connect_wifi:
		{
			Connect.at_cmd = WF_QBLENAME;
			Connect_reset_time();
			Flag_Connect_EN	=	0x02;
		}
			break;
		
		case	QMTINFO:
		{
			Connect.at_cmd = WF_QMTINFO;
		}
		break;
		
		case	WF_QHTTPCFG_url:
		{
			Connect_SendStr(AT_QHTTPCFG_url, (sizeof(AT_QHTTPCFG_url)-1));
		}
		break;
		
		case	WF_QHTTPCFG_header:
		{
			Connect_SendStr(AT_QHTTPCFG_header, (sizeof(AT_QHTTPCFG_header)-1));
		}
		break;
		
		case	WF_QHTTPCFG_header1:
		{
			Connect_SendStr(AT_QHTTPCFG_header1, (sizeof(AT_QHTTPCFG_header1)-1));
		}
		break;
		
		case	WF_QHTTPCFG_output_1:
		{
			Connect_SendStr(AT_QHTTPCFG_output_1, (sizeof(AT_QHTTPCFG_output_1)-1));
		}
		break;
		
		case	WF_QHTTPCFG_header_0:
		{
			Connect_SendStr(AT_QHTTPCFG_header_0, (sizeof(AT_QHTTPCFG_header_0)-1));
		}
		break;
		
		case	WF_QHTTPPOST:
		{
			Connect_SendStr(AT_QHTTPPOST, (sizeof(AT_QHTTPPOST)-1));
		}
		break;
		
		case	WF_QHTTPPOST_DATA:
		{
			Connect_SendStr(AT_QHTTPPOST_DATA, (sizeof(AT_QHTTPPOST_DATA)-1));
			T_Connect_send_time	=	-53;																								//云平台应答慢
		}
		break;
		
		case	WF_WRITE_QMTINFO:
		{
			Connect_SendStr(AT_WRITE_QMTINFO, (sizeof(AT_WRITE_QMTINFO)-1));
		}
			break;
		
		default:break;
	}
#endif
#if defined (Quectel_tx_WiFi)																				//移远WiFi_腾讯云
	switch(Connect.at_cmd)
	{
		case	WF_QWLMAC:
		{
			Connect_SendStr(cdWiFi_QWLMAC,(sizeof(cdWiFi_QWLMAC)-1));
		}break;
		
		case	WF_QPRDINFOSET:
		{
			Connect_SendStr(cdWiFi_QPRDINFOSET,(sizeof(cdWiFi_QPRDINFOSET)-1));
		}
			break;

		case	WF_QDEVREG:
		{
			Connect_SendStr(cdWiFi_QDEVREG,(sizeof(cdWiFi_QDEVREG)-1));
		}break;
		
		case	WF_QMQTTCONN:
		{
			Connect_SendStr(cdWiFi_QMQTTCONN,(sizeof(cdWiFi_QMQTTCONN)-1));
		}break;

		case	WF_QMQTTSUB:
		{
			Connect_SendStr(cdWiFi_QMQTTSUB,(sizeof(cdWiFi_QMQTTSUB)-1));
		}break;
		
		case	WF_QMTPUBRM:
		{
			if(Connect_master_tx_command	<	10)
			{
				cdWiFi_QMQTTPUBRAW[60] = Connect_master_tx_command + '0';
				cdWiFi_QMQTTPUBRAW[61]	=	'\r';
				cdWiFi_QMQTTPUBRAW[62]	=	'\n';
				Connect_SendStr(cdWiFi_QMQTTPUBRAW, 63);
			}
			else	if(Connect_master_tx_command	<	100)
			{
				cdWiFi_QMQTTPUBRAW[60] = Connect_master_tx_command/10	+	'0';
				cdWiFi_QMQTTPUBRAW[61] = Connect_master_tx_command%10	+	'0';
				cdWiFi_QMQTTPUBRAW[62]	=	'\r';
				cdWiFi_QMQTTPUBRAW[63]	=	'\n';
				Connect_SendStr(cdWiFi_QMQTTPUBRAW, 64);
			}
			else	if(Connect_master_tx_command	<	1000)
			{
				cdWiFi_QMQTTPUBRAW[60] = Connect_master_tx_command/100	+	'0';
				cdWiFi_QMQTTPUBRAW[61] = Connect_master_tx_command/10%10	+	'0';
				cdWiFi_QMQTTPUBRAW[62] = Connect_master_tx_command%10	+	'0';
				cdWiFi_QMQTTPUBRAW[63]	=	'\r';
				cdWiFi_QMQTTPUBRAW[64]	=	'\n';
				Connect_SendStr(cdWiFi_QMQTTPUBRAW, 65);
			}
			else
			{
				cdWiFi_QMQTTPUBRAW[60] = Connect_master_tx_command/1000	+	'0';
				cdWiFi_QMQTTPUBRAW[61] = Connect_master_tx_command/100%10	+	'0';
				cdWiFi_QMQTTPUBRAW[62] = Connect_master_tx_command/10%10	+	'0';
				cdWiFi_QMQTTPUBRAW[63] = Connect_master_tx_command%10	+	'0';
				cdWiFi_QMQTTPUBRAW[64]	=	'\r';
				cdWiFi_QMQTTPUBRAW[65]	=	'\n';
				Connect_SendStr(cdWiFi_QMQTTPUBRAW, 66);
			}

		}
			break;
		
		case	WF_DATA:												//发送内容
			{
				Connect_SendStr(Connect_TXbuf,Connect_master_tx_command);
			}
			break;
			
		case	WF_QBLENAME:										//蓝牙名称修改成ECO_home
			{
				Connect_SendStr(AT_QBLENAME, (sizeof(AT_QBLENAME)-1));
			}
			break;
			
			case	WF_QBLEINIT:										//开启蓝牙配网
			{
				Connect_SendStr(AT_QBLEINIT, (sizeof(AT_QBLEINIT)-1));
			}
			break;
			
//		case	WF_QMTCONN_READ:									//查询网络
//		{
//			Connect_SendStr(AT_QMTCONN_READ, (sizeof(AT_QMTCONN_READ)-1));
//		}
//		break;
		
		case	WF_QGETWIFISTATE:									//WIFI信息							
		{
			Connect_SendStr(AT_QGETWIFISTATE, (sizeof(AT_QGETWIFISTATE)-1));
		}
			break;
		
		case	Connect_wifi:
		{
			Connect.at_cmd = WF_QBLENAME;
			Connect_reset_time();
			Flag_Connect_EN	=	0x02;
		}
			break;
		
		case	SEND_DATA:
		{
	  	Connect.at_cmd	=	WF_QGETWIFISTATE;
			Connect_reset_time();
		}
			break;
		
//		case	WF_QGETWIFISINGLE:									//WIFI信号							
//		{
//			Connect_SendStr(AT_QGETWIFISTATE, (sizeof(AT_QGETWIFISTATE)-1));
//		}
//			break;
		
		case Connect_wifi_sop:
		{
			Connect_SendStr(cdQSTAAPINFODEF, (sizeof(cdQSTAAPINFODEF)-1));
			//Flag_Connect_EN	=	0x01;
			Connect.at_cmd	=	NOP;
			Connect_reset_time();
		}
		break;
		default:break;
	}		
	
#endif


}

/*
*函数：void	Connect_AT_Read_Handler(void)
*功能：串口解析指令集
*该函数放在while
*/
void	Connect_AT_Read_Handler(void)
{
//#if defined (tuya_WiFi)																							//涂鸦WiFi
//#endif
#if defined (Ms_WiFi)																								//觅感WiFi
	uint8_t	value;
//	uint8_t	connect_addr;
	uint8_t	connect_read_rxbuf[Connect_RXBUFF];
	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
	switch(Connect.at_cmd)
	{
		case	WF_ATINIT:
		{
			if(Connect_RXbuf[0]	==	'a')
			{
				Connect.at_cmd	=	WF_A;
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_A:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,3);
			value	=	strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				if(Flag_Connect_EN	&	0x08)										//BIT0:复位动作，BIT1:配网标志位，BIT2:判断网络是否在线，BIT3:获取设备编码
				{																							//获取设备编码
					Flag_Connect_EN	&= ~0x08;
					Connect.at_cmd = WF_EQUIPMENT;
				}
				else	if(Flag_Connect_EN	&	0x02)
				{																							//设备进入配网
					Connect.at_cmd = WF_NETP;										//进入配网
				}
				else
				{
					Connect.at_cmd = WF_TCPLK;									//联网成功查询网络是否正常
				}
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_EQUIPMENT:																											//读取设备号
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[13], 3);
			value = strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd = WF_TCPLK;
				write_dgus_vp(DISP_DeviceName,&Connect_RXbuf[17],6);							//打印设备名称到线控器上	
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_TCPLK:
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[11], 6);
			value	=	strcmp(RECV_TCPLK, connect_read_rxbuf);
			if(value == 0)
			{
				Flag_Connect_EN	|=	0x04;								//模块在线
				Connect.at_cmd	=	WF_WSLQ;							//退出指令模式
				Connect_reset_time();
			}
			else	if(Connect_Send_fault_Count	>	1)
			{
				Connect_reset_time();
				Flag_Connect_EN	|=	0x04;								//模块在线
				Connect.at_cmd	=	WF_WSLQ;							//退出指令模式
			}
		}
			break;
		
		case	WF_WSLQ:
		{																							//信号强度
			memcpy(connect_read_rxbuf, &Connect_RXbuf[0], 7);
			value	=	strcmp(RECV_WSLQ, connect_read_rxbuf);
			if(value == 0)
			{
				if(Connect_RXbuf[14]	==	'G')
				{																					//有效地址
					
				}
				Connect_reset_time();
				Connect.at_cmd	=	WF_EXCMD;							//退出指令模式
			}
		}
			break;
			
		case	WF_EXCMD:
		{																							//退出指令集
			memcpy(connect_read_rxbuf, &Connect_RXbuf[11], 3);
			value	=	strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd	=	WF_WAIT_SEND;
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_BLENAME:
		{
			memcpy(connect_read_rxbuf, Connect_RXbuf, 3);
			value = strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd	=	WF_SMARTBTTYPE;
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_SMARTBTTYPE:
		{
			memcpy(connect_read_rxbuf, Connect_RXbuf, 3);
			value = strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd	=	WF_SMARTBTSTART;
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_SMARTBTSTART:
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[18], 3);
			value = strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd	=	WF_WAIT;
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_WAIT:																						//等待联网成功
		{
			memcpy(connect_read_rxbuf, Connect_RXbuf, 15);
			value = strcmp(RECV_SOCKA, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd	=	WF_WAIT_SEND;
				Connect_reset_time();
			}
		}break;
		
		
		case	WF_MAC:	
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[13], 3);
			value = strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{

				Connect_reset_time();
			}
		}
			break;			
	
		case	WF_NETP:
		{
			memcpy(connect_read_rxbuf, Connect_RXbuf, 3);
			value = strcmp(RECV_A, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd = WF_BLENAME;
				Connect_reset_time();
			}
		}
			break;
		
		default:
			break;
	}
	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
	memcpy(connect_read_rxbuf, &Connect_RXbuf[0], 14);
	value	=	strcmp(RECV_CON, connect_read_rxbuf);
	if(value	==	0)
	{
		Flag_Connect_EN	&=	~0x04;								//离线
	}
	else
	{
		memcpy(connect_read_rxbuf, &Connect_RXbuf[0], 15);
		value	=	strcmp(RECV_SOCKA, connect_read_rxbuf);
		if(value	==	0)
		{
			Flag_Connect_EN	=	0x0C;								//在线
			Connect_test	|=	0x01;
			Connect.at_cmd	=	WF_ATINIT;
			Flag_Connect_03_command	=	0;
			Connect_reset_time();
		}
	}
	
																						//每次进入中断都判断是否为正常数据下发
	Connect_Master_receive_handle_process(&Connect_RXbuf[0],8);
#endif
	
#if defined (Quectel_WiFi)																					//移远WiFi_阿里云
	uint8_t	value,value1;
	uint8_t	connect_addr;
	uint16_t length1,length2,length3;
	uint8_t	connect_read_rxbuf[Connect_RXBUFF];
	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
	connect_addr	=	Connect_return_addr(0,'+',Connect_RXBUFF);
	switch(Connect.at_cmd)
	{
		case	WF_QMTCFG_VERSION:								//设置 MQTT 协议的版本号为 V4
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMTCFG_SESSION;
			}
		}	
			break;
		
		case	WF_QMTCFG_SESSION:								//设置 MQTT 协议会话类型为 0，该连接则被认为是持久连接，所有的订阅主题都会被保存。
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMTCFG_KEEPALIVE;
			}
		}
		break;
		
		case	WF_QMTCFG_KEEPALIVE:							//设置 QMTT 保活时间为10Min,心跳包为5Min
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QWLMAC;
				//Connect.at_cmd	=	WF_QMTINFO;//WF_QMTCONN_READ;//WF_QMTOPEN;
			}
		}
		break;
		
		case	WF_QMTINFO:												//读取三元组信息
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],9);
			value	=	strcmp(RECV_QMTINFO, connect_read_rxbuf);
			if(value	==	0)
			{			
				memcpy(connect_read_rxbuf, &Connect_RXbuf[(connect_addr + 10)], 11);				//product_key
				value	=	strcmp(RECV_ProductKey, connect_read_rxbuf);
				if(value	==	0)
				{//三元组内容显示到线控器上
					memcpy(&AT_QMTSIGN[11], &Connect_RXbuf[(connect_addr + 9)], 63);				//计算三元组
					memcpy(&AT_QMTSUB_UP[16], &Connect_RXbuf[(connect_addr + 10)], 11);				//product_key
					memcpy(&AT_QMTSUB_UP[28], &Connect_RXbuf[(connect_addr + 24)], 12);				//device_name
					
					memcpy(&AT_QMTSUB_DN[16], &Connect_RXbuf[(connect_addr + 10)], 11);				//product_key
					memcpy(&AT_QMTSUB_DN[28], &Connect_RXbuf[(connect_addr + 24)], 12);				//device_name
					
					memcpy(&AT_QMTPUBRAM[23], &Connect_RXbuf[(connect_addr + 10)], 11);				//product_key
					memcpy(&AT_QMTPUBRAM[35], &Connect_RXbuf[(connect_addr + 24)], 12);				//device_name
				
//				memcpy(&AT_QMTOTA[12], &Connect_RXbuf[(connect_addr + 9)], 63);				//product_key			
//				
				write_dgus_vp(DISP_DeviceName,&Connect_RXbuf[(connect_addr + 24)],6);							//打印设备名称到线控器上	

					//已经获取到正确三元组
					if(Flag_Connect_EN	& 0x10)
					{
						Connect.at_cmd	=	NOP;
					}
					else
					{
						Connect.at_cmd	=	WF_QMTCONN_READ;
					}
					Flag_Connect_EN &= ~ 0x10;
				}
				else
				{
					Connect.at_cmd	=	WF_QHTTPCFG_url;
				}
				
				
				Connect_reset_time();
				
//				Connect.at_cmd	=	WF_QWLMAC;	
//				if(Flag_Connect_EN	& 0x10)
//				{
//					Connect.at_cmd	=	NOP;
//				}
//				else
//				{
//					Connect.at_cmd	=	WF_QMTCONN_READ;
//				}
//				Flag_Connect_EN &= ~ 0x10;
			}
		}
			break;
		
		case	WF_QMTSIGN:												//计算三元组
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],9);
			value	=	strcmp(RECV_QMTSIGN, connect_read_rxbuf);
			if(value	==	0)
			{	
				value1	=	Connect_return_addr(connect_addr,'"',Connect_RXBUFF);
				value	=	Connect_return_addr(value1+1,'"',Connect_RXBUFF)+2;
				length1 = value - value1;
				memcpy(&AT_QMTCONN[13], &Connect_RXbuf[value1], length1);													//clientid
				
				value1	=	Connect_return_addr((connect_addr + value + 5),'"',Connect_RXBUFF);
				value	=	Connect_return_addr(value1+1,'"',Connect_RXBUFF)+2;
				length2 = value - value1;
				memcpy(&AT_QMTCONN[13+length1], &Connect_RXbuf[value1], length2);									//username

				value1	=	Connect_return_addr((connect_addr + value + 5),'"',Connect_RXBUFF);
				value	=	Connect_return_addr(value1+1,'"',Connect_RXBUFF)+2;
				length3	=	value - value1;
				memcpy(&AT_QMTCONN[13+length1+length2], &Connect_RXbuf[value1],length3);					//password
				
				AT_QMTCONN[12+length1+length2 + length3] = '\r';
				AT_QMTCONN[13+length1+length2 + length3] = '\n';
				QMTSIGN_LENGTH	=	14+length1+length2 + length3;		
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMTOPEN;
			}
		}
		break;
		
		case	WF_QMTOPEN:												//连接法兰克福服务器连接
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],13);
			value	=	strcmp(RECV_QMTOPEN, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMTCONN;
			}
		}
		break;

		case	WF_QMTCONN:												//连接MQTT
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],13);
			value	=	strcmp(RECV_QMTSTAT, connect_read_rxbuf);
			
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],15);
			value1	=	strcmp(RECV_QMTCONN, connect_read_rxbuf);
			if(value	==	0	||	value1	==	0)
			{
				Connect_reset_time();
				Flag_Connect_EN	|=	0x04;
				Connect.at_cmd	=	WF_QMTSUB_DN;//WF_QMTSUB_UP;
				Connect_test	|=	0x02;
			}
		}
		break;
		
		case	WF_QMTSUB_UP:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],14);
			value	=	strcmp(RECV_QMTSUB, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMTSUB_DN;													//等待发送
				Flag_Connect_03_command	=	0;														//初始化发送
			}
		}
		break;
		
		case	WF_QMTSUB_DN:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],14);
			value	=	strcmp(RECV_QMTSUB, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_WAIT_SEND;													//等待发送
				Flag_Connect_03_command	=	0;														//初始化发送
			}
		}
			break;	
		
		case	WF_QMTPUBRM:
		{																				//使用mqtt发送hex数据
			if(Connect_RXbuf[0]	==	0x3E)
			{
				Connect.at_cmd	=	WF_DATA;
				Connect_reset_time();
			}
		}
		break;
		
		case	WF_DATA:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],14);
			value	=	strcmp(RECV_QMTPUB, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect.at_cmd	=	WF_WAIT_SEND;
				Connect_reset_time();
			}
		}
			break;
		
		case	WF_QBLENAME:																									//蓝牙名称修改成ECO_home
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[0],2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QBLEINIT;
			}
		}
		break;
		
		case	WF_QBLEINIT:																								//开启蓝牙配网
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[0],2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{																		
				Connect_reset_time();									//等待配网成功
				Connect.at_cmd	=	NOP;								//等待配网成功
			}
		}
		break;
		
		case	WF_QMTCONN_READ:																						//查询网络
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],13);
			value	=	strcmp(RECV_QMTCONN_READ, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();									
				Connect.at_cmd	=	WF_QGETWIFISTATE;			
				Flag_Connect_EN	|=	0x04;
			}
			else
			{
				Flag_Connect_EN	&=	~0x04;
				if(Flag_Connect_03_command	!=	Connect_length)
					Connect.at_cmd	=	WF_QMTSIGN;//WF_QMTINFO;//WF_QMTOPEN;
			}
		}
		break;
		
		case	WF_QGETWIFISTATE:
		{

			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],15);
			value	=	strcmp(RECV_QGETWIFISTATE, connect_read_rxbuf);
			if(value	==	0)
			{
				value1	=	Connect_return_addr((connect_addr + 20),',',Connect_RXBUFF);				//判断有效地址
				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
				memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr + 20], (value1 - (connect_addr + 20)));				
				write_dgus_vp(DISP_SSID,connect_read_rxbuf,16);													//显示线控器硬件
				
				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
				
				memcpy(connect_read_rxbuf, &Connect_RXbuf[value1 + 30], 4);
				write_dgus_vp(DISP_Signal_strength,connect_read_rxbuf,2);		

				memcpy(connect_read_rxbuf, &Connect_RXbuf[value1 + 7], 17);
				write_dgus_vp(DISP_BSSID,connect_read_rxbuf,9);													//显示线控器硬件

			}
			
			if(Flag_Connect_03_command	!=	Connect_length)
			{
				Flag_Connect_03_command	=	0;
			}
			Connect_reset_time();	
			Connect.at_cmd	=	WF_QMTOTA;	
			//Connect.at_cmd	=	WF_QWLMAC;	
				
		}
		break;
		
		case	WF_QWLMAC:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],8);
			value	=	strcmp(RECV_QWLMAC, connect_read_rxbuf);
			if(value	==	0)
			{																						//读取mac地址
				write_dgus_vp(DISP_MAC,&Connect_RXbuf[connect_addr + 8],9);
				memcpy(&AT_QHTTPPOST_DATA[18],&Connect_RXbuf[connect_addr + 8],17);


				//显示线控器硬件
				Connect_reset_time();	
				Connect.at_cmd	=	WF_QMTINFO;
//				if(Flag_Connect_EN	& 0x10)
//				{
//					Connect.at_cmd	=	NOP;
//				}
//				else
//				{
//					Connect.at_cmd	=	WF_QMTCONN_READ;
//				}
//				Flag_Connect_EN &= ~ 0x10;
				//Connect.at_cmd	=	WF_QMTOTA;	
			}
		}
		break;
		
		case	WF_QMTOTA:											//上传主控版本号到云端
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 12);
			value = strcmp(RECV_QMTOTA, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();	
				
			}
			Connect.at_cmd	=	WF_WAIT_SEND;	
		}
			break;
		
		case	WF_WAIT_OTA:										//等待下载程序完成
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 29);
			value = strcmp(RECV_QMTOTASTATUS_DATAFALL, connect_read_rxbuf);
			if(value	==	0)
			{																		//升级失败重启模块
				Flag_Connect_EN	=	1;
			}
			else
			{
				memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 32);
				value = strcmp(RECV_QMTOTASTATUS_OK, connect_read_rxbuf);
				if(value	==	0)
				{
					Connect_reset_time();	
					Connect.at_cmd	=	WF_QMTFWINFOREAD;	
				}
			}
		}
		break;
		
		case	WF_QMTFWINFOREAD:
		{
			memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 16);
			value = strcmp(RECV_NO_FW, connect_read_rxbuf);
			if(value	==	0)
			{
				Flag_Connect_EN	=	1;
			}
			else
			{
				memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 21);
				value = strcmp(RECV_QMTFWINFO, connect_read_rxbuf);
				if(value	==	0)
				{
					Connect.at_cmd	=	WF_WAIT_QMTFWREAD;				//等待主控进入OTA升级
				}
			}
		}
		break;
		
		case	WF_QHTTPCFG_url:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QHTTPCFG_header;
			}
		}
		break;
		
		case	WF_QHTTPCFG_header:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QHTTPCFG_header1;
			}
		}
		break;
		
		case	WF_QHTTPCFG_header1:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QHTTPCFG_output_1;
			}
		}
		break;
		
		case	WF_QHTTPCFG_output_1:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QHTTPCFG_header_0;
			}
		}
		break;
		
		case	WF_QHTTPCFG_header_0:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QHTTPPOST;
			}
		}
		break;
		
		case	WF_QHTTPPOST:
		{
			connect_addr	=	Connect_return_addr(0,'C',Connect_RXBUFF);
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],7);
			value	=	strcmp(RECV_QHTTPPOST, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QHTTPPOST_DATA;
			}
		}
		break;
		
		case	WF_QHTTPPOST_DATA:
		{
			connect_addr	=	Connect_return_addr(20,'+',Connect_RXBUFF);
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],17);
			value	=	strcmp(RECV_QHTTPURC, connect_read_rxbuf);
			if(value	==	0)
			{
				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
				memcpy(connect_read_rxbuf, &Connect_RXbuf[(connect_addr + 88)], 11);				//product_key
				value	=	strcmp(RECV_ProductKey, connect_read_rxbuf);
				if(value	==	0)
				{	
					//memcpy(&AT_WRITE_QMTINFO[14], &Connect_RXbuf[(connect_addr + 115)], 12);				//product_key
					memcpy(&AT_WRITE_QMTINFO[28], &Connect_RXbuf[(connect_addr + 115)], 12);				//
					memcpy(&AT_WRITE_QMTINFO[14], &Connect_RXbuf[(connect_addr + 88)], 11);				//product_key
					memcpy(&AT_WRITE_QMTINFO[43], &Connect_RXbuf[(connect_addr + 40)], 32);				//device_name
					Connect_reset_time();
					Connect.at_cmd	=	WF_WRITE_QMTINFO;
				}	
				else
				{
					Flag_Connect_EN	=	0x01;							//重启WiFi模块
				}					

			}
		}
		break;
		
		case	WF_WRITE_QMTINFO:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMTCFG_VERSION;
				
				//Flag_Connect_EN	=	0x01;							//重启WiFi模块
			}
		}
			break;
		
		default:break;
	}
	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
	memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 9);
	value	=	strcmp(RECV_QMTRECV, connect_read_rxbuf);
	if(value	==	0)
	{																																//云端有数据下发
		connect_addr	=	Connect_return_addr(16,',',Connect_RXBUFF);				//判断有效地址
		if(Connect_RXbuf[(connect_addr	+	1)]	==	'8'	&&	Connect_RXbuf[(connect_addr	+	2)]	== ',')
		{																																	//修改单独参数
			Connect_Master_receive_handle_process(&Connect_RXbuf[(connect_addr	+	4)],8);
		}
		
	}
	else	
	{
		memcpy(connect_read_rxbuf,Connect_RXbuf,16);
		value	=	strcmp(RECV_QSTASTAT, connect_read_rxbuf);
		if(value	==	0)
		{																											//断网重连，需要重新连接阿里云于上传全部参数
			Connect_reset_time();									
			Connect.at_cmd	=	WF_QMTCFG_VERSION;	
			Flag_Connect_03_command	=	0;	
			Connect_test	|=	0x01;
		}
		else
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],30);
			value	=	strcmp(RECV_QMTOTASTATUS_DATA, connect_read_rxbuf);
			if(value	==	0	&&	Connect.at_cmd	!=	WF_WAIT_OTA	&& Connect.at_cmd	!=	WF_QMTFWREAD)
			{
				T_Connect_ota_time	=	0;
//				Connect.at_cmd	=	WF_WAIT_OTA;									//进入OTA程序入口
			}
		}
	}
#endif
#if defined (Quectel_tx_WiFi)																				//移远WiFi_腾讯云
	
	uint8_t	value,value1;
	uint8_t	connect_addr;
	uint8_t	connect_read_rxbuf[Connect_RXBUFF];
	char new_mac_address[18]="760"; // 为新MAC地址字符串分配空间
	uint8_t	IMEI_CODE[16];
//	static	uint8_t	
	
	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
	memset(IMEI_CODE, 0, sizeof(IMEI_CODE));
	connect_addr	=	Connect_return_addr(0,'+',Connect_RXBUFF);
	
//	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
//	connect_addr	=	Connect_return_addr(0,'+',Connect_RXBUFF);
	switch(Connect.at_cmd)
	{
		
		case	NOP:
		{
//			memcpy(connect_read_rxbuf,Connect_RXbuf,16);
//			value	=	strcmp(RECV_QSTASTAT, connect_read_rxbuf);
//			if(value	==	0)
//			{																											//断网重连，需要重新连接阿里云于上传全部参数
//				Connect_reset_time();									
//				Connect.at_cmd	=	WF_QGETWIFISTATE;	
//				Flag_Connect_03_command	=	0;
//				SET_BIT(Flag_Connect_EN,2);
//			}
//			else
//			{
//				memcpy(connect_read_rxbuf,Connect_RXbuf,23);
//				value	=	strcmp(RECV_QSTAAPINFODEF, connect_read_rxbuf);
//				if(value	==	0)
//				{		
//					Flag_Connect_EN	=	1;				
//					Connect.at_cmd	=	WF_QGETWIFISTATE;
//				}
//			}
		}
		break;
		
		case	WF_QWLMAC:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],8);
			value	=	strcmp(wRev_QWLMAC, connect_read_rxbuf);
			if(value	==	0)
			{
				
				write_dgus_vp(DISP_MAC,&Connect_RXbuf[connect_addr + 8],9);
				read_dgus_vp(MODBUS_IMEI,(u8*)IMEI_CODE,8);

				memcpy(&cdWiFi_QMQTTSUB[40],&IMEI_CODE[0],15);
				memcpy(&cdWiFi_QMQTTPUBRAW[41],&IMEI_CODE[0],15);
				memcpy(&cdWiFi_QPRDINFOSET[58],&IMEI_CODE[0],15);
				memcpy(&new_mac_address[0],&IMEI_CODE[0],15);
//				removeColon(&IMEI_CODE[0],&cdWiFi_QMQTTSUB[40]);
//				removeColon(&IMEI_CODE[0],&cdWiFi_QMQTTPUBRAW[41]);
//				removeColon(&IMEI_CODE[0],&cdWiFi_QPRDINFOSET[58]);
//				removeColon(&IMEI_CODE[0],&new_mac_address[0]);
				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
				write_dgus_vp(DISP_DeviceName,new_mac_address,8);
				
//				removeColon(&Connect_RXbuf[connect_addr + 8],&cdWiFi_QMQTTSUB[43]);
//				removeColon(&Connect_RXbuf[connect_addr + 8],&cdWiFi_QMQTTPUBRAW[44]);
//				removeColon(&Connect_RXbuf[connect_addr + 8],&cdWiFi_QPRDINFOSET[61]);
//				removeColon(&Connect_RXbuf[connect_addr + 8],&new_mac_address[3]);
//				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
//				write_dgus_vp(DISP_DeviceName,new_mac_address,8);
		
				WIFI_TESET_VAILE |= 0x01; 
				
				Connect_reset_time();
				if(GET_BIT(Flag_Connect_EN,4))
				{																													//读取MAC地址
					CLR_BIT(Flag_Connect_EN,4);
					if(GET_BIT(Flag_Connect_EN,2))
					{
						Connect.at_cmd	=	WF_WAIT_SEND;
					}
					else
					{
						Connect.at_cmd	=	NOP;
					}
				}
//				if(SOP_MODE == 1)
//				{
//					Connect.at_cmd	=	WF_QGETWIFISTATE;
//				}
//				else
//				{
					if(Para_pack[PARA_F18_122]	==	5)
					{																												//单热机
						memcpy(&cdWiFi_QPRDINFOSET[18],ID,10);
						memcpy(&cdWiFi_QPRDINFOSET[31],ProductSecret,24);				
						memcpy(&cdWiFi_QMQTTSUB[29],ID,10);
						memcpy(&cdWiFi_QMQTTPUBRAW[30],ID,10);

					}				
					Connect.at_cmd	=	WF_QPRDINFOSET;
//				}				
			}
		}break;
		
//			case	WF_QGETWIFISTATE:
//		{

//			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],15);
//			value	=	strcmp(RECV_QGETWIFISTATE, connect_read_rxbuf);
//			if(value	==	0)
//			{
//				value1	=	Connect_return_addr((connect_addr + 20),',',Connect_RXBUFF);				//判断有效地址
//				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
//				memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr + 20], (value1 - (connect_addr + 20)));				
//				write_dgus_vp(DISP_SSID,connect_read_rxbuf,16);													//显示线控器硬件
//				
//				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
//				
//				memcpy(connect_read_rxbuf, &Connect_RXbuf[value1 + 30], 4);
//				write_dgus_vp(DISP_Signal_strength,connect_read_rxbuf,2);		

//				WIFI_dBM = atoi(&Connect_RXbuf[value1 + 30]);
//				write_dgus_vp(DISP_wifi_dbm_state,(u8*)&WIFI_dBM,1);
//				
//				memcpy(connect_read_rxbuf, &Connect_RXbuf[value1 + 7], 17);
//				write_dgus_vp(DISP_BSSID,connect_read_rxbuf,9);													//显示线控器硬件
//			}
//			
//				Connect_reset_time();	
//		  	Connect.at_cmd	=	WF_QPRDINFOSET;
//			
////			if(Flag_Connect_03_command	!=	Connect_length)
////			{
////				Flag_Connect_03_command	=	0;
////			}
//	
//		}
//		break;

		
		case	WF_QPRDINFOSET:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],15);
			value	=	strcmp(wRev_QPRDINFOSET, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QDEVREG;
				WIFI_TESET_VAILE |= 0x04; 
			}
		}
			break;

		case	WF_QDEVREG:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,11);
			value	=	strcmp(wRev_QDEVREG, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMQTTCONN;
			}
		}break;
		
		case	WF_QMQTTCONN:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,13);
			value	=	strcmp(wRev_QMQTTCONN, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QMQTTSUB;
				WIFI_TESET_VAILE |= 0x08; 
			}
		}break;

		case	WF_QMQTTSUB:
		{
			memcpy(connect_read_rxbuf,Connect_RXbuf,12);
			value	=	strcmp(wRev_QMQTTSUB, connect_read_rxbuf);
			if(value	==	0)
			{
//				if(SOP_MODE == 1)
//				{
//					Connect_reset_time();
//					Connect.at_cmd	=	WF_QMTPUBRM;
//				}
//				else
//				{
					Connect_reset_time();
					Connect.at_cmd	=	WF_QGETWIFISTATE;
					//Connect.at_cmd	=	WF_WAIT_SEND;
//				}
			}
		}break;
		
		case	WF_QMTPUBRM:
		{
			if(Connect_RXbuf[0]	==	0x3E)
			{
				Connect.at_cmd	=	WF_DATA;
				Connect_reset_time();
//			SET_BIT(Flag_Connect_EN,2);
			}
		}
			break;
		
		case	WF_DATA:												//发送内容
			{
				memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],15);
				value	=	strcmp(RECV_QMTPUB, connect_read_rxbuf);
				if(value	==	0)
				{
					Connect.at_cmd	=	WF_WAIT_SEND;
					Connect_reset_time();
				}
			}
			break;
			
		case	WF_QBLENAME:										//蓝牙名称修改成ECO_home
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[0],2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{
				Connect_reset_time();
				Connect.at_cmd	=	WF_QBLEINIT;
			}
		}
		break;
		
		case	WF_QBLEINIT:										//开启蓝牙配网
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[0],2);
			value	=	strcmp(RECV_OK, connect_read_rxbuf);
			if(value	==	0)
			{																		
				Connect_reset_time();									//等待配网成功
				Connect.at_cmd	=	CONNECT;						//等待配网成功
			}
		}
		break;
		
//		
//		case	WF_QMTCONN_READ:																						//查询网络
//		{
//			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],13);
//			value	=	strcmp(RECV_QMTCONN_READ, connect_read_rxbuf);
//			if(value	==	0)
//			{
//				Connect_reset_time();									
//				Connect.at_cmd	=	WF_QGETWIFISTATE;			
//				Flag_Connect_EN	|=	0x04;
//			}
//			else
//			{
//				Flag_Connect_EN	&=	~0x04;
//				
////				Connect.at_cmd	=	WF_QMTSIGN;//WF_QMTINFO;//WF_QMTOPEN;
//			}
//		}
//		break;

	case	WF_QGETWIFISTATE:
		{
			memcpy(connect_read_rxbuf,&Connect_RXbuf[connect_addr],15);
			value	=	strcmp(RECV_QGETWIFISTATE, connect_read_rxbuf);
			if(value	==	0)
			{		
				value1	=	Connect_return_addr((connect_addr + 20),',',Connect_RXBUFF);				//判断有效地址
				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
				memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr + 20], (value1 - (connect_addr + 20)));				
				write_dgus_vp(DISP_SSID,connect_read_rxbuf,16);													//显示线控器硬件
				
				memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
				
				memcpy(connect_read_rxbuf, &Connect_RXbuf[value1 + 30], 4);
				write_dgus_vp(DISP_Signal_strength,connect_read_rxbuf,2);		

				WIFI_dBM = atoi(&Connect_RXbuf[value1 + 30]);
				write_dgus_vp(DISP_wifi_dbm_state,(u8*)&WIFI_dBM,1);
				
				memcpy(connect_read_rxbuf, &Connect_RXbuf[value1 + 7], 17);
				write_dgus_vp(DISP_BSSID,connect_read_rxbuf,9);													//显示线控器硬件		
				
//			  if(SOP_MODE == 1)
//				{	 	 	 
//          Connect_reset_time();   	 	 	 
//          Connect.at_cmd  =  WF_QPRDINFOSET;	 	 	 
//				}
//			  else
//				{
				  Connect_reset_time();	
		  	  Connect.at_cmd	= WF_WAIT_SEND;//WF_QMTOTA;	
//				}
			}
		}
		break;

		default:break;
	}

	memset(connect_read_rxbuf, 0, sizeof(connect_read_rxbuf));
	memcpy(connect_read_rxbuf, &Connect_RXbuf[connect_addr], 13);
	value	=	strcmp(RECV_TCMQTTRCVPUB, connect_read_rxbuf);
	if(value	==	0)
	{																																//云端有数据下发
		connect_addr	=	Connect_return_addr(16,',',Connect_RXBUFF);				//判断有效地址
		if(Connect_RXbuf[(connect_addr	+	1)]	==	'8'	&&	Connect_RXbuf[(connect_addr	+	2)]	== ',')
		{																																	//修改单独参数
			Connect_Master_receive_handle_process(&Connect_RXbuf[(connect_addr	+	4)],8);
		}
	}
	else
	{
			memcpy(connect_read_rxbuf,Connect_RXbuf,16);
			value	=	strcmp(RECV_QSTASTAT, connect_read_rxbuf);
			if(value	==	0	&&	!GET_BIT(Flag_Connect_EN,2))
			{																											//断网重连，需要重新连接阿里云于上传全部参数
				if(Connect.at_cmd	==	CONNECT)
				{
					Flag_Connect_EN	=	1;
				}
				else
				{
					Connect_reset_time();									
					Connect.at_cmd	=	WF_QWLMAC;	
					Flag_Connect_03_command	=	0;
					SET_BIT(Flag_Connect_EN,2);
				}
				WIFI_TESET_VAILE |= 0x02; 
			}
			if (value	==	0	&& SOP_MODE == 1)
			{
					Connect_reset_time();									
					Connect.at_cmd	=	WF_QWLMAC;	
					Flag_Connect_03_command	=	0;
					SET_BIT(Flag_Connect_EN,2);
			}
			
			/*else
			{
				memcpy(connect_read_rxbuf,Connect_RXbuf,23);
				value	=	strcmp(RECV_QSTAAPINFODEF, connect_read_rxbuf);
				if(value	==	0)
				{		
					Flag_Connect_EN	=	1;				
				}
			}*/
	}


#endif
	memset(Connect_RXbuf, 0, sizeof(Connect_RXbuf));
}

/*
*函数：Connect_Overting_Handler(void)
*功能：配网超时，4Min
*该函数放在秒时间片
*/
void	Connect_Overting_Handler(void)
{
	static uint8_t	t_reset_timer_s = 0,t_reset_timer_min = 0;
	if(Flag_Connect_EN	& 0x02)
	{
		if(t_reset_timer_s	<	59)
		{
			t_reset_timer_s	++;
		}
		else
		{
			t_reset_timer_s	=	0;
			if(t_reset_timer_min	<	4)
			{
				t_reset_timer_min++;
			}
			else
			{
				t_reset_timer_min	=	0;
				Flag_Connect_EN	=	0x01;
			}
		}
	}
	else
	{
		t_reset_timer_s	=	0;
		t_reset_timer_min	=	0;
	}
}
/*
*函数：Connect_Reset_Protocol(void)
*功能：WIFI模块重启,持续发送6帧数据失败模块重启
*时序：复位键拉低500Ms左右后拉高模块复位（复位键低电平有效）
*该函数放在100MS时间片
*/
void	Connect_Reset_Protocol(void)
{
	static int8_t	t_reset_timer = 0;
	if(Connect_Send_fault_Count	>	5)
	{
		Connect_Send_fault_Count	=	0;
		Flag_Connect_EN	=	0x01;							//重启WiFi模块
		t_reset_timer	=	0;
	}
	
	if(Flag_Connect_EN	& 0x01)
	{
		t_reset_timer++;
		if(t_reset_timer	<	6)
		{
			WIFI_EN	=	false;
		}
		else 
		{
			//if(SOP_MODE==1)
		
				if(Para_pack[PARA_H06_99]	!= 0)
				{
					WIFI_EN	=	true;
				}
					
				t_reset_timer	=	0;
				Flag_Connect_EN	=	0;
				Connect.at_cmd	=	NOP;
				Connect_test	=	0;
				WIFI_TESET_VAILE	=	0;

		}
	}
}
void	Connect_WiFi_EN_Protocol(void)
{
	uint8_t	IMEI_CODE[2];
	read_dgus_vp(MODBUS_IMEI,(u8*)IMEI_CODE,1);
	if(Para_pack[PARA_H06_99]	==	0 || (IMEI_CODE[0] != '8' || IMEI_CODE[1] != '6') ||	T_Connect_4G_wifi_time < 120)
	{
		if(SOP_MODE==0)
		{
			WIFI_EN	=	false;
			Flag_Connect_EN	=	0; 
			Connect.at_cmd	=	NOP;
			Connect_test	=	0;
			WIFI_TESET_VAILE	=	0;
		}
		else
		{
			if(!(Flag_Connect_EN	& 0x01))
			{
				WIFI_EN	=	true;
			}
		}
	}
	else if(!(Flag_Connect_EN	& 0x01))
	{
		WIFI_EN	=	true;
	}
}

/*
*函数：uint8_t	Connect_Send_Data(uint8_t	Fun,uint16_t	modbus_add,uint16_t	len,int16_t	*Value)
*功能：发送数据
*返回：1表示发送中，0表示发送成功
*/
uint8_t	Connect_Send_Data(uint8_t	Fun,uint16_t	modbus_add,uint16_t	len,int16_t	*Value)
{
	uint8_t	value = 1;
	
//#if defined (tuya_WiFi)																							//涂鸦WiFi
//#endif
	
#if defined (Ms_WiFi)																								//觅感WiFi
																																		//觅感使用透传
	if(Connect.at_cmd	==	WF_WAIT_SEND)																//处于待发送状态
	{
		if(Fun	==	MASTER_06_COMMAND)
		{
			Connect_Modbus_06_Fountion(modbus_add,len);
			Connect_master_tx_command	=	8;						//06指令固定8个字节
		}
		else	
		{
			Connect_Modbus_03_Fountion(modbus_add,len,Value);
			Connect_master_tx_command	=	(len*2)+13;		//赋值发送数据长度
		}
		Connect_Send_fault_Count	=	0;
		T_Connect_send_time	=	60;
		Connect.at_cmd	=	WF_DATA;								//发送HEX
		value	=	0;																//觅感使用透窗无需AT指令
	}
#endif

#if defined (Quectel_WiFi)	 || defined (Quectel_tx_WiFi)	
																		//当前无发送任务
	static uint8_t	Connect_stpe = 0;
	static	uint16_t	Connect_modbus_addr = 0;
	if(Connect_stpe	==	0)
	{
		if(Connect.at_cmd	==	WF_WAIT_SEND)		//处于待发送状态
		{
			if(Fun	==	MASTER_06_COMMAND)
			{
				Connect_master_tx_command	=	8;						//06指令固定8个字节
				Connect_Modbus_06_Fountion(modbus_add,len);
			}
			else	
			{
				Connect_Modbus_03_Fountion(modbus_add,len,Value);
				Connect_master_tx_command	=	(len*2)+13;		//赋值发送数据长度
			}
			Connect_modbus_addr	=	modbus_add;
			Connect.at_cmd	=	WF_QMTPUBRM;			//发送HEX
			Connect_reset_time();
			Connect_stpe	=	1;
			
		}
	}
	else	if(Connect_stpe	==	1	&&	Connect.at_cmd	==	WF_WAIT_SEND)
	{
		Connect_stpe	=	0;
		if(Connect_modbus_addr	==	modbus_add)
		{
			Connect_modbus_addr	=	0;
			value	=	0;
		}
		
	}
	
#endif	

	return	value;
}




/*
*函数：uint8_t	Connect_return_addr(uint8_t	addr,uint8_t	Data,uint16_t	len)
*功能：判断Data数据所在地址并返回
*/
uint8_t	Connect_return_addr(uint8_t	addr,uint8_t	Data,uint16_t	len)
{
	uint16_t	i;
	uint8_t	vault;
	for(i	=	addr;	i	<	len;i++)
	{
		if(Connect_RXbuf[i] == Data)
		{
			vault	=	i;
			break;
		}
	}
	return	vault;
}


/*
*函数：void	Connect_reset_time(void)
*功能：重置发送时间，立即发送下一帧指令
*/
void	Connect_reset_time(void)
{
	Connect_Send_fault_Count	=	0;
	T_Connect_send_time	=	39;
}


// 函数：去除MAC地址中的冒号并合并为一个字符串
void removeColon(char* mac_address, char* new_mac_address) {
    int8_t j = 0;
	int8_t	i;
	//strlen(mac_address)
    for ( i = 0; i < 17; i++) {
        if (mac_address[i] != ':') {
            new_mac_address[j] = mac_address[i];
            j++;
        }
    }
//    new_mac_address[j] = '\0';
}


