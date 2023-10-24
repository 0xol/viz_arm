#ifndef LIB_ASSERT_H
#define LIB_ASSERT_H

#include <lib/panic.h>

//if true, panic
#define CHECK(cond) if (cond) { PANIC(#cond); }

#endif