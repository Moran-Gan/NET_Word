#include "Dwin_CONF.h"
#include "user_include.h"
#include "DHT83.h"
#include "string.h"
const u16 code TabNTC_10k[TempSize] = {
    32040, 30490, 29022, 27633, 26317, 25071, 23889, 22769, 21707, 20700, 19788, 18838, 17977, 17160, 16383, 15646, 14945, 14280, 13647, 13045,
    12472, 11928, 11409, 10916, 10447, 10000, 9574, 9168, 8781, 8413, 8062, 7727, 7407, 7103, 6812, 6534, 6270, 6017, 5775, 5545,
    5324, 5114, 4913, 4720, 4536, 4360, 4192, 4031, 3877, 3730, 3572, 3454, 3324, 3201, 3082, 2968, 2859, 2755, 2654, 2558,
    2466, 2378, 2293, 2212, 2134, 2059, 1987, 1918, 1851, 1788, 1726, 1668, 1611, 1557, 1504, 1454, 1406, 1359, 1314, 1271,
    1230, 1190, 1151, 1114, 1079, 1045, 1011, 980, 949, 919, 891, 863, 837, 811, 786, 763, 740, 718, 696, 675};

// adc����ֵ
u16 adc_value[3] = {0};
// ADC��ֵ�ݴ�
u16 adc_val[8] = {0};
//����ֵ
u16 R1_value = 0; // ADC7��NTC����
u16 R2_value = 0; // ADC0��NTC����
//�¶�ֵ
u16 R1_Temperature = 0;
u16 R2_Temperature = 0;
//�¶�ֵ  ��adc���ü���Ĵ�
u16 Temperature_Real = 0;

void Get_ADC_Value(void)
{
    static u16 temp[8] = {0};
    read_dgus_vp(AD_VALUE, (u8*)temp, 8);
    adc_val[0] += temp[0];
    adc_val[6] += temp[6];
    adc_val[7] += temp[7];
}

void Get_R_Value(u16 n)
{
    float R1_temp = 0.0, R2_temp = 0.0;
    adc_value[0] = adc_val[0] / n;
    adc_value[6] = adc_val[6] / n;
    adc_value[7] = adc_val[7] / n;
    R1_temp = adc_value[7] * 1.0 / (4095 - adc_value[7]); // ad7
    R2_temp = adc_value[6] * 1.0 / (4095 - adc_value[6]); // ad6
    R1_value = R1_temp * 10000;
    R2_value = R2_temp * 10000;
}

u8 FindTab(u16* pTab, u8 Tablong, u16 dat)
{
    u8 st = 0, ed = 0, m = 0;
    u8 i = 0;
    ed = TempSize - 1;
    if (dat >= pTab[st])
        return st;
    else if (dat <= pTab[ed])
        return ed;
    while (st < ed)
    {
        m = (st + ed) / 2;
        if (dat == pTab[m])
            break;
        if ((dat < pTab[m]) && (dat > pTab[m + 1]))
            break;
        if (dat > pTab[m])
            ed = m;
        else
            st = m;
        if (i++ > Tablong)
            break;
    }
    if (st > ed)
        return 0;
    return m;
}

