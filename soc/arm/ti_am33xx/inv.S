	.text
	.global ti_am335x_inv_all_test

ti_am335x_inv_all_test:
// SECTION_SUBSEC_FUNC(TEXT, _reset_section, ti_am335x_inv_all_test)

	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			        @ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
	mcr	p15, 0, r0, c7, c5, 6	@ invalidate BP array
	mcr p15, 0, r0, c7, c10, 4	@ DSB
	mcr p15, 0, r0, c7, c5, 4	@ ISB

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	    @ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	    @ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	    @ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	    @ set bit 11 (Z---) BTB
	bic	r0, r0, #0x00001000	    @ clear bit 12 (I) I-cache
	mcr	p15, 0, r0, c1, c0, 0

	bx		lr
	.size	ti_am335x_inv_all_test, . - ti_am335x_inv_all_test
