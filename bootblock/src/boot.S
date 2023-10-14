// the bootblock will load us over to our kernel's c-code
.section ".text.boot"
.globl _start
_start:
	b isolate_main_core

boot_main:
	// this is the main core, all others have been halted
	// we want to set up the stack
	ldr x5, =_start
	mov sp, x5

	// we now have a stack that has been set-up so we can
	// proceed with clearing the bss section
	b clear_bss_section
	
	bss_return:
	bl kernel_main
		b distract_core

// 
// Sub-Methods for our Boot Section
//
isolate_main_core:
	mrs x0, MPIDR_EL1
	and x0, x0, 3
	cmp x0, xzr
	b.ne distract_core
	b boot_main

distract_core:
	wfe
	b distract_core

clear_bss_section:
	ldr x5, =__bss_start
	ldr x6, =__bss_size

	loop_bss_clear:
		cbz w6, bss_return
		
		str wzr, [x5, x6]
		sub w6, w2, #1