void Get_Temperature(void)
{
    static u16 i = 0;
    float temp1 = 0, temp2 = 0;
    int16_t temp_compensation = 0;
    Get_ADC_Value();
    i++;
    if (i >= 3)
    {

        Get_R_Value(3);
        R1_Temperature = FindTab(TabNTC_10k, TempSize, R1_value);
        R2_Temperature = FindTab(TabNTC_10k, TempSize, R2_value);
        temp1 = (TabNTC_10k[R1_Temperature] - R1_value) * 1.0 / (TabNTC_10k[R1_Temperature] - TabNTC_10k[R1_Temperature + 1]);
        temp2 = (TabNTC_10k[R2_Temperature] - R2_value) * 1.0 / (TabNTC_10k[R2_Temperature] - TabNTC_10k[R2_Temperature + 1]);
        R1_Temperature = R1_Temperature * 10 + temp1 * 10;
        R2_Temperature = R2_Temperature * 10 + temp2 * 10;
        write_dgus_vp(R1_TEMP, (u8*)&R1_Temperature, 1);
        write_dgus_vp(R2_TEMP, (u8*)&R2_Temperature, 1);

        read_dgus_vp(0x3e, (u8*)&Temperature_Real, 1);
        Temperature_Real = Temperature_Real ;    // NOTE:ƫ��0.5���϶ȣ��ɵ���
        //Temperature_Real = 255;
        //write_dgus_vp(TEMP_REAL, (u8*)&Temperature_Real, 1);
        temp_compensation = Read_VP(INDOOR_TEMP_COMPENSATION);
        temp_compensation = (temp_compensation*10)+Temperature_Real;
        Wite_VP(TEMP_REAL, temp_compensation);
        memset(adc_val, 0, sizeof(adc_val));
        i = 0;
    }
}

sys_params_t sys_params;
//void T_and_H_adjust_Test()
//{
//    // 温度校准测试
//    u16  T_env; //edge_T, certer_T, T_3E, T_standard
//    int16_t T_error, H_error;
//    uint16_t H_env, H_standard, T_8310, H_8310;

//    u16 parms[3];
//    read_dgus_vp(0X600B, (u8*)parms, 3);
////    if (parms[0] != sys_params.K1 || parms[1] != sys_params.D1 || parms[2] != sys_params.K0)
////    {
////        sys_params.K1 = parms[0];
////        sys_params.D1 = parms[1];
////        sys_params.K0 = parms[2];
////        //sys_params_save();
////    }

////    T_env = Get_Temperature(&certer_T, &edge_T, sys_params.K1, sys_params.D1);
////    write_dgus_vp(0X6000, (u8*)&T_env, 1);
////    write_dgus_vp(0X6001, (u8*)&certer_T, 1);
////    write_dgus_vp(0X6002, (u8*)&edge_T, 1);

////    read_dgus_vp(0x3e, (u8*)&T_3E, 1);
////    write_dgus_vp(0x6006, (u8*)&T_3E, 1);

////    read_dgus_vp(0x600E, (u8*)&T_standard, 1);

////    T_error = (s16)T_env - T_standard;
////    write_dgus_vp(0X6008, (u8*)&T_error, 1);

//    // 湿度校准测试
//    H_env = DHT83_MeasurementResult(&T_8310, &H_8310, T_env, sys_params.K0);
//    write_dgus_vp(0X6004, (u8*)&T_8310, 1);
//    write_dgus_vp(0X6005, (u8*)&H_8310, 1);
//    write_dgus_vp(0X6003, (u8*)&H_env, 1);

//    read_dgus_vp(0X600F, (u8*)&H_standard, 1);

//    H_error = (int16_t)H_env - H_standard;
//    write_dgus_vp(0X6009, (u8*)&H_error, 1);

//    {
//        // 温湿度误差曲线
//        static u16 cnt = 0;
//        u8 tmp[16];

//        cnt++;
//        if (cnt >= 20)
//        {
//            cnt = 0;

//            if (T_error > 30)
//                T_error = 30;
//            else if (T_error < -30)
//                T_error = -30;
//            T_error += 30;

//            if (H_error > 90)
//                H_error = 90;
//            else if (H_error < -90)
//                H_error = -90;
//            H_error += 90;

//            tmp[0] = 0x5a;
//            tmp[1] = 0xa5;
//            tmp[2] = 0x02;
//            tmp[3] = 0;
//            tmp[4] = 0;
//            tmp[5] = 1;
//            tmp[6] = (u8)(T_error >> 8);
//            tmp[7] = (u8)(T_error);
//            tmp[8] = 1;
//            tmp[9] = 1;
//            tmp[10] = (u8)(H_error >> 8);
//            tmp[11] = (u8)(H_error);
//            write_dgus_vp(0x310, tmp, 6);
//        }
//    }
//}
