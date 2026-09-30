#include "minemu/uart.h"

uint32_t minemu_uart0_read() {
    while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY));
    return MINEMU_UART0->rx_data;
}

void minemu_uart0_write(uint32_t data) {
    while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY));
    MINEMU_UART0->tx_data = data;
}

uint32_t minemu_uart1_read() {
    while (!(MINEMU_UART1->status & MINEMU_UART_STATUS_RX_READY));
    return MINEMU_UART1->rx_data;
}

void minemu_uart1_write(uint32_t data) {
    while (!(MINEMU_UART1->status & MINEMU_UART_STATUS_TX_READY));
    MINEMU_UART1->tx_data = data;
}

void minemu_printf(const char* string) {
    int length = 0;
    while (string[length] != '\0')
        length++;
    for (int i = 0; i < length; i++) {
        minemu_uart0_write((uint32_t)string[i]);
    }
}