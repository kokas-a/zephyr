#include <zephyr/arch/cpu.h>
#include <zephyr/init.h>

#define CLKSEL_TIMER1MS_CLK (0x28)

#define CLKSEL_TIMER1MS_CLK_SEL_MASK (0x7 << 0)
#define CLKSEL_TIMER1MS_CLK_SEL_SEL1 (0x0 << 0)
/* Select CLK_M_OSC clock */
#define CLKSEL_TIMER1MS_CLK_SEL_SEL2 (0x1 << 0)

#define CM_WKUP_TIMER1_CLKCTRL  (0xC4)

#define CM_MODULEMODE_MASK        (0x3 << 0)
#define CM_MODULEMODE_ENABLE      (0x2 << 0)
#define CM_MODULEMODE_DISABLED    (0x0 << 0)
#define CM_CLKCTRL_IDLEST         (0x3 << 16)
#define CM_CLKCTRL_IDLEST_FUNC    (0x0 << 16)
#define CM_CLKCTRL_IDLEST_TRANS   (0x1 << 16)
#define CM_CLKCTRL_IDLEST_IDLE    (0x2 << 16)
#define CM_CLKCTRL_IDLEST_DISABLE (0x3 << 16)

#define am335x_cm_read32(a)            (*(volatile uint32_t *)(a))
#define am335x_cm_write32(a, v)        (*(volatile uint32_t *)(a) = (v))

uint32_t am335x_cm_bringup(void)
{
    uint32_t val;

    /* disable the module and wait for the module to be disabled */
    val = am335x_cm_read32(0x44E00400 + CM_WKUP_TIMER1_CLKCTRL);
    val &= ~CM_MODULEMODE_MASK;
    val |= (CM_MODULEMODE_DISABLED & CM_MODULEMODE_MASK);
    am335x_cm_write32(0x44E00400 +  CM_WKUP_TIMER1_CLKCTRL, val);

    while ((am335x_cm_read32(0x44E00400 + CM_WKUP_TIMER1_CLKCTRL) & CM_CLKCTRL_IDLEST)
           != CM_CLKCTRL_IDLEST_DISABLE) {
        ;
    }

    val = am335x_cm_read32(0x44E00500 +  CLKSEL_TIMER1MS_CLK);
    val &= ~CLKSEL_TIMER1MS_CLK_SEL_MASK;
    val |= (CLKSEL_TIMER1MS_CLK_SEL_SEL2 & CLKSEL_TIMER1MS_CLK_SEL_MASK);
    am335x_cm_write32(0x44E00500 + CLKSEL_TIMER1MS_CLK, val);

    while ((am335x_cm_read32(0x44E00500 + CLKSEL_TIMER1MS_CLK) &
            CLKSEL_TIMER1MS_CLK_SEL_MASK) !=
           CLKSEL_TIMER1MS_CLK_SEL_SEL2) {
        ;
    }

    /* enable the module and wait for the module to be ready */
    val = am335x_cm_read32(0x44E00400 + CM_WKUP_TIMER1_CLKCTRL);
    val &= ~CM_MODULEMODE_MASK;
    val |= (CM_MODULEMODE_ENABLE & CM_MODULEMODE_MASK);
    am335x_cm_write32(0x44E00400 + CM_WKUP_TIMER1_CLKCTRL, val);

    while ((am335x_cm_read32(0x44E00400 + CM_WKUP_TIMER1_CLKCTRL) & CM_CLKCTRL_IDLEST)
           != CM_CLKCTRL_IDLEST_FUNC) {
        ;
    }

    return 0;
}

