#include "key.zhh"
#include "uart.zhh"

/*
 *Key
 *A --> P54
 *B --> P32
 *C --> P33
 */

#define KEY_PRESS   -1
#define KEY_SILENCE 0
#define KEY_LOOSEN  1

typedef struct {
    char Key_UP_states; //  -1表示按下    0表示静默    1表示松开
    char Key_DOWN_states;
    char Key_LEFT_states;
    char Key_RIGHT_states;

    char Key_UP_last;
    char Key_DOWN_last;
    char Key_LEFT_last;
    char Key_RIGHT_last;
} KEY;

// static KEY idata key;
// extern unsigned char idata hum_max;
static KEY key;

void Key_init()
{
    P3M0 &= ~0x0c;
    P3M1 |= 0x0c;
    P5M0 &= ~0x30;
    P5M1 |= 0x30;

    P55 = 1; // A
    P54 = 1; // B
    P32 = 1; // C
    P33 = 1; // D
}

void Key_Scan()
{
    key.Key_UP_states    = (char)((P5 >> 5) & 0x01) - key.Key_UP_last;
    key.Key_DOWN_states  = (char)((P5 >> 4) & 0x01) - key.Key_DOWN_last;
    key.Key_LEFT_states  = (char)((P3 >> 2) & 0x01) - key.Key_LEFT_last;
    key.Key_RIGHT_states = (char)((P3 >> 3) & 0x01) - key.Key_RIGHT_last;

    key.Key_UP_last    = (P5 >> 5) & 0x01;
    key.Key_DOWN_last  = (P5 >> 4) & 0x01;
    key.Key_LEFT_last  = (P3 >> 2) & 0x01;
    key.Key_RIGHT_last = (P3 >> 3) & 0x01;
}

void Key_function()
{
    // Key up
    switch (key.Key_UP_states) {
        case KEY_PRESS: {
            UART_SendStr("UP_OPEN\r\n");
            break;
        }
        case KEY_SILENCE: {
            break;
        }
        case KEY_LOOSEN: {
            UART_SendStr("UP_LOOSEN\r\n");
            break;
        }
        default: {
            break;
        }
    }
    // Key down
    switch (key.Key_DOWN_states) {
        case KEY_PRESS: {
            UART_SendStr("DOWN_OPEN\r\n");
            break;
        }
        case KEY_SILENCE: {
            break;
        }
        case KEY_LOOSEN: {
            UART_SendStr("DOWN_LOOSEN\r\n");
            break;
        }
        default: {
            break;
        }
    }
    // Key left
    switch (key.Key_LEFT_states) {
        case KEY_PRESS: {
            UART_SendStr("LEFT_OPEN\r\n");
            break;
        }
        case KEY_SILENCE: {
            break;
        }
        case KEY_LOOSEN: {
            UART_SendStr("LEFT_LOOSEN\r\n");
            break;
        }
        default: {
            break;
        }
    }
    // Key right
    switch (key.Key_RIGHT_states) {
        case KEY_PRESS: {
            UART_SendStr("RIGHT_OPEN\r\n");
            break;
        }
        case KEY_SILENCE: {
            break;
        }
        case KEY_LOOSEN: {
            UART_SendStr("RIGHT_LOOSEN\r\n");
            break;
        }
        default: {
            break;
        }
    }
}
