#include "main.zhh"

unsigned char flag = 0;

int main()
{
    uint8 command[] = "A03begin"; // 测距命令

    I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {
        delay_ms(100);
        if (rx_receive_string[10] == 1) {
            if (Str_check_test(command, rx_receive_string, 7) == 1)
                read_date();
            rx_receive_string[10] = 0; // 清除接收完成标志位
        }
    }
}
