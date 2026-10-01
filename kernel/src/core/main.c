#include "minemu/boot.h"
#include "minemu/trap.h"
#include "minemu/trace.h"
#include "minemu/uart.h"
#include "minemu/irq.h"

void minemu_kernel_main(const struct minemu_boot_info *boot_info) {
    if ((uintptr_t)boot_info != MINEMU_BOOT_INFO_VADDR ||
        boot_info->magic != MINEMU_BOOT_INFO_MAGIC ||
        boot_info->version != MINEMU_ABI_VERSION ||
        boot_info->size != sizeof(*boot_info) ||
        boot_info->system_rom_base != UINT32_C(0x08000000) ||
        boot_info->direct_map_vaddr != UINT32_C(0xc0000000) ||
        boot_info->direct_map_paddr != UINT32_C(0x40000000) ||
        boot_info->direct_map_size != UINT32_C(0x04000000)) {
        minemu_trace_event(UINT32_C(0xb007bad0));
        minemu_fail_stop();
    }
    minemu_irq_enable();
    uint32_t cpsr_val;
    __asm__ volatile("mrs %0, cpsr" : "=r"(cpsr_val));

    // Check if Bit 7 (IRQ mask) is set. 
    // If (cpsr_val & 0x80) is true, interrupts are STILL locked out by the hardware!
    if (cpsr_val & 0x80) {
        minemu_trace_event(0xDEADBEEF); // Proof that minemu_irq_enable() failed to unmask
    }

    minemu_trace_event(1);

    while (1) {
        __asm__ volatile("wfi");
    }
    //minemu_fail_stop();
}