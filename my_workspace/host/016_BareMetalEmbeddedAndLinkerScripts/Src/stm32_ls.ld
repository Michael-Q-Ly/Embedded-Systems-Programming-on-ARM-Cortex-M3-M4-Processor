ENTRY(Reset_Handler)

MEMORY {
    FLASH   (rx)    : ORIGIN=0x08000000 , LENGTH=2048K
    SRAM    (rwx)   : ORIGIN=0x20000000 , LENGTH=192K
}

/* Linker Script Symbols; symbols are a name given for an address */
__max_heap_size		= 0x400 ;	/* A symbol declaration. Not a variable!!! */
__max_stack_size	= 0X200 ;	/* A symbol declaration. Not a variable!!! */

SECTIONS {
    .text : {				/* At the start, the location counter is at vma (.=vma => .= 0x08000000) */
	*(.isr_vector)
	*(.text)
	*(.rodata)
	end_of_text	= .;		/* Store the updated location counter value into a symbol 'end_of_test' */
    } > FLASH				/* Equivalently: >FLASH AT> FLASH

    .data : {				/* .data section is relocatable and must be stored in flash. The startup code has to copy this section from FLASH  to SRAM */
	start_of_data	= 0x20000000;	/* Assign a value to a symbol 'stat_of_data' */
	*(.data)
    } > SRAM AT > FLASH			/* .data's absolute location falls in SRAM, but it is stored in FLASH, its load address */

    .bss : {				/* Uninitialized data */
	*(.bss)
    } > SRAM
}
