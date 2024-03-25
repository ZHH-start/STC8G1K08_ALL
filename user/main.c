#include "main.zhh"

unsigned char flag = 0;

int main()
{
    uint8 command[]    = "B1begin"; // 测距命令
    uint8 i            = 0;
    uint8 check_result = 0;

    I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {

        delay_ms(100);
        if (rx_receive_string[8] == 1) {
            read_date();
            Str_clean(rx_receive_string);
            rx_receive_string[8] = 0; // 清除接收完成标志位，避免重复读取
        }
        // UART_SendStr("RUN");
    }
}
