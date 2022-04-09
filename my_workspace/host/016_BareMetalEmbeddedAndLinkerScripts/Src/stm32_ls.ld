ENTRY(Reset_Handler)

MEMORY {
    FLASH   (rx)    : ORIGIN=0x08000000 , LENGTH=2048K
    SRAM    (rwx)   : ORIGIN=0x20000000 , LENGTH=192K
}

SECTIONS {
    .text : {
	*(.isr_vector)
	*(.text)
	*(.rodata)
    } > FLASH			/* Equivalently: >FLASH AT> FLASH

    .data : {			/* .data section is relocatable and must be stored in flash. The startup code has to copy this section from FLASH  to SRAM */
	*(.data)
    } > SRAM AT > FLASH		/* .data's absolute location falls in SRAM, but it is stored in FLASH, its load address */

    .bss : {			/* Uninitialized data */
	*(.bss)
    } > SRAM
}
