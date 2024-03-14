#include "UART.zhh"
#include "vl53l0x.zhh"

int send_data_flag = 0; // 激光测距发送数据标志位，1为发送

void Uart1Init(void) // 115200bps@33.1776.000MHz
{
    P3M0 = (P3M0 & ~0x01) | 0x02;
    P3M1 = (P3M1 & ~0x02) | 0x01;

    SCON = 0x50;  // 8位数据,可变波特率
    AUXR |= 0x40; // 定时器时钟1T模式
    AUXR &= 0xFE; // 串口1选择定时器1为波特率发生器
    TMOD &= 0x0F; // 设置定时器模式
    TL1 = 0xB8;   // 设置定时初始值
    TH1 = 0xFF;   // 设置定时初始值
    ET1 = 0;      // 禁止定时器中断
    TR1 = 1;      // 定时器1开始计时
    ES  = 1;      // 使能串口1中断
    EA  = 1;//使能总中断
}

void UART_SendByte(unsigned char da)
{
    TI   = 0;
    SBUF = da;
    while (!TI)
        ;
    TI = 0;
}

void UART_SendStr(char *p)
{
    unsigned char i;
    for (i = 0; p[i] != '\0'; i++) {
        TI   = 0;
        SBUF = p[i];
        while (!TI)
            ;
        TI = 0;
    }
}

unsigned int Str_length(char *p)
{
    unsigned int i;
    for (i = 0; p[i] != '\0'; i++)
        ;
    return i;
}

unsigned char Str_check(char *str, char *con)
{
    unsigned int i, n = 0;
    unsigned int cnt    = 0;
    unsigned int length = Str_length(con);
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == con[n]) {
            cnt++;
            n++;
            if (cnt == length)
                return 1;
        } else {
            cnt = 0;
            n   = 0;
        }
    }
    return 0;
}

// UART1中断
void UartIsr() interrupt 4
{
    uint8 res;
    static int send_cmd_count;
    if (RI) {
        RI  = 0;
        res = SBUF;
        if (res == 'B') {
            if (send_cmd_count++ > 5)
                send_data_flag = 1;
        } else {
            send_cmd_count = 0;
        }
    }
}
