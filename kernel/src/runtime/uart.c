#include "minemu/platform.h"

void send_hwrld() {
    uint32_t hwrld[12] = {0x104, 0x101, 0x108, 0x108, 0x111, 0x32, 0x119, 0x111, 0x114, 0x108, 0x100, 0x10};
    for(int i = 0; i < 12; i++) { 
        if (MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY) {
            // send a single byte
            MINEMU_UART0->tx_data = hwrld[i];
        }
    }
}