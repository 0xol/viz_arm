#ifndef _VIZ_LIB_LOCK_H
#define _VIZ_LIB_LOCK_H

#include <stdbool.h>

struct lock_t {
    int lock;
};

inline static bool lock_test_and_acq(struct lock_t *lock) {
    return __atomic_test_and_set(&lock->lock, 1);
}

inline static void lock_acquire(struct lock_t *lock) {
    for (;;) {
        if (lock_test_and_acq(lock)) {break;}
    }
}

inline static void lock_release(struct lock_t *lock) {
    __atomic_store_n(&lock->lock, 0, __ATOMIC_SEQ_CST);
}

inline static void lock_init(struct lock_t *lock) {
    lock->lock = 0;
}

#endif
