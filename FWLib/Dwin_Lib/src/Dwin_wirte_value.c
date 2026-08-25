#include	"Dwin_CONF.h"
#include	<string.h>
#include	"user_include.h"

//#include	"Global.h"
//int dpm[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31}; /* ƽ��ÿ������ */

/*****************************************************************************
 �� �� ��  : void read_dgus_vp(u32 addr,u8* buf,u16 len)
 ��������  : ��dgus��ַ��ֵ
 �������  :	 addr��dgus��ֵַ  len�������ݳ���
 �������  : buf�����ݱ��滺����
 �޸���ʷ  :
  1.��    ��   : 2019��6��20��
    ��    ��   : ganboqing
    �޸�����   : �޸Ķ�дdgus���̣���ʹ��Ƕ��
*****************************************************************************/



void read_dgus_vp(uint16_t addr, uint8_t *buf, uint16_t len)
{
	uint16_t OS_addr = 0;
	uint16_t OS_addr_offset = 0;
	uint16_t OS_len = 0, OS_len_offset = 0;
	OS_addr = addr >> 1;
	OS_addr_offset = addr & 0x01;
	ADR_H = 0;
	ADR_M = (uint8_t)(OS_addr >> 8);
	ADR_L = (uint8_t)OS_addr;
	ADR_INC = 1;	
	RAMMODE = 0xAF; 
	while (!APP_ACK);				
	if (OS_addr_offset) 
	{
		APP_EN = 1;
		while (APP_EN);
		*buf++ = DATA1;
		*buf++ = DATA0;
		len--;
	}
	OS_len = len >> 1;
	OS_len_offset = len & 0x01;
	while (OS_len--)
	{
		APP_EN = 1;
		while (APP_EN); 
		*buf++ = DATA3;
		*buf++ = DATA2;
		*buf++ = DATA1;
		*buf++ = DATA0;
	}
	if (OS_len_offset)
	{
		APP_EN = 1;
		while (APP_EN); 
		*buf++ = DATA3;
		*buf++ = DATA2;
	}
	RAMMODE = 0x00; 
}

/**************************************************************************   ***
 �� �� ��  : void write_dgus_vp(u32 addr,u8* buf,u16 len)
 ��������  : дdgus��ַ����
�������  :	 addr��д��ֵַ	buf��д������ݱ��滺���� len���ֳ���
 �������  : 
 �޸���ʷ  :
  1.��    ��   : 2019��6��20��
    ��    ��   : ganboqing
    �޸�����   : �޸Ķ�дdgus���̣���ʹ��Ƕ��
*****************************************************************************/
void write_dgus_vp(uint16_t addr, uint8_t *buf, uint16_t len)   
{
	uint16_t OS_addr = 0;
	uint16_t OS_addr_offset = 0;
	uint16_t OS_len = 0, OS_len_offset = 0;
	EA = 0;
	OS_addr = addr >> 1;
	OS_addr_offset = addr & 0x01;
	ADR_H = 0;
	ADR_M = (uint8_t)(OS_addr >> 8);
	ADR_L = (uint8_t)OS_addr;
	ADR_INC = 0x01; 																						//DGUS  OS????????????1
	RAMMODE = 0x83;
	while (!APP_ACK); 																					//??
	if (OS_addr_offset)
	{
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while(APP_EN);
		len--;
	}
	OS_len = len >> 1;
	OS_len_offset = len & 0x01;
	RAMMODE = 0x8F; //?????
	while (OS_len--)
	{
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while (APP_EN);
	}
	if (OS_len_offset)
	{
		RAMMODE = 0x8C;
		DATA3 = *buf++;
		DATA2 = *buf++;
		APP_EN = 1;
		while (APP_EN);
	}
	RAMMODE = 0x00; //?????RAMMODE????
	EA = 1;
}
	
void	write_LED_Config(uint8_t Set_Data,uint8_t Data3)
{
	uint8_t 	led_on[4];
	led_on[0] = Set_Data;
	led_on[1] = 0;
	led_on[2] = 0;
	led_on[3] = Data3;			
	write_dgus_vp(LED_CONFIG,led_on,2);	
}

void	write_PIC_Set(uint8_t PIC)
{
	uint8_t	Value[4];
	Value[0] = 0x5A;
	Value[1] = 0x01;
	Value[2] = 0x00;
	Value[3] = PIC;
	write_dgus_vp(PIC_SET,Value,2);
}

uint16_t	read_PIC_Now(void)
{
	uint16_t PIC;
	read_dgus_vp(PIC_NOW,(uint8_t*)&PIC,1);
	return PIC;
}

void	write_Button_Set(uint8_t Bool)
{
	uint8_t	Value[4];
	Value[0] = 0x5A;
	Value[1] = 0;
	Value[2] = 0;
	if(Bool == 0)
	{
		Value[3] = 0x13;											//��ת90����ʾ
	}else
	{
		Value[3] = 0x1B;											//��ת90����ʾ
	}
			
	write_dgus_vp(0x0080,(uint8_t*)Value,2);
}

void	write_Button(uint8_t Button_Ms)
{
	uint8_t	Value[4];
	Value[0] = 0X0;
	Value[1] = Button_Ms;
	Value[2] = 0X00;
	Value[3] = 0x00;
	write_dgus_vp(WAE_SET,(uint8_t*)Value,2);
}



void	write_Painting(uint8_t	shape)
{
	uint16_t	DrawAdot_Array[8];
	uint8_t	DrawAdot_Status_New_Array[8] = 0;
	static	uint8_t DrawAdot_Status_New[8] = {0};
	uint8_t	byte = 0;
	
	read_dgus_vp(TP_STATUS,DrawAdot_Status_New_Array,4);									//��ȡ����λ��																						//��ȡ����λ��
	byte=strcmp(DrawAdot_Status_New_Array,DrawAdot_Status_New);
	
	if(byte != 0)
	{																																			//�а�������
		DrawAdot_Array[0] = shape;																					//������״																													//����ʵ��0x0004
		DrawAdot_Array[1] = Painting_cmd_byte;																	//��ͼ����																														//������ĸ���
		DrawAdot_Array[2] = (DrawAdot_Status_New_Array[2]<<8|DrawAdot_Status_New_Array[3])-7;								//���Ͻ�x������
		DrawAdot_Array[3] = (DrawAdot_Status_New_Array[4]<<8|DrawAdot_Status_New_Array[5])-7;								//���Ͻ�y������
		DrawAdot_Array[4] = DrawAdot_Array[2]+15;																														//���½�x������
		DrawAdot_Array[5] = DrawAdot_Array[3]+15;																														//���½�y������
		DrawAdot_Array[6] = Draw_Colour;																																		//��ɫ
		DrawAdot_Array[7] = 0xFF00;																																					//����
		write_dgus_vp(Draw_First_Addr,(u8*)DrawAdot_Array,2);	
		write_dgus_vp((Draw_First_Addr+2)+((Painting_cmd_byte-1)*5),(u8*)&DrawAdot_Array[2],6);	
		if(++Painting_cmd_byte >= Painting_Max)																															//����������ۼӵ�1024����ʱ��ո���
		{
			Painting_cmd_byte = 0;
		}
		memcpy(DrawAdot_Status_New,DrawAdot_Status_New_Array,sizeof(DrawAdot_Status_New_Array));										//����λ��
	}
	
	
}

