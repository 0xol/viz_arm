void halt(void) {
    for (;;) {
        asm ("wfi");
    }
}