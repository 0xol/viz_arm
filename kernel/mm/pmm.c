#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <boot/limine/limine.h>

#include <lib/lock.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <lib/math.h>
#include <lib/string.h>

#include <mm/pmm.h>

volatile struct limine_memmap_request mem_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};
volatile struct limine_hhdm_request hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .response = 0
};

static struct lock_t lock;
static uint64_t free_pages = 0;
static uint8_t* bitmap = NULL;
static uint64_t last_indexed = 0;
static uint64_t highest_page = 0;

#define memmap (mem_req.response)
#define hhdm (hhdm_req.response)
#define bitmap_size DIV_UP(free_pages, 8)
#define test(bit) (bitmap[bit / 8] & (1 << (bit % 8)))
#define set(bit) (bitmap[bit / 8] |= (1 << (bit % 8)))

void pmm_init(void) {
    lock_init(&lock);
    lock_acquire(&lock);

    if ((void*)&mem_req == NULL) {PANIC("MEMMAP REQUEST NULL");}
    if ((void*)&hhdm_req == NULL) {PANIC("HHDM REQUEST NULL");}

    uint64_t highest_addr = 0;

    for (size_t x = 0; x < memmap->entry_count; x++) {
        #define entry (memmap->entries[x])

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            free_pages += entry->length / PAGE_SIZE;
            if (entry->base > highest_addr) {highest_addr = entry->base;}
        }
    }

    for (size_t x = 0; x < memmap->entry_count; x++) {
        #define entry (memmap->entries[x])

        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length > bitmap_size) {
            bitmap = (uint8_t*)entry->base;
            print("placing bitmap @ 0x%p", entry->base);
            print("bitmap size: %llu KiB", bitmap_size / 1024);
            entry->base += DIV_UP(bitmap_size, PAGE_SIZE) * PAGE_SIZE;
            entry->length -= DIV_UP(bitmap_size, PAGE_SIZE) * PAGE_SIZE;

            memset(bitmap, 0x00, bitmap_size);

            if ((free_pages % 8) != 0) {
                bitmap[bitmap_size - 1] |= (1 << (free_pages % 8)) - 1;
            }

            break;
        }
    }

    highest_page = highest_addr / PAGE_SIZE;

    for (size_t x = 0; x < memmap->entry_count; x++) {
        #define entry (memmap->entries[x])

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            print("free mem @ 0x%p, size: %lluKiB", entry->base, (entry->length / PAGE_SIZE) * PAGE_SIZE / 1024);
        }
    }

    lock_release(&lock);
}

void* pmm_alloc_nz(size_t pages) {
    size_t starting_index = last_indexed;
    void* mem = NULL;

    size_t continuous_pages = 0;

    while (last_indexed < highest_page) {
        last_indexed++;
        if (!test(last_indexed)) {if (++continuous_pages == pages) {
            for (size_t x = last_indexed - pages; x < last_indexed; x++) {set(x);}
            mem = (void*)((last_indexed - pages) * PAGE_SIZE);
        } else {continuous_pages = 0;}
        }
    }

    if (mem == NULL) {
        last_indexed = 0;
        while (last_indexed < starting_index) {
            last_indexed++;
            if (!test(last_indexed)) {if (++continuous_pages == pages) {
                for (size_t x = last_indexed - pages; x < last_indexed; x++) {set(x);}
                mem = (void*)((last_indexed - pages) * PAGE_SIZE);
            } else {continuous_pages = 0;}
            }
        }
    }

    if (mem == NULL) {PANIC("PAGE ALLOC FAILED, NO MORE MEMORY");}
    return mem;
}

void* pmm_alloc(size_t pages) {
    lock_acquire(&lock);
    void* mem = pmm_alloc_nz(pages);
    lock_release(&lock);

    memset(mem + hhdm->offset, 0, pages * PAGE_SIZE);
    return mem;
}

void pmm_free(void* addr, size_t pages) {
    lock_acquire(&lock);

    for (size_t x = (uint64_t)addr / PAGE_SIZE; x < ((uint64_t)addr / PAGE_SIZE) + pages; x++) {
        bitmap[x / 8] &= ~(1 << (x % 8));
    }

    lock_release(&lock);
}