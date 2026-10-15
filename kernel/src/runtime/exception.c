#include "minemu/irq.h"
#include "minemu/syscall.h"
#include "minemu/trace.h"

#define CPSR_MODE_MASK UINT32_C(0x1f)
#define CPSR_MODE_USR UINT32_C(0x10)
#define CPSR_THUMB UINT32_C(0x20)
#define MINEMU_SYSCALL_ERROR UINT32_C(-1)
#define A32_SVC_ZERO UINT32_C(0xef000000)

void minemu_fail_stop(void) {
    minemu_trace_event(0xDEAD0000);
    for (;;) {
        __asm__ volatile("nop");
    }
}

void minemu_panic(const char *message) {
    minemu_trace_event(0xDEAD0001);
    (void)message;
    minemu_fail_stop();
}

/*
__attribute__((weak, noreturn)) void minemu_svc_trampoline(void) {
    minemu_trace_event(0xDEAD0002);
    minemu_fail_stop();
}*/

__attribute__((weak, noreturn)) void minemu_irq_trampoline(void) {
    minemu_trace_event(0xDEAD0003);
    minemu_fail_stop();
}

struct minemu_trap_frame *minemu_svc_dispatch(struct minemu_trap_frame *frame) {
    if ((frame->spsr & CPSR_MODE_MASK) != CPSR_MODE_USR ||
        (frame->spsr & CPSR_THUMB) != 0 || *(const uint32_t*)(uintptr_t)frame->fault_pc != A32_SVC_ZERO) {
        frame->r[0] = (uint32_t)MINEMU_SYSCALL_ERROR;
        return frame;
    }

    switch (frame->r[7]) {
        case MINEMU_SYSCALL_IOCTL:
            frame->r[0] = (uint32_t)sys_ioctl((int)frame->r[0], (int)frame->r[1], frame->r[2], frame->r[3]);
            break;

        default:
            frame->r[0] = (uint32_t)MINEMU_SYSCALL_ERROR;
            break;
    }

    return frame;
}

__attribute__((weak)) struct minemu_trap_frame *minemu_irq_dispatch(
    struct minemu_trap_frame *frame) {
    minemu_trace_event(0xDEAD0004);
    (void)frame;
    minemu_fail_stop();
}

__attribute__((weak)) void minemu_undefined_dispatch(struct minemu_trap_frame *frame) {
    minemu_trace_event(0xDEAD0005);
    (void)frame;
    minemu_fail_stop();
}

__attribute__((weak)) void minemu_abort_dispatch(struct minemu_trap_frame *frame) {
    minemu_trace_event(0xDEAD0006);
    (void)frame;
    minemu_fail_stop();
}
