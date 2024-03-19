#include "main.zhh"

unsigned char flag = 0;

int main()
{
    uint8 command[] = "a2begin"; // 测距命令
    
    I2C_Init();
    Uart1Init();

    UART_SendStr("init done");

    while (1) {
        if (rx_receive_string[8] == 1) {
            if (Str_check(command, rx_receive_string)) {
                read_date();
            }
            rx_receive_string[8] = 0; // 清除接收完成标志位，避免重复读取
        }
        delay_ms(100);
    }
}
