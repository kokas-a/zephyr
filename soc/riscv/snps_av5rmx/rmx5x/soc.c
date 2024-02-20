
#include <stdbool.h>
#include <zephyr/irq.h>
#include <zephyr/irq_multilevel.h>

#define __MSTATUS_IEN     (1UL << 3)

#define __MIE_MSIE        (1UL << 3)
#define __MIE_MTIE        (1UL << 7)

unsigned int z_soc_irq_lock(void)
{
#if 1
	unsigned int key;

	__asm__ volatile ("csrrc %0, mstatus, %1"
			  : "=r" (key)
			  : "rK" (__MSTATUS_IEN)
			  : "memory");

	return key;
#else
	#if 0
		unsigned int key;
		unsigned int mie_reg_val;

		mie_reg_val = csr_read(mie);
		key = mie_reg_val;
		mie_reg_val &= ~__MIE_MTIE;
		csr_write(mie, mie_reg_val);

		__asm__ volatile ( "nop"::: "memory");
	#endif

	#if 0
		unsigned int key;
		__asm__ volatile ("csrrc %0, mie, %1"
				  : "=r" (key)
				  : "rK" (__MIE_MTIE | __MIE_MSIE)
				  : "memory");

		return key;
	#endif
#endif
}


void z_soc_irq_unlock(unsigned int key)
{
#if 1
	__asm__ volatile ("csrs mstatus, %0"
			  :
			  : "r" (key & __MSTATUS_IEN)
			  : "memory");
#else
	#if 0
		unsigned int mie_reg_val;

		mie_reg_val = csr_read(mie);
		mie_reg_val |= key & __MIE_MTIE;
		csr_write(mie, mie_reg_val);

		__asm__ volatile ("nop" ::: "memory");
	#endif

	#if 0
		__asm__ volatile ("csrs mie, %0"
				  :
				  : "r" (key & (__MIE_MTIE | __MIE_MSIE))
				  : "memory");
	#endif
#endif
}


bool z_soc_irq_unlocked(unsigned int key)
{
	return (key & __MSTATUS_IEN) != 0;
}
