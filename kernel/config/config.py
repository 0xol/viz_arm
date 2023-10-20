arch = "aarch64"
bootloader = "limine"

cc = f"clang -target {arch}-elf"
ld = f"ld.lld"
asm = f"clang -target {arch}-elf"
strip = f"{arch}-elf-strip"

cc_flags = "-O2 -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fPIE -fno-lto -fno-stack-check -Ikernel -I."
as_flags = "-O2 -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fPIE -fno-lto -fno-stack-check -Ikernel"
ld_flags = f"-nostdlib -static -pie --no-dynamic-linker -z text -z max-page-size=0x1000 -T linker/{bootloader}_{arch}.ld"

if arch == "aarch64":
    cc_flags += " -mgeneral-regs-only"
    as_flags += " -mgeneral-regs-only"

if bootloader == "limine":
    cc_flags += " -Dlimine"
    as_flags += " -Dlimine"