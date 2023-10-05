#include <zephyr/arch/cpu.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/init.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/sys/util.h>

#include <zephyr/arch/arm/mmu/arm_mmu.h>
#include "soc.h"

#warning TODO: MEM_REGIONS

static const struct arm_mmu_region mmu_regions[] = {


};

const struct arm_mmu_config mmu_config = {
	.num_regions = ARRAY_SIZE(mmu_regions),
	.mmu_regions = mmu_regions,
};

#if defined(CONFIG_PLATFORM_SPECIFIC_INIT)
extern void ti_am335x_inv_all_test(void);

void z_arm_platform_init(void)
{
	ti_am335x_inv_all_test();

	am335x_cm_bringup();
}
#endif
