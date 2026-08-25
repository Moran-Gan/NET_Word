#include "DHT83.h"
// #include "common.h"
#include <math.h>

#include "Dwin_CONF.h"
#include "user_include.h"
//-----------命令------------------------------------------------------------------------------------------|
#define DHT83_Write 0x80 // 写命令
#define DHT83_Read  0x81 // 读命令

// 寄存器地址
#define TADDRESS    0x00 // 温度
#define HUMADDRESS  0x01 // 湿度
#define VERADDRESS  0xFF // 传感器型号

//-----------GPIO定义----------|
#define DHT83_I2C_SCL_SETOUT \
    {                        \
        P2MDOUT |= 0x02;     \
    }
#define DHT83_I2C_SDA_SETOUT \
    {                        \
        P2MDOUT |= 0x01;     \
    }
#define DHT83_I2C_SDA_SETIN \
    {                       \
        P2MDOUT &= ~0x01;   \
    }
#define DHT83_SCL_H DHT83_I2C_SCL_PORT = 1;
#define DHT83_SCL_L DHT83_I2C_SCL_PORT = 0;

#define DHT83_SDA_H             \
    {                           \
        P2MDOUT |= 0x01;        \
        DHT83_I2C_SDA_PORT = 1; \
    }
#define DHT83_SDA_L             \
    {                           \
        P2MDOUT |= 0x01;        \
        DHT83_I2C_SDA_PORT = 0; \
    }

#define DHT83_SDA_read (DHT83_I2C_SDA_PORT)

#define DHT8305ID      5
#define DHT8310ID      10
u8 VersionID = 0;

// void DHT83_ReadOneWords(u8 ReadAddr,u8* databuf);				//读出1个字节从DHT83中指定的地址
// void DHT83_ReadTwoWords(u8 ReadAddr,u8* databuf);				//读出2个字节从DHT83中指定的地址
// void DHT83_WriteOneWords(u8 WriteAddr,u8 *DataToWrite);			//写入1个字节到DHT83中指定的地址

// u32 DHT83Temp = 0, DHT83Hum = 0;
// u16 DHT83TempAve = 0, DHT83HumAve = 0;
// u8 DHT83Data[4] = {0};
// u16 DHT83Count = 0;
// u16 DHT83_COMP;
//|---------------------------------------------------|
//|Function		:I2C_Init
//|Description	:initialize I2C GPIO,SCL and SDA
//|Input		:None
//|Output		:None
//|Return		:None
//|---------------------------------------------------|
void DHT83_I2C_Initial(void)
{
    DHT83_I2C_SCL_SETOUT;
    DHT83_I2C_SDA_SETOUT;
    //    DHT83Temp = 0;
    //    DHT83Hum = 0;
    //    DHT83TempAve = 0;
    //    DHT83HumAve = 0;
    //    DHT83Count = 0;
}

/*******************************************************************************
 * Function Name  : I2C_delay
 * Description    : Simulation IIC Timing series delay
 * Input          : None
 * Output         : None
 * Return         : None
 ********************************************************************************/
void DHT83_I2C_delay(void)
{
    u8 i = 200; // 这里可以优化速度	，经测试最低到5还能写入
    while (i)
    {
        i--;
    }
    // 	delay_us(30);
}

void DHT83_delay5ms(void)
{
    u16 i = 60000;
    while (i)
    {
        i--;
    }
    // 	delay_ms(5);
}
/*******************************************************************************
 * Function Name  : I2C_Start
 * Description    : Master Start Simulation IIC Communication
 * Input          : None
 * Output         : None
 * Return         : Wheather	 Start
 ****************************************************************************** */
