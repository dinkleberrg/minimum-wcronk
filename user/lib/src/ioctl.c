#include <stdarg.h>
#include <stdint.h>

#include "minemu/user_abi.h"
#include "minemu/platform.h"

static int32_t ioctl_svc(int fd, uint32_t request, uint32_t argument0,
                         uint32_t argument1) {
    register uint32_t r0 __asm__("r0") = (uint32_t)fd;
    register uint32_t r1 __asm__("r1") = request;
    register uint32_t r2 __asm__("r2") = argument0;
    register uint32_t r3 __asm__("r3") = argument1;
    register uint32_t r7 __asm__("r7") = MINEMU_SYSCALL_IOCTL;

    __asm__ volatile("svc #0"
                     : "+r"(r0)
                     : "r"(r1), "r"(r2), "r"(r3), "r"(r7)
                     : "memory", "cc");
    return (int32_t)r0;
}

int32_t ioctl(int fd, uint32_t request, ...) {
    uint32_t argument0 = 0;
    uint32_t argument1 = 0;
    va_list arguments;

    va_start(arguments, request);
    switch ((uint32_t)fd) {
    case MINEMU_UART0_BASE:
    case MINEMU_UART1_BASE:
        if (request == 0U || request == 1U) {
            argument0 = (uint32_t)(uintptr_t)va_arg(arguments, void *);
            argument1 = va_arg(arguments, uint32_t);
        }
        break;

    case MINEMU_RNG_BASE:
        if (request == 0U)
            argument0 = va_arg(arguments, uint32_t);
        else if (request == 1U)     /* read: uint32_t *out */
            argument0 = (uint32_t)(uintptr_t)va_arg(arguments, uint32_t *);
        break;

    case MINEMU_TRACE_BASE:
        argument0 = va_arg(arguments, uint32_t);
        break;

    default:
        break;
    }
    va_end(arguments);
    return ioctl_svc(fd, request, argument0, argument1);
}
