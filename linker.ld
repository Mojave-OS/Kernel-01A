
SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x80000;

    /* For AArch64, use . = 0x80000; */
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
 
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
 
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
 
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
}