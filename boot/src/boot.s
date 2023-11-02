.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here
_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b
2:  // We're on the main core!

    // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1

    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      kernel_main  // kentry will load the kernel 
    // In case it does return, halt the master core too
    b       1b

.global _end
_end:
    .word 0x0
