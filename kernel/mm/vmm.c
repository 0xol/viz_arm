#include <mm/vmm.h>
#include <mm/pmm.h>

#include <stddef.h>
#include <stdint.h>

#include <lib/panic.h>

#include <boot/limine/limine.h>

volatile struct limine_kernel_address_request kernel_addr = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};


void vmm_init(void) {
    

    PANIC("VMM");
}