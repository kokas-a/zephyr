
#include <zephyr/irq.h>
#include <zephyr/irq_multilevel.h>

#define AW_D1_RISCV_CFG		0x06010000

#define WAKEUP_EN_REG		0x20
#define WAKEUP_MASK_REG		0x24

static volatile uint32_t wu_mask;
static volatile uint32_t wu_en;

#if defined(CONFIG_RISCV_SOC_INTERRUPT_INIT)
void soc_interrupt_init(void)
{
	volatile uint32_t *hw_reg;

	/* ensure that all interrupts are disabled */
	(void)arch_irq_lock();

	csr_write(mie, 0);
	csr_write(mip, 0);

#if 0
	/* unmask 0-31 wakeup irq sources */
	hw_reg = (uint32_t *)(AW_D1_RISCV_CFG + WAKEUP_MASK_REG);
	*hw_reg = 0xFFFFFFFF;

	__asm volatile (".long 0x0030000b" ::: "memory"); /* dcache.ciall */
	__asm volatile (".long 0x01a0000b" ::: "memory"); /* sync_i */;

	wu_mask = *hw_reg;

	/* Enable wakeup by irq */
	hw_reg = (uint32_t *)(AW_D1_RISCV_CFG + WAKEUP_EN_REG);
	*hw_reg = 0x1;

	__asm volatile (".long 0x0030000b" ::: "memory"); /* dcache.ciall */
	__asm volatile (".long 0x01a0000b" ::: "memory"); /* sync_i */;

	wu_en = *hw_reg;
#endif
}
#endif



