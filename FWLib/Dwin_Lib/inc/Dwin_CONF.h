#ifndef _Dwin_CONF_h_
#define _Dwin_CONF_h_

#define true 1
#define false 0

#define CPU_CLK 206438400

/*!< Signed integer types  */
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;

/*!< Unsigned integer types  */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef enum
{
    RESET = 0,
    SET = !RESET
} FlagStatus,
    ITStatus, BitStatus;

/*****************************************
 *	    ϵͳ�ӿڱ�����ַ�궨��            *
 *****************************************/
#define NOR_FLASH 0x0008 //
#define SOFT_VERSION 0x000F
#define RTC 0x0010
#define PIC_NOW 0x0014
#define TP_STATUS 0x0016

#define WAE_SET 0x00A0

#define LED_NOW 0x0031
#define AD_VALUE 0x0032
#define TEMP_VALUE 0X003E
#define LED_CONFIG 0x0082
#define PIC_SET 0x0084
#define RTC_Set 0x009C
#define TOUCH_AUTO_FLAG 0x0F00

#define CPL_BIT(value, bit) (value ^= (1 << bit))  // ȡ��ָ��λ
#define SET_BIT(value, bit) (value |= (1 << bit))  // ��λָ��λ
#define CLR_BIT(value, bit) (value &= ~(1 << bit)) // ����ָ��λ
#define GET_BIT(value, bit) (value & (1 << bit))   // ��ȡָ��λ

#include "t5los8051.h"

#include "HeadFiles\Dwin_it.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_8130.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_flash.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_IIC.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_int.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_time2.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_Uart2.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_Uart3.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_Uart4.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_Uart5.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_wdt.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_ad.h"
#include ".\FWLib\Dwin_Lib\inc\Dwin_wirte_value.h"

#endif