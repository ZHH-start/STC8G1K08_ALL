#include "UART.zhh"

int send_data_flag         = 0;   // 激光测距发送数据标志位，1为发送
uint8 rx_receive_string[8] = {0}; // 前7位用于存放接收数据，rx_receive_string[8]=1接收完成

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
    EA  = 1;      // 使能总中断
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

unsigned char Str_check_test(char *str, char *con, uint8 number)
{
    unsigned int i   = 0;
    unsigned int cnt = 0;
    for (i = 0; i < number; i++) {
        if (str[i] == con[i]) {
            // UART_SendStr("check\r\n");
            cnt++;
        }
    }
    if (cnt == number)
        return 1;
    else
        return 0;
}

void Str_clean(char *str)
{
    int i = 0;
    for (i = 0; i < 7; i++) {
        str[i] = 0;
    }
}

// UART1中断
void UartIsr() interrupt 4
{
    static int rx_count = 0;
    if (RI) {
        RI = 0;
        if (rx_count >= 7) {
            rx_count             = 0;
            rx_receive_string[8] = 1;
        }
        if (rx_count < 7) {
            rx_receive_string[rx_count] = SBUF;
            rx_count++;
        }
    }
}
