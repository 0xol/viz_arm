#ifndef _VIZ_LIB_PANIC_H
#define _VIZ_LIB_PANIC_H

#include <lib/printf.h>
#include <arch/halt.h>

#define PANIC(x) printf("["__FILE__"] PANIC: "x"\n ***SYSTEM HALTED***\n"); halt();

#endif