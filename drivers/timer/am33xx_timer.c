#define DT_DRV_COMPAT ti_am335x_timer_1ms
#include <limits.h>

#include <zephyr/device.h>
#include <zephyr/drivers/timer/system_timer.h>
#include <zephyr/irq.h>
#include <zephyr/sys_clock.h>
#include <zephyr/spinlock.h>
#include <soc.h>

#define AM335X_TIMER_TIDR      0x000    /* IP revision code */
#define AM335X_TIMER_TIOCP_CFG 0x010    /* Controls params for GP timer L4 iface */
#define AM335X_TIMER_TISTAT    0x014    /* Status (excl. interrupt status) */
#define AM335X_TIMER_TISR      0x018    /* Pending interrupt status */
#define AM335X_TIMER_TIER      0x01C    /* Interrupt enable */
#define AM335X_TIMER_TWER      0x020    /* Wakeup enable */
#define AM335X_TIMER_TCLR      0x024    /* Controls optional features */
#define AM335X_TIMER_TCRR      0x028    /* Internal counter value */
#define AM335X_TIMER_TLDR      0x02C    /* Timer load value */
#define AM335X_TIMER_TTGR      0x030    /* Triggers counter reload */
#define AM335X_TIMER_TWPS      0x034    /* Indicates if Write-Posted pending */
#define AM335X_TIMER_TMAR      0x038    /* Value to be compared with counter */
#define AM335X_TIMER_TCAR1     0x03C    /* First captured value of counter reg */
#define AM335X_TIMER_TSICR     0x040    /* Control posted mode and functional SW rst */
#define AM335X_TIMER_TCAR2     0x044    /* Second captured value of counter register */
#define AM335X_TIMER_TPIR      0x048    /* Positive increment (1 ms tick) */
#define AM335X_TIMER_TNIR      0x04C    /* Negative increment (1 ms tick) */
#define AM335X_TIMER_TCVR      0x050    /* Defines TCRR is sub/over-period (1 ms tick) */
#define AM335X_TIMER_TOCR      0x054    /* Masks tick interrupt */
#define AM335X_TIMER_TOWR      0x058    /* Number of masked overflow interrupts */

/* Interrupt status register fields */
#define AM335X_TISR_MAT_IT_FLAG  (1 << 0)    /* Pending match interrupt status */
#define AM335X_TISR_OVF_IT_FLAG  (1 << 1)    /* Pending overflow interrupt status */
#define AM335X_TISR_TCAR_IT_FLAG (1 << 2)    /* Pending capture interrupt status */

/* Interrupt enable register fields */
#define AM335X_TIER_MAT_IT_ENA  (1 << 0) /* Enable match interrupt */
#define AM335X_TIER_OVF_IT_ENA  (1 << 1) /* Enable overflow interrupt */
#define AM335X_TIER_TCAR_IT_ENA (1 << 2) /* Enable capture interrupt */

/* Timer control fields */
#define AM335X_TCLR_ST       (1 << 0)    /* Start/stop timer */
#define AM335X_TCLR_AR       (1 << 1)    /* Autoreload or one-shot mode */
#define AM335X_TCLR_PRE      (1 << 5)    /* Prescaler on */
#define AM335X_TCLR_PTV      (1 << 1)    /* looks like "bleed" from Minix */
#define AM335X_TCLR_OVF_TRG  (1 << 10)   /* Overflow trigger */

#define am335x_timer_read32(a)            (*(volatile uint32_t *)(a))
#define am335x_timer_write32(a, v)        (*(volatile uint32_t *)(a) = (v))

static const uint32_t am33xx_timer_base = DT_INST_REG_ADDR(0);

static void timer_isr(const void *arg)
{
	ARG_UNUSED(arg);

	uint32_t reg_val;

	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TIER, 0);

	/* Clear Pending Interrupt by writing '1' to it */
	reg_val = am335x_timer_read32(am33xx_timer_base + AM335X_TIMER_TISR);
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TISR, reg_val);

	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TIER, AM335X_TIER_OVF_IT_ENA);

	sys_clock_announce(1);
}

uint32_t sys_clock_elapsed(void)
{
	return 0;
}

uint32_t sys_clock_cycle_get_32(void)
{
	return (uint32_t)am335x_timer_read32(am33xx_timer_base + AM335X_TIMER_TCRR);
}

/******************************************************************************
 *
 */
__attribute__((unused))
static uint32_t am335x_timer_check_id(void)
{
	uint32_t id;

	id = am335x_timer_read32(am33xx_timer_base + AM335X_TIMER_TIDR);
	id &= 0xFF;

	return (id ? 0 : -ENODEV);
}

/******************************************************************************
 *
 */
static int sys_clock_driver_init(void)
{
	uint32_t reg_val;
	const int irq_num = DT_INST_IRQN(0);
	const int irq_prio = DT_INST_IRQ(0, priority);

	am335x_timer_check_id();

	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TPIR, 232000);
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TNIR, -768000);

	reg_val = 0xffffffff - (sys_clock_hw_cycles_per_sec() / CONFIG_SYS_CLOCK_TICKS_PER_SEC) + 1;
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TLDR, reg_val);
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TCRR, reg_val);

	IRQ_CONNECT(irq_num , irq_prio, timer_isr, NULL, 0);

	/* Set up overflow interrupt */
	reg_val = AM335X_TISR_MAT_IT_FLAG | AM335X_TISR_OVF_IT_FLAG | AM335X_TISR_TCAR_IT_FLAG;
	/* Clear interrupt status */
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TISR, reg_val);
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TIER, AM335X_TIER_OVF_IT_ENA);

	irq_enable(DT_INST_IRQ(0, irq));

	/* Enable with reload period */
	reg_val = (AM335X_TCLR_OVF_TRG | AM335X_TCLR_AR | AM335X_TCLR_ST);
	am335x_timer_write32(am33xx_timer_base + AM335X_TIMER_TCLR, reg_val);

	return 0;
}

SYS_INIT(sys_clock_driver_init, PRE_KERNEL_2,
	 CONFIG_SYSTEM_CLOCK_INIT_PRIORITY);

