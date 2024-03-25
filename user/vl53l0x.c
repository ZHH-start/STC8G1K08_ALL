#include "vl53l0x.zhh"
#include "iic.zhh"
#include "delay.zhh"
#include "uart.zhh"

void vl53l0x_send(uint8 reg, uint8 dat)
{
    I2C_Start();
    I2C_SendByte(address);
    I2C_SendByte(reg);
    I2C_SendByte(dat);
    I2C_Stop();
}

uint8 vl53l0x_read(uint8 reg)
{
    unsigned char num;
    I2C_Start();
    I2C_SendByte(address); // 发送写器件地址
    I2C_SendByte(reg);     // 发送要读取的地址
    I2C_Stop();
    I2C_Start();
    I2C_SendByte(address + 1); // 发送读器件地址
    num = I2C_ReadByte();      // 读取数据
    I2C_Stop();
    return num;
}

short makeuint16(int lsb, int msb)
{
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

unsigned short VL53L0X_decode_vcsel_period(short vcsel_period_reg)
{
    // Converts the encoded VCSEL period register value into the real
    // period in PLL clocks
    unsigned short vcsel_period_pclks = (vcsel_period_reg + 1) << 1;
    return vcsel_period_pclks;
}

void read_date(void)
{
    uint8 gbuf[16];
    // uint16 acnt;
    // uint16 scnt;
    uint16 dist;
    uint8 DeviceRangeStatusInternal;
    
    uint8 val = 0;
    char vall = 0x0d;
    int cnt   = 0;

    vl53l0x_send(VL53L0X_REG_SYSRANGE_START, 0x01);
    while (cnt < 100) {
        // 1 second waiting time max
        delay_ms(10);
        val = vl53l0x_read(VL53L0X_REG_RESULT_RANGE_STATUS);
        if (val & 0x01) break;
        cnt++;
    }

    gbuf[0]  = vl53l0x_read(0x14);
    // gbuf[7]  = vl53l0x_read(0x14 + 7);
    // gbuf[6]  = vl53l0x_read(0x14 + 6);
    // gbuf[9]  = vl53l0x_read(0x14 + 9);
    // gbuf[8]  = vl53l0x_read(0x14 + 8);
    gbuf[11] = vl53l0x_read(0x14 + 11);
    gbuf[10] = vl53l0x_read(0x14 + 10);
    // acnt     = makeuint16(gbuf[7], gbuf[6]);
    // scnt     = makeuint16(gbuf[9], gbuf[8]);
    dist     = makeuint16(gbuf[11], gbuf[10]);

    DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);
    if (DeviceRangeStatusInternal == 11 && dist > 20 && dist < 1200) {
        dist = dist - 50;
        vall = dist % 10000 / 1000;
        UART_SendByte(vall + 0x30);
        vall = dist % 1000 / 100;
        UART_SendByte(vall + 0x30);
        vall = dist % 100 / 10;
        UART_SendByte(vall + 0x30);
        vall = dist % 10 / 1;
        UART_SendByte(vall + 0x30);
        UART_SendByte(0x0d);
        UART_SendByte(0x0a);
    }
    else
    UART_SendStr("out of range\r\n");
}
