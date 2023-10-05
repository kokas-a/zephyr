#define DT_DRV_COMPAT ti_am33xx_intc

#include <zephyr/kernel.h>
#include <zephyr/arch/cpu.h>
#include <zephyr/device.h>
#include <zephyr/init.h>

#define AM33XX_INTC_REVISION     0x000    /* IP revision code */
#define AM33XX_INTC_SYSCONFIG    0x010    /* Controls params */
#define AM33XX_INTC_SYSSTATUS    0x014    /* Status */
#define AM33XX_INTC_SIR_IRQ      0x040    /* Active IRQ number */
#define AM33XX_INTC_SIR_FIQ      0x044    /* Active FIQ number */
#define AM33XX_INTC_CONTROL      0x048    /* New int agreement bits */
#define AM33XX_INTC_PROTECTION   0x04C    /* Protection for other regs */
#define AM33XX_INTC_IDLE         0x050    /* Clock auto-idle/gating */
#define AM33XX_INTC_IRQ_PRIORITY 0x060    /* Active IRQ priority level */
#define AM33XX_INTC_FIQ_PRIORITY 0x064    /* Active FIQ priority level */
#define AM33XX_INTC_THRESHOLD    0x068    /* Priority threshold */

#define AM33XX_INTC_ITR0         0x080    /* Raw pre-masking interrupt status */
#define AM33XX_INTC_MIR0         0x084    /* Interrupt mask */
#define AM33XX_INTC_MIR_CLEAR0   0x088    /* Clear interrupt mask bits */
#define AM33XX_INTC_MIR_SET0     0x08C    /* Set interrupt mask bits */
#define AM33XX_INTC_ISR_SET0     0x090    /* Set software int bits */
#define AM33XX_INTC_ISR_CLEAR0   0x094    /* Clear software int bits */
#define AM33XX_INTC_PENDING_IRQ0 0x098    /* IRQ status post-masking */
#define AM33XX_INTC_PENDING_FIQ0 0x09C    /* FIQ status post-masking */

#define AM33XX_INTC_ITR1         0x0A0    /* Raw pre-masking interrupt status */
#define AM33XX_INTC_MIR1         0x0A4    /* Interrupt mask */
#define AM33XX_INTC_MIR_CLEAR1   0x0A8    /* Clear interrupt mask bits */
#define AM33XX_INTC_MIR_SET1     0x0AC    /* Set interrupt mask bits */
#define AM33XX_INTC_ISR_SET1     0x0B0    /* Set software int bits */
#define AM33XX_INTC_ISR_CLEAR1   0x0B4    /* Clear software int bits */
#define AM33XX_INTC_PENDING_IRQ1 0x0B8    /* IRQ status post-masking */
#define AM33XX_INTC_PENDING_FIQ1 0x0BC    /* FIQ status post-masking */

#define AM33XX_INTC_ITR2         0x0C0    /* Raw pre-masking interrupt status */
#define AM33XX_INTC_MIR2         0x0C4    /* Interrupt mask */
#define AM33XX_INTC_MIR_CLEAR2   0x0C8    /* Clear interrupt mask bits */
#define AM33XX_INTC_MIR_SET2     0x0CC    /* Set interrupt mask bits */
#define AM33XX_INTC_ISR_SET2     0x0D0    /* Set software int bits */
#define AM33XX_INTC_PENDING_IRQ2 0x0D8    /* IRQ status post-masking */
#define AM33XX_INTC_PENDING_FIQ2 0x0DC    /* FIQ status post-masking */

#define AM33XX_INTC_ITR3         0x0E0    /* Raw pre-masking interrupt status */
#define AM33XX_INTC_MIR3         0x0E4    /* Interrupt mask */
#define AM33XX_INTC_MIR_CLEAR3   0x0E8    /* Clear interrupt mask bits */
#define AM33XX_INTC_MIR_SET3     0x0EC    /* Set interrupt mask bits */
#define AM33XX_INTC_ISR_SET3     0x0F0    /* Set software int bits */
#define AM33XX_INTC_PENDING_IRQ3 0x0F8    /* IRQ status post-masking */
#define AM33XX_INTC_PENDING_FIQ3 0x0FC    /* FIQ status post-masking */

