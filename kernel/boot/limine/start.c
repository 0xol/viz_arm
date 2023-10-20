#include <stdint.h>
#include "flanterm/flanterm.h"
#include "flanterm/backends/fb.h"
#include "limine.h"

extern void init(void);
struct flanterm_context *ft_ctx;
static volatile struct limine_framebuffer_request limine_fb = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void limine_putchar(char c) {
    char str[2];
    str[0] = c;
    str[1] = '\0';
    flanterm_write(ft_ctx, (const char*)&str, 2);
}

void _start(void) {
    if (limine_fb.response->framebuffer_count == 0) {for(;;);}
    ft_ctx = flanterm_fb_simple_init(limine_fb.response->framebuffers[0]->address, limine_fb.response->framebuffers[0]->width, limine_fb.response->framebuffers[0]->height, limine_fb.response->framebuffers[0]->pitch);
    init();
}
