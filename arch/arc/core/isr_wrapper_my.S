
#include <offsets_short.h>
#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sw_isr_table.h>
#include <zephyr/kernel_structs.h>
#include <zephyr/arch/cpu.h>
#include <swap_macros.h>
#include <zephyr/arch/arc/asm-compat/assembler.h>

#if defined(CONFIG_PM)
/* r0, r1, and r3 will be used in exit_tickless_idle macro */
.macro exit_tickless_idle
	clri r0 /* do not interrupt exiting tickless idle operations */
	MOVR r1, _kernel
	breq r3, 0, _skip_pm_save_idle_exit

	st 0, [r1, _kernel_offset_to_idle] /* zero idle duration */
	PUSHR blink
	jl z_pm_save_idle_exit
	POPR blink

_skip_pm_save_idle_exit:
	seti r0
.endm
#endif /* CONFIG_PM */

/*****************************************************************************
 *
 *
 */
GTEXT(_isr_wrapper)

SECTION_FUNC(TEXT, _isr_wrapper)

#ifdef CONFIG_ARC_STACK_CHECKING
#ifdef CONFIG_ARC_SECURE_FIRMWARE
	lr MACRO_ARG(r2), [_ARC_V2_SEC_STAT]
	bclr MACRO_ARG(r2), MACRO_ARG(r2), _ARC_V2_SEC_STAT_SSC_BIT
	sflag MACRO_ARG(r2)

#else
	lr MACRO_ARG(r2), [_ARC_V2_STATUS32]
	bclr MACRO_ARG(r2), MACRO_ARG(r2), _ARC_V2_STATUS32_SC_BIT
	kflag MACRO_ARG(r2)
#endif
#endif

	clri

	/* check whether irq stack is used, if
	 * not switch to isr stack
	 */
	_check_and_inc_int_nest_counter r0, r1

	bne.d rirq_nest
	MOVR r0, sp

	_get_curr_cpu_irq_stack sp
rirq_nest:
	PUSHR r0

	seti

	/* according to ARCv2 ISA, r25, r30, r58, r59 are caller-saved
	 * scratch registers, possibly used by interrupt handlers
	 */
	PUSHR r25
	PUSHR r30
#ifdef CONFIG_ARC_HAS_ACCL_REGS
	PUSHR r58
#ifndef CONFIG_64BIT
	PUSHR r59
#endif /* !CONFIG_64BIT */
#endif

#ifdef CONFIG_SCHED_THREAD_USAGE
	bl z_sched_usage_stop
#endif

#ifdef CONFIG_TRACING_ISR
	bl sys_trace_isr_enter
#endif

#if defined(CONFIG_PM)
	/* cannot be done before this point because we must be able to run C */
	exit_tickless_idle
#endif /* CONFIG_PM */

	lr r0, [_ARC_V2_ICAUSE]
	/* handle software triggered interrupt */
	lr r3, [_ARC_V2_AUX_IRQ_HINT]
	brne r3, r0, irq_hint_handled_my
	sr 0, [_ARC_V2_AUX_IRQ_HINT]
irq_hint_handled_my:

	sub r0, r0, 16

	MOVR r1, _sw_isr_table
	/* SW ISR table entries are 8-bytes wide for 32bit ISA and
	 * 16-bytes wide for 64bit ISA */
	ASLR r0, r0, (ARC_REGSHIFT + 1)
	ADDR r0, r1, r0
	/* ISR into r1 */
	LDR r1, r0, ARC_REGSZ
	jl_s.d [r1]
	/* delay slot: ISR parameter into r0 */
	LDR r0, r0

#ifdef CONFIG_TRACING_ISR
	bl sys_trace_isr_exit
#endif

#ifdef CONFIG_ARC_HAS_ACCL_REGS
#ifndef CONFIG_64BIT
	POPR r59
#endif /* !CONFIG_64BIT */
	POPR r58
#endif

	POPR r30
	POPR r25

	clri

	POPR sp

	_dec_int_nest_counter r0, r1

	_check_nest_int_by_irq_act r0, r1

	jne _rirq_no_switch

	/* sp is struct k_thread **old of z_arc_switch_in_isr which is a wrapper of
	 * z_get_next_switch_handle. r0 contains the 1st thread in ready queue. If it isn't NULL,
	 * then do switch to this thread.
	 */
	_get_next_switch_handle

	CMPR r0, 0
	beq _rirq_no_switch

#ifdef CONFIG_ARC_SECURE_FIRMWARE
	/* here need to remember SEC_STAT.IRM bit */
	lr r3, [_ARC_V2_SEC_STAT]
	push_s r3
#endif

	/* r2 is old thread
	 * _thread_arch.relinquish_cause is 32 bit despite of platform bittnes
	 */
	_st32_huge_offset _CAUSE_RIRQ, r2, _thread_offset_to_relinquish_cause, r1

	_irq_store_old_thread_callee_regs

	/* mov new thread (r0) to r2 */
	MOVR r2, r0

/* _rirq_newthread_switch required by exception handling */
.align 4
_rirq_newthread_switch:

	_load_new_thread_callee_regs

	breq r3, _CAUSE_RIRQ, _rirq_switch_from_rirq
	nop_s
//	breq r3, _CAUSE_FIRQ, _rirq_switch_from_firq
//	nop_s

	/* fall through */

.align 4
_rirq_switch_from_coop:

	/* for a cooperative switch, it's not in irq, so
	 * need to set some regs for irq return
	 */
	_set_misc_regs_irq_switch_from_coop

	/*
	 * See verbose explanation of
	 * RETURN FROM INTERRUPT TO COOPERATIVE THREAD above
	 */

	/* carve fake stack */
	SUBR sp, sp, ___isf_t_pc_OFFSET


#ifdef CONFIG_ARC_HAS_ZOL
	/* reset zero-overhead loops */
	STR 0, sp, ___isf_t_lp_end_OFFSET
#endif /* CONFIG_ARC_HAS_ZOL */

	/*
	 * r13 is part of both the callee and caller-saved register sets because
	 * the processor is only able to save registers in pair in the regular
	 * IRQ prologue. r13 thus has to be set to its correct value in the IRQ
	 * stack frame.
	 */
	STR r13, sp, ___isf_t_r13_OFFSET

#ifdef CONFIG_INSTRUMENT_THREAD_SWITCHING
	PUSHR blink

	bl z_thread_mark_switched_in

	POPR blink
#endif
	/* stack now has the IRQ stack frame layout, pointing to sp */
	/* rtie will pop the rest from the stack */
	rtie

.align 4
_rirq_switch_from_firq:
_rirq_switch_from_rirq:

	_set_misc_regs_irq_switch_from_irq

#ifdef CONFIG_INSTRUMENT_THREAD_SWITCHING
	PUSHR blink

	bl z_thread_mark_switched_in

	POPR blink
#endif
_rirq_no_switch:
	rtie

