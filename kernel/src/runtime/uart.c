#include "minemu/platform.h"

void send_hwrld() {
    uint32_t hwrld[12] = {0x68, 0x65, 0x65, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x0A};
    for(int i = 0; i < 12; i++) { 
        while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY));
        if (MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY) {
            // send a single byte
            MINEMU_UART0->tx_data = (uint32_t)hwrld[i];
        }
    }
}