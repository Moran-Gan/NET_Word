#ifndef __DHT83_H
#define __DHT83_H

//#include "sys.h"


// sbit DHT83_I2C_ALARM_PORT=P2^7;			//INT端口数值引脚定义，注意引脚的数字IO口属性修改

//extern u8 VersionID; // 传感器型号

/**
 * @brief    初始化DHT83传感器
 */
void DHT83_Init(void);


/**
 * @brief
 * @return    T_CHT83 {unsigned int*} CHT83XX的温度值
 * @return    H_CHT83 {unsigned int*} CHT83XX的湿度值
 * @param    T_env {unsigned int} 环境温度
 * @param    D1 {unsigned int} 校准系数
 * @return   {unsigned int} 返回校准后的环境湿度值
 */
unsigned int DHT83_MeasurementResult(unsigned int* T_CHT83, unsigned int* H_CHT83, unsigned int T_env, unsigned int D1);

#endif
