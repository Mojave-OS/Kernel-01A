ENTRY(_start)
SECTIONS {
    . = 0x80000;
    .text : {
        *(.text.boot)
        *(.text.*)
    }
    .data : { *(.data) }
    
    __bss_start = .;
    .bss : { *(.bss) }
    __bss_end = .;
    __bss_size = (__bss_end - __bss_start) >> 3;

}