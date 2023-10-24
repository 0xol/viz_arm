#ifndef _VIZ_MM_PMM_H
#define _VIZ_MM_PMM_H

#include <stdint.h>
#include <stddef.h>
#include <boot/limine/limine.h>

extern volatile struct limine_memmap_request mem_req;
#define PAGE_SIZE (4096)

void pmm_init(void);
void* pmm_alloc(size_t pages);
void pmm_free(void* addr, size_t pages);

#endif