u8 DHT83_I2C_Start(void)
{
    DHT83_I2C_SDA_SETOUT;
    DHT83_SDA_H;
    DHT83_SCL_H;
    DHT83_I2C_delay();
    if (!DHT83_SDA_read) return 0; // SDA线为低电平则总线忙,退出
    DHT83_SDA_L;
    DHT83_I2C_delay();
    if (DHT83_SDA_read) return 0; // SDA线为高电平则总线出错,退出
    DHT83_SDA_L;
    DHT83_I2C_delay();
    DHT83_SCL_L;
    return 1;
}
/*******************************************************************************
 * Function Name  : I2C_Stop
 * Description    : Master Stop Simulation IIC Communication
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void DHT83_I2C_Stop(void)
{
    DHT83_I2C_SDA_SETOUT;
    DHT83_SCL_L;
    DHT83_I2C_delay();
    DHT83_SDA_L;
    DHT83_I2C_delay();
    DHT83_SCL_H;
    DHT83_I2C_delay();
    DHT83_SDA_H;
    DHT83_I2C_delay();
}
/*******************************************************************************
 * Function Name  : I2C_Ack
 * Description    : Master Send Acknowledge I2C
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void DHT83_I2C_Ack(void)
{
    DHT83_SCL_L;
    DHT83_I2C_delay();
    DHT83_SDA_L;
    DHT83_I2C_delay();
    DHT83_SCL_H;
    DHT83_I2C_delay();
    DHT83_SCL_L;
    DHT83_I2C_delay();
}

/*******************************************************************************
 * Function Name  : I2C_NoAck
 * Description    : Master Send No Acknowledge I2C
 * Input          : None
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void DHT83_I2C_NoAck(void)
{
    DHT83_SCL_L;
    DHT83_I2C_delay();
    DHT83_SDA_H;
    DHT83_I2C_delay();
    DHT83_SCL_H;
    DHT83_I2C_delay();
    DHT83_SCL_L;
    DHT83_I2C_delay();
}

/*******************************************************************************
 * Function Name  : I2C_WaitAck
 * Description    : Master Reserive Slave Acknowledge I2C
 * Input          : None
 * Output         : None
 * Return         : Wheather	 Reserive Slave Acknowledge I2C
 ****************************************************************************** */
u8 DHT83_I2C_WaitAck(void) // 返回为:=1有ACK,=0无ACK
{
    DHT83_I2C_SDA_SETIN;
    DHT83_SCL_L;
    DHT83_I2C_delay();
    DHT83_SCL_H;
    DHT83_I2C_delay();
    if (DHT83_SDA_read)
    {
        DHT83_SCL_L;
        DHT83_I2C_SDA_SETOUT;
        DHT83_I2C_delay();
        return 0;
    }
    DHT83_SCL_L;
    DHT83_I2C_SDA_SETOUT;
    DHT83_I2C_delay();
    return 1;
}

/*******************************************************************************
 * Function Name  : I2C_SendByte
 * Description    : Master Send a Byte to Slave
 * Input          : Will Send Date
 * Output         : None
 * Return         : None
 ****************************************************************************** */
void DHT83_I2C_SendByte(u8 SendByte) // 数据从高位到低位//
{
    u8 i = 8;
    while (i--)
    {
        DHT83_SCL_L;
        DHT83_I2C_delay();
        if (SendByte & 0x80)
        {
            DHT83_SDA_H;
        }
        else
        {
            DHT83_SDA_L;
        }
        SendByte <<= 1;
        DHT83_I2C_delay();
        DHT83_SCL_H;
        DHT83_I2C_delay();
    }
    DHT83_SCL_L;
}

/*******************************************************************************
 * Function Name  : I2C_ReadByte
 * Description    : Master Reserive a Byte From Slave
 * Input          : None
 * Output         : None
 * Return         : Date From Slave
 ****************************************************************************** */
u8 DHT83_I2C_ReadByte(void) // 数据从高位到低位//
{
    u8 i = 8;
    u8 ReceiveByte = 0;
    DHT83_I2C_SDA_SETIN;
    while (i--)
    {
        ReceiveByte <<= 1;
        DHT83_SCL_L;
        DHT83_I2C_delay();
        DHT83_I2C_delay();
        DHT83_I2C_delay();
        DHT83_SCL_H;
        DHT83_I2C_delay();
        DHT83_I2C_delay();
        DHT83_I2C_delay();
        if (DHT83_SDA_read)
        {
            ReceiveByte |= 0x01;
        }
    }
    DHT83_SCL_L;
    DHT83_I2C_delay();
    DHT83_I2C_delay();
    DHT83_I2C_delay();
    return ReceiveByte;
}

//|---------------------------------------------------|
//|Function		:I2C_Write
//|Description	:Write data to SlaveDeive's register
//|Input		:
//|					SlaveAddress:Slave Device's Address
//|					REG_Address :Slave Device's Register Address
//|					REG_data	:Data to be wrote into register
//|Output		:None
//|Return		:u8 data of slave device's register
//|---------------------------------------------------|
u8 DHT83_I2C_Write(u8 SlaveAddress, u8 REG_Address, u8 REG_data) // void
{
    if (!DHT83_I2C_Start()) return 0;
    DHT83_I2C_SendByte(SlaveAddress); // 发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址
    DHT83_I2C_WaitAck();
    DHT83_I2C_SendByte(REG_Address); // 设置低起始地址
    DHT83_I2C_WaitAck();
    DHT83_I2C_SendByte(REG_data);
    DHT83_I2C_WaitAck();
    DHT83_I2C_Stop();
    //delay_ms(5);
    return 1;
}

