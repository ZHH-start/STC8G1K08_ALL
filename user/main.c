#include "main.zhh"

unsigned char flag = 0;

int main()
{
    int i=0;
    // I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {

        delay_ms(100);
        if (rx_receive_string[8] == 1) {
            UART_SendStr(rx_receive_string);//发送接收到的字符串
            rx_receive_string[8] = 0;//清除接收完成标志位，避免重复读取
        }
    }
}
