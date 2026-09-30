#ifndef MINEMU_UART_H
#define MINEMU_UART_H

#include "minemu/platform.h"

uint32_t minemu_uart0_read();
void minemu_uart0_write(uint32_t);
uint32_t minemu_uart1_read();
void minemu_uart1_write(uint32_t);
void minemu_printf(const char*);
void minemu_uart0_irq_handler();

#endif