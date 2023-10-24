#include <lib/printf.h>
#include <lib/panic.h>
#include <lib/check.h>

#include <mm/pmm.h>
#include <mm/vmm.h>

#include <config/config.h>

#if (ACPI == 1)
#include <drivers/acpi/acpi.h>
#endif

void init(void) {
    printf("VIZ kernel v0.0.0, Built: "__DATE__" "__TIME__"\n");
    pmm_init();
    vmm_init();
    #if (ACPI == 1)
    //CHECK(acpi_init());
    #endif
    PANIC("init() finished");
}