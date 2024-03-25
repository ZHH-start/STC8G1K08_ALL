#include "main.zhh"

unsigned char flag = 0;

int main()
{
    uint8 command[] = "a2begin"; // 测距命令
    uint8 i         = 0;

    I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {

        delay_ms(100);
        if (rx_receive_string[8] == 1) {
            if (Str_check_test(command, rx_receive_string, 7) == 1)
                read_date();
            Str_clean(rx_receive_string);
            rx_receive_string[8] = 0; // 清除接收完成标志位，避免重复读取
        }
    }
}
