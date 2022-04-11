ENTRY(Reset_Handler)

MEMORY {
    FLASH   (rx)    : ORIGIN = 0x08000000 , LENGTH = 2048K
    SRAM    (rwx)   : ORIGIN = 0x20000000 , LENGTH = 192K
}

/* Linker Script Symbols; symbols are a name given for an address */

SECTIONS {
    .text : {				/* At the start, the location counter is at vma (.=vma => .= 0x08000000) */
	*(.isr_vector)
	*(.text)
	*(.text.*)			/* Merge .text and further */
	*(.init)
	*(.fini)
	*(.rodata)
	*(.rodata.*)
	. = ALIGN(4) ;
	_etext = . ;
    } > FLASH				/* Equivalently: >FLASH AT> FLASH */

    _la_data = LOADADDR(.data) ;	/* Assign the load address */

    .data : {				/* .data section is relocatable and must be stored in flash. The startup code has to copy this section from FLASH  to SRAM */
	_sdata = . ;
	*(.data)
	*(.data.*)
	. = ALIGN(4) ;
	_edata = . ;
    } > SRAM AT > FLASH			/* .data's absolute location falls in SRAM, but it is stored in FLASH, its load address */

    .bss : {				/* Uninitialized data */
	_sbss = . ;
    __bss_start__ = _sbss ;
	*(.bss)
	*(.bss.*)
	*(COMMON)
	. = ALIGN(4) ;
	_ebss = . ;
    __bss_end__ = _ebss ;
    . = ALIGN(4) ;
    end = . ;				/* End of heap; heap is located right after bss section */
    } > SRAM
}