//|---------------------------------------------------------------------|
//|Function		:I2C_Read
//|Description	:Read I2C_Device's Register
//|Input		:
//|					SlaveAddress:Slave Device's Address
//|					REG_Address :Slave Device's Register Address
//|Output		:None
//|Return		:u8  value of Register
//|---------------------------------------------------------------------|
u8 DHT83_I2C_Read(u8 SlaveAddress, u8 REG_Address)
{
    u8 REG_data;
    DHT83_I2C_Start();
    DHT83_I2C_SendByte(SlaveAddress); // I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址
    DHT83_I2C_WaitAck();
    DHT83_I2C_SendByte((u8)REG_Address); // 设置低起始地址
    DHT83_I2C_WaitAck();
    DHT83_I2C_Start();
    DHT83_I2C_SendByte(SlaveAddress + 1);
    DHT83_I2C_WaitAck();

    REG_data = DHT83_I2C_ReadByte();
    DHT83_I2C_NoAck();
    DHT83_I2C_Stop();
    return REG_data;
}

void DHT83_I2C_ReadRegister(u8 startaddr, u8* regbuf, u8 num)
{
    u8 i = 0;
    for (i = 0; i < num; i++, regbuf++, startaddr++)
    {
        *regbuf = DHT83_I2C_Read(0x26, startaddr);
    }
}

void DHT83_ReadOneWords(u8 ReadAddr, u8* databuf)
{
    // 	u16 temp;
    DHT83_I2C_Start();
    DHT83_I2C_SendByte(DHT83_Write); // 发送写命令
    // DHT83_I2C_WaitAck();
    if (!DHT83_I2C_WaitAck()) return;
    DHT83_I2C_SendByte(ReadAddr); // 发送需要写的目的地址
    // DHT83_I2C_WaitAck();
    if (!DHT83_I2C_WaitAck()) return;
    DHT83_I2C_Stop(); // 产生一个停止条件

		DHT83_delay5ms();

    DHT83_I2C_Start();
    DHT83_I2C_SendByte(DHT83_Read); // 写入 写命令,进入接收模式
    DHT83_I2C_WaitAck();
    databuf[0] = DHT83_I2C_ReadByte(); // 接收数据
    DHT83_I2C_Ack();
    databuf[1] = DHT83_I2C_ReadByte(); // 接收数据
    DHT83_I2C_Stop();                  // 停止

    // 	return temp;					//返回读出的数据
}
void DHT83_ReadTwoWords(u8 ReadAddr, u8* databuf)
{
    // 	u16 temp;
    DHT83_I2C_Start();
    DHT83_I2C_SendByte(DHT83_Write); // 发送写命令
    if (!DHT83_I2C_WaitAck()) return;
    DHT83_I2C_SendByte(ReadAddr); // 发送需要写的目的地址
    if (!DHT83_I2C_WaitAck()) return;
    DHT83_I2C_Stop(); // 产生一个停止条件

    //delay_ms(20);

    DHT83_I2C_Start();
    DHT83_I2C_SendByte(DHT83_Read); // 写入 写命令,进入接收模式
    if (!DHT83_I2C_WaitAck()) return;
    databuf[0] = DHT83_I2C_ReadByte(); // 接收数据
    DHT83_I2C_Ack();
    databuf[1] = DHT83_I2C_ReadByte(); // 接收数据
    DHT83_I2C_Ack();
    databuf[2] = DHT83_I2C_ReadByte(); // 接收数据
    DHT83_I2C_Ack();
    databuf[3] = DHT83_I2C_ReadByte(); // 接收数据
    DHT83_I2C_Stop();                  // 停止

    // 	return temp;					//返回读出的数据
}
void DHT83_WriteOneWords(u8 WriteAddr, u8* DataToWrite)
{
    // 	u8 dataW[2] = {0};
    // 	dataW = DataToWrite;
    DHT83_I2C_Start();                  // 开始
    DHT83_I2C_SendByte(DHT83_Write);    // 发送写命令
    DHT83_I2C_WaitAck();                // 等待VCNL返回一个ACK
    DHT83_I2C_SendByte(WriteAddr);      // 发送需要写的目的地址
    DHT83_I2C_WaitAck();                // 等待VCNL返回一个ACK
    DHT83_I2C_SendByte(DataToWrite[0]); // 发送要写入的地址
    DHT83_I2C_WaitAck();                // 等待VCNL返回一个ACK
    DHT83_I2C_SendByte(DataToWrite[1]); // 发送要写入的地址
    DHT83_I2C_WaitAck();                // 等待VCNL返回一个ACK
    DHT83_I2C_Stop();                   // 产生一个停止条件
    //delay_ms(10);
}

