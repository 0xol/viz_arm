#ifdef limine
extern void limine_putchar(char c);    
#endif
int putchar(char c) {
    #ifdef limine
    limine_putchar(c);
    #endif
    return 0;
}