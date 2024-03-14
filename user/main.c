#include "main.zhh"

unsigned char flag = 0;

int main()
{
    I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {
        if (send_data_flag == 1) {
            read_date();
            send_data_flag = 0; // 表示发送完成，标志位关闭
        }

        delay_ms(500);
    }
}
