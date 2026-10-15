#ifndef MINEMU_SYSCALL_H
#define MINEMU_SYSCALL_H

#include "minemu/trap.h"
#include "minemu/user_abi.h"

/* Students implement this A32 vector trampoline and its syscall contract. */
void minemu_svc_trampoline(void) __attribute__((noreturn));
struct minemu_trap_frame *minemu_svc_dispatch(struct minemu_trap_frame *frame);

int sys_ioctl(int fd, uint32_t op, uint32_t arg0, uint32_t arg1);

#endif
