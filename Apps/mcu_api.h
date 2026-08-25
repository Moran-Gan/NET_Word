
#ifndef __MCU_API_H_
#define __MCU_API_H_

void	Connect_Modbus_03_Fountion(uint16_t	addr,uint16_t	len,int16_t	*Data);
void	Connect_Modbus_06_Fountion(uint16_t	addr,int16_t	Data);
void	Connect_SendStr(uint8_t *pstr,uint16_t strlen);

#endif
