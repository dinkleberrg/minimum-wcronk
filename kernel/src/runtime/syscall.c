#include "minemu/syscall.h"
#include "minemu/uart.h"
#include "minemu/trace.h"
#include "minemu/platform.h"

int sys_ioctl(int fd, uint32_t op, uint32_t arg0, uint32_t arg1) {
    switch (fd) {
        case MINEMU_RNG_BASE:
            if (op == 0) {
                MINEMU_RNG->seed = arg0;
                return 0;
            } else if (op ==1 ) {
                *(uint32_t*)arg0 = MINEMU_RNG->data;
                return 0;
            }
            return MINEMU_IOCTL_ERROR;

        case MINEMU_UART0_BASE:
            if (op == 0)
                return minemu_uart0_read((char*)arg0, arg1);
            else if (op == 1)
                return minemu_uart0_write((const char*)arg0, arg1);  
            return MINEMU_IOCTL_ERROR; 

        case MINEMU_UART1_BASE:
            if (op == 0)
                return minemu_uart1_read((char*)arg0, arg1);
            else if (op == 1)
                return minemu_uart1_write((const char*)arg0, arg1);  
            return MINEMU_IOCTL_ERROR;

        case MINEMU_TRACE_BASE:
            minemu_trace_event(arg0);
            return 0;

        default:
            return MINEMU_IOCTL_ERROR;
    }
}