#include "minemu/boot.h"
#include "minemu/trap.h"
#include "minemu/trace.h"
#include "minemu/uart.h"
#include "minemu/platform.h"

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
    //minemu_printf("hello world\n");
    //minemu_printf("hi jorge\n");
    while(1) {
    while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY));
    uint32_t read = MINEMU_UART0->rx_data;
    while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY));
    MINEMU_UART0->tx_data = read;
    }
    minemu_trace_event(1);
    minemu_fail_stop();
}
