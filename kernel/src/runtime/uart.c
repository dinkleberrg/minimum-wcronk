#include "minemu/uart.h"
#include "minemu/irq.h"

#define BUFFER_SIZE 256

static char uart_rx_buf[BUFFER_SIZE];
static uint32_t rx_buf_head = 0;
static uint32_t rx_buf_tail = 0;

int minemu_uart0_read(char* dst, uint32_t n) {
    uint32_t i = 0;
    while (MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY && i < n) {
        dst[i++] = (char)MINEMU_UART0->rx_data;
    }
    if (i == 0)
        return -1;
    return (int)i;
}

int minemu_uart0_write(const char* src, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY));
        MINEMU_UART0->tx_data = (uint32_t)(src[i]);
    }
    return 0;
}

int minemu_uart1_read(char* dst, uint32_t n) {
    uint32_t i = 0;
    while (MINEMU_UART1->status & MINEMU_UART_STATUS_RX_READY && i < n) {
        dst[i++] = (char)MINEMU_UART1->rx_data;
    }
    if (i == 0)
        return -1;
    return (int)i;
}

int minemu_uart1_write(const char* src, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        while (!(MINEMU_UART1->status & MINEMU_UART_STATUS_TX_READY));
        MINEMU_UART1->tx_data = (uint32_t)(src[i]);
    }
    return 0;
}

void minemu_printf(const char* string) {
    uint32_t length = 0;
    while (string[length] != '\0')
        length++;
    for (uint32_t i = 0; i < length; i++) {
        minemu_uart0_write(string + i, 1);
    }
}

char uart_buf_pop() {
    minemu_irq_disable();
    if (rx_buf_head == rx_buf_tail) {
        //minemu_printf("\nERROR: Cannot read UART buffer, it is empty\n");
        minemu_irq_enable();
        return '\0';
    }
    char data = uart_rx_buf[rx_buf_tail];
    rx_buf_tail = (rx_buf_tail + 1) % BUFFER_SIZE;
    minemu_irq_enable();
    return data;
}

void minemu_uart0_irq_handler() {
    while(MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY) {
        char c = (char)MINEMU_UART0->rx_data;
        if ((rx_buf_head + 1) % BUFFER_SIZE == rx_buf_tail) {
            //minemu_printf("\nWARNING: UART buffer full\n");
            return;
        }
        uart_rx_buf[rx_buf_head] = c;
        rx_buf_head = (rx_buf_head + 1) % BUFFER_SIZE;
    }
    //minemu_printf("\nIRQ called\n");
}