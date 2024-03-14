#include "main.zhh"

unsigned char flag = 0;

int main()
{
    Key_init();
    Uart1Init();

    while (1) {
        Key_Scan();
        Key_function();
        delay_ms(50);
    }
}
