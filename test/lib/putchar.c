#include "rv32i.h"

void _putchar(char c) {
    char str[2] = {c, '\0'};
    uart_print(str);
}
