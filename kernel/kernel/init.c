#include <lib/printf.h>
#include "dtb.h"
#include <mm/pmm.h>
#include <lib/panic.h>

void init(void) {
    printf("VIZ kernel v0.0.0, Built: "__DATE__" "__TIME__"\n");
    init_dtb();
    pmm_init();
    PANIC("init() finished");
}