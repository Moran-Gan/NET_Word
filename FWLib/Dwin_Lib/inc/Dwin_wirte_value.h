#ifndef	_Dwin_wirte_value_h_
#define	_Dwin_wirte_value_h_

#define		Painting_Max				999								//最大个数
#define		Draw_Colour					0xF800							//颜色
#define		Draw_First_Addr			0x7A00							//绘画首地址


extern	void read_dgus_vp(uint16_t addr, uint8_t *buf, uint16_t len);
extern	void write_dgus_vp(uint16_t addr, uint8_t *buf, uint16_t len);
extern	void	write_LED_Config(uint8_t Set_Data,uint8_t Data3);
extern	void	write_PIC_Set(uint8_t PIC);
extern	uint16_t	read_PIC_Now(void);
extern	void	write_Button_Set(uint8_t Bool);
extern	void	write_Button(uint8_t Button_Ms);
extern	void	write_Painting(uint8_t	shape);

//extern	int16_t	my_strcmp(int16_t	*s1,int16_t	*s2);


extern	int16_t EXCHANGE_C_F_X10(int16_t temp_CV,int16_t type_CV);
extern	int16_t EXCHANGE_F_C_X10(int16_t temp_CV,int16_t type_CV);
extern	int16_t EXCHANGE_C_F(int16_t temp_CV,int16_t type_CV);
extern	int16_t EXCHANGE_F_C(int16_t temp_CV,int16_t type_CV);
extern	uint16_t crc16(const uint8_t* pucDataBuff, uint16_t usDataLen);
extern	u16 crc16_1(u16	crc_data, u8 *dat, u16 length);

#endif

