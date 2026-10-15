#include <stdint.h>
#include "ioctl.h"

void minemu_user_main(void) {
    //ioctl(MINEMU_TRACE_BASE, 0U, 0x1234U);
    ioctl(MINEMU_UART0_BASE, 1U, "Hello World\n\0", 12);

    for (;;) {
        __asm__ volatile("nop");
    }
}