#define AM33XX_INTC_ILR0         0x100    /* Priority for interrupts */

/* CONTROL_REG: NewIrq_Reset IRQ output and enable new IRQ generation */
#define CONTROL_NEW_IRQ_GEN     0x1
#define CONTROL_NEW_FIQ_GEN     0x2

/* SIR_IRQ_REG: Active IRQ number */
#define SIR_IRQ_ACTIVE_MASK		0x7f
#define SIR_IRQ_SPURIOUS_MASK	0xFFFFFF80

static const uint32_t am33xx_intc_base = DT_INST_REG_ADDR(0);

#define am335x_intc_read32(a)            (*(volatile uint32_t *)(a))
#define am335x_intc_write32(a, v)        (*(volatile uint32_t *)(a) = (v))

void z_soc_irq_enable(unsigned int irq)
{
	uint32_t mir_num = irq / 32;
	uint32_t irq_offs = irq % 32;
	uint32_t mir_addr = AM33XX_INTC_MIR_CLEAR0 + 0x20 * mir_num;

	am335x_intc_write32((am33xx_intc_base + mir_addr), (1 << irq_offs));
}

void z_soc_irq_disable(unsigned int irq)
{
	uint32_t mir_num = irq / 32;
	uint32_t irq_offs = irq % 32;
	uint32_t mir_addr = AM33XX_INTC_MIR_SET0 + 0x20 * mir_num;

	am335x_intc_write32((am33xx_intc_base + mir_addr), (1 << irq_offs));
}

int z_soc_irq_is_enabled(unsigned int irq)
{
	uint32_t mir_num = irq / 32;
	uint32_t irq_offs = irq % 32;
	uint32_t mir_addr = AM33XX_INTC_MIR0 + 0x20 * mir_num;
	uint32_t reg_val;

	reg_val = am335x_intc_read32((am33xx_intc_base + mir_addr));

	return (reg_val & (1 << irq_offs)) ? 0 : 1;
}

void z_soc_irq_priority_set(
	unsigned int irq, unsigned int prio, unsigned int flags)
{
	/* FIXME: normalize OS prio to HW */

}

void z_soc_irq_sw_trigger(int irq)
{
	uint32_t isr_num = irq / 32;
	uint32_t irq_offs = irq % 32;
	uint32_t isr_addr = AM33XX_INTC_ISR_SET0 + 0x20 * isr_num;

	am335x_intc_write32((am33xx_intc_base + isr_addr), (1 << irq_offs));
}

unsigned int z_soc_irq_get_active(void)
{
	uint32_t irq;

	irq = am335x_intc_read32(am33xx_intc_base + AM33XX_INTC_SIR_IRQ);

	return (irq & SIR_IRQ_ACTIVE_MASK);
}

void z_soc_irq_eoi(unsigned int irq)
{
	am335x_intc_write32((am33xx_intc_base + AM33XX_INTC_CONTROL), CONTROL_NEW_IRQ_GEN);
}

void z_soc_irq_init(void)
{
	/* mask all interrupts */
	am335x_intc_write32((am33xx_intc_base + AM33XX_INTC_MIR_SET0), 0xFFFFFFFF);
	am335x_intc_write32((am33xx_intc_base + AM33XX_INTC_MIR_SET1), 0xFFFFFFFF);
	am335x_intc_write32((am33xx_intc_base + AM33XX_INTC_MIR_SET2), 0xFFFFFFFF);
	am335x_intc_write32((am33xx_intc_base + AM33XX_INTC_MIR_SET3), 0xFFFFFFFF);
}
