#ifndef __TEMP_H
#define __TEMP_H

typedef struct SYS
{                             // ??4????
    u16 valid;                // ??????
    u16 ModbusId;             // Modbus Id
    u16 K0;
    u16 K1;
    u16 D1;
    u16 unused;
} sys_params_t;

extern sys_params_t sys_params;
#define TempSize 100
void Get_Temperature(void);
void T_and_H_adjust_Test();
#endif
