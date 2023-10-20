#include <lib/printf.h>
#include <stddef.h>

#ifdef limine
#include <boot/limine/limine.h>

static volatile struct limine_dtb_request dtb_request = {
    .id = LIMINE_DTB_REQUEST,
    .revision = 0
};
void init_dtb(void) {
    if (dtb_request.response != NULL) {printf("["__FILE__"] DTB found, not implemented\n");}
}

#else
#error "No dtb backend
#endif