void VersionIDQR()
{
    u8 result[2];
    // u16 temp;
    DHT83_ReadOneWords(VERADDRESS, result);
    // temp = result[0] * 256 + result[1];
    // write_dgus_vp(0x9000, (u8*)&temp, 1);
    if (result[0] == 0x83 && result[1] == 0x05)
    {
        VersionID = DHT8305ID;
    }
    else if (result[0] == 0x82 && result[1] == 0x15)
    {
        VersionID = DHT8310ID;
    }

    // {
    // 	u16 temp1 = VersionID;
    // 	write_dgus_vp(0x6008,(u8*)&temp1,1);
    // }
}

//----------初始化----------
void DHT83_Init(void)
{
    DHT83_I2C_Initial(); // 初始化I2C引脚
    VersionID = 0;
}

/**
 * @brief
 * @return    T_CHT83 {unsigned int*} CHT83XX的温度值
 * @return    H_CHT83 {unsigned int*} CHT83XX的湿度值
 * @param    T_env {unsigned int} 环境温度
 * @param    D1 {unsigned int} 校准系数
 * @return   {unsigned int} 返回校准后的环境湿度值
 */
unsigned int DHT83_MeasurementResult(unsigned int* T_CHT83, unsigned int* H_CHT83, unsigned int T_env, unsigned int D1) // 获取测量结果
{
    float Pws_T_Jt, Pws_T_St;
    float Hum_jiao = 0;
    float T_JTemp = (float)T_env / 10.0f;
    float Hum_Sensor;
    float T_Sensor;
    u8 result[4];
    unsigned int DHT83_T = 0, DHT83_Hum = 0;
    if (VersionID == DHT8310ID)
    {
        DHT83_ReadOneWords(TADDRESS, result);
        DHT83_T = ((unsigned int)result[0] << 8 | result[1]);
        T_Sensor = (float)(DHT83_T >> 1) / 128.0;
        DHT83_ReadOneWords(HUMADDRESS, result);
        DHT83_Hum = ((unsigned int)result[0] << 8 | result[1]);
        DHT83_Hum &= 0x7FFF;
        Hum_Sensor = (float)DHT83_Hum / 32768.0 * 100;
    }
    else if (VersionID == DHT8305ID)
    {
        DHT83_ReadTwoWords(TADDRESS, result);
        DHT83_T = (unsigned int)result[0] << 8 | result[1];
        DHT83_Hum = (unsigned int)result[2] << 8 | result[3];
        T_Sensor = (float)DHT83_T / 397.0f - 40.0f;
        Hum_Sensor = (float)DHT83_Hum / 655.35f;
    }
    else
    {
        Hum_jiao = 0xFFFF;
        VersionIDQR();
        return 0;
    }
    if (T_CHT83 != NULL)
    { // 中心温度
        *T_CHT83 = T_Sensor * 10;
    }
    if (H_CHT83 != NULL)
    { // 边缘温度
        *H_CHT83 = Hum_Sensor * 10;
    }

    if (Hum_jiao == 0xFFFF)
    {
        DHT83_Hum = 0;
    }
    else
    {
        Pws_T_St = 6.112f * exp(17.67f * T_Sensor / (T_Sensor + 243.5f));
        Pws_T_Jt = 6.112f * exp(17.67f * T_JTemp / (T_JTemp + 243.5f));
        Hum_jiao = Hum_Sensor * Pws_T_St / Pws_T_Jt;
        DHT83_Hum = (unsigned int)(Hum_jiao * 10) + D1;
        if (DHT83_Hum >= 950)
        {
            DHT83_Hum = 950;
        }
    }
    return DHT83_Hum;
}
