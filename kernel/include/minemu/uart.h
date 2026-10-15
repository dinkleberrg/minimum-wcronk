#ifndef MINEMU_UART_H
#define MINEMU_UART_H

#include "minemu/platform.h"

int minemu_uart0_read(char* dst, uint32_t n);
int minemu_uart0_write(const char* src, uint32_t n);
int minemu_uart1_read(char* dst, uint32_t n);
int minemu_uart1_write(const char* src, uint32_t n);
void minemu_printf(const char*);
char uart_buf_pop();
void minemu_uart0_irq_handler();

#endif