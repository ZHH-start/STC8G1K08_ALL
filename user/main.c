#include "main.zhh"

unsigned char flag = 0;

int main()
{
    uint8 command[] = "B1begin"; // 测距命令
    uint8 i         = 0;

    I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {

        delay_ms(100);
        if (rx_receive_string[10] == 1) {
            if (Str_check_test(command, rx_receive_string, 7) == 1)
            UART_SendStr(rx_receive_string);
            UART_SendStr("check pass");
            //     read_date();
            Str_clean(rx_receive_string);
            rx_receive_string[10] = 0; // 清除接收完成标志位
        }
    }
}
