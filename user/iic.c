#include "iic.zhh"
#include "delay.zhh"

/*******************************************************************************
 * 函 数 名         : I2C_Init()
 * 函数功能		   : 初始化i2c需要用到的P32，P33
 * 输    入         : 无
 * 输    出         : 无
 * 备    注         : 起始之后I2C_SDA和I2C_SCL都为0
 *******************************************************************************/
void I2C_Init()
{
    P3M0 = (P3M0 & ~0x08) | 0x04;
    P3M1 &= ~0x0c;
}

/*******************************************************************************
 * 函 数 名         : I2C_Start()
 * 函数功能		   : 起始信号：在I2C_SCL时钟信号在高电平期间I2C_SDA信号产生一个下降沿
 * 输    入         : 无
 * 输    出         : 无
 * 备    注         : 起始之后I2C_SDA和I2C_SCL都为0
 *******************************************************************************/
void I2C_Start()
{
    I2C_SDA = 1;
    delay_us(5);
    I2C_SCL = 1;
    delay_us(5); // 建立时间是I2C_SDA保持时间>4.7us
    I2C_SDA = 0;
    delay_us(5); // 保持时间是>4us
    I2C_SCL = 0;
    delay_us(5);
}
/*******************************************************************************
 * 函 数 名           : I2C_Stop()
 * 函数功能	         : 终止信号：在I2C_SCL时钟信号高电平期间I2C_SDA信号产生一个上升沿
 * 输    入           : 无
 * 输    出         	 : 无
 * 备    注           : 结束之后保持I2C_SDA和I2C_SCL都为1；表示总线空闲
 *******************************************************************************/
void I2C_Stop()
{
    I2C_SDA = 0;
    delay_us(5);
    I2C_SCL = 1;
    delay_us(5); // 建立时间大于4.7us
    I2C_SDA = 1;
    delay_us(5);
}
/*******************************************************************************
 * 函 数 名           : I2cSendByte(uint8 num)
 * 函数功能 	         : 通过I2C发送一个字节。在I2C_SCL时钟信号高电平期间，
 *                    * 保持发送信号I2C_SDA保持稳定
 * 输    入           : num ,ack
 * 输    出         	 : 0或1。发送成功返回1，发送失败返回0
 * 备    注           : 发送完一个字节I2C_SCL=0, 需要应答则应答设置为1，否则为0
 *******************************************************************************/
uint8 I2C_SendByte(uint8 dat)
{
    uint8 a = 0, b = 0; // 最大255，一个机器周期为1us，最大延时255us。

    for (a = 0; a < 8; a++) // 要发送8位，从最高位开始
    {
        dat <<= 1;
        I2C_SDA = CY;
        delay_us(5);
        I2C_SCL = 1;
        delay_us(5); // 建立时间>4.7us
        I2C_SCL = 0;
        delay_us(5); // 时间大于4us
    }

    // I2C_SDA = 1;
    // delay_us(5);
    I2C_SCL = 1;
    delay_us(5);
    CY      = I2C_SDA;
    I2C_SCL = 0;
    delay_us(5);
    return CY;
}
/*******************************************************************************
 * 函 数 名           : I2cReadByte()
 * 函数功能	    	 : 使用I2c读取一个字节
 * 输    入           : 无
 * 输    出         	 : dat
 * 备    注           : 接收完一个字节I2C_SCL=0
 *******************************************************************************/
uint8 I2C_ReadByte()
{
    uint8 a = 0, dat = 0;
    I2C_SDA = 1; // 起始和发送一个字节之后I2C_SCL都是0
    delay_us(5);
    for (a = 0; a < 8; a++) // 接收8个字节
    {
        I2C_SCL = 1;
        delay_us(5);
        dat <<= 1;
        dat |= I2C_SDA;
        delay_us(5);
        I2C_SCL = 0;
        delay_us(5);
    }
    return dat;
}
// void I2C_SendACK(bit ack)
//{
//     I2C_SDA=ack;                  //写应答信号
//     I2C_SCL = 1;                   //拉高时钟线
//     delay_us(5);                 //延时
//     I2C_SCL = 0;                    //拉低时钟线
//     delay_us(5);                //延时
// }