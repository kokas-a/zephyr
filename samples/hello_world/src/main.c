/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/sys/time_units.h>
#include <zephyr/types.h>

#define param 33 / 33 + 1 -1

int main(void)
{
	printk("Hello World! %s\n", CONFIG_BOARD);
#if 0
	uint64_t tmp_val;

	printk("k_ms_to_cyc_floor32(%d) = %u\r\n", param,   k_ms_to_cyc_floor32(param));

	tmp_val =  k_ms_to_cyc_floor64(param);
	printk("k_ms_to_cyc_floor64(%d) = %llu\r\n", param, tmp_val);
	printk("k_ms_to_cyc_near32(%d)  = %u\r\n", param,   k_ms_to_cyc_near32(param));

	tmp_val =  k_ms_to_cyc_near64(param);
	printk("k_ms_to_cyc_near64(%d)  = %llu\r\n", param, tmp_val);
	printk("k_ms_to_cyc_ceil32(%d)  = %u\r\n", param,   k_ms_to_cyc_floor32(param));

	tmp_val =  k_ms_to_cyc_ceil64(param);
	printk("k_ms_to_cyc_ceil64(%d)  = %llu\r\n", param, tmp_val);

	printf("k_ms_to_ticks_floor32(%d) = %u\r\n", param,   k_ms_to_ticks_floor32(param));
	printf("k_ms_to_ticks_floor64(%d) = %llu\r\n", param, k_ms_to_ticks_floor64(param));
	printf("k_ms_to_ticks_near32(%d)  = %u\r\n", param,   k_ms_to_ticks_near32(param));
	printf("k_ms_to_ticks_near64(%d)  = %llu\r\n", param, k_ms_to_ticks_near64(param));
	printf("k_ms_to_ticks_ceil32(%d)  = %u\r\n", param,   k_ms_to_ticks_floor32(param));
	printf("k_ms_to_ticks_ceil64(%d)  = %llu\r\n", param, k_ms_to_ticks_ceil64(param));

	printf("k_us_to_cyc_floor32(%d) = %u\r\n", param,   k_us_to_cyc_floor32(param));
	printf("k_us_to_cyc_floor64(%d) = %llu\r\n", param, k_us_to_cyc_floor64(param));
	printf("k_us_to_cyc_near32(%d)  = %u\r\n", param,   k_us_to_cyc_near32(param));
	printf("k_us_to_cyc_near64(%d)  = %llu\r\n", param, k_us_to_cyc_near64(param));
	printf("k_us_to_cyc_ceil32(%d)  = %u\r\n", param,   k_us_to_cyc_floor32(param));
	printf("k_us_to_cyc_ceil64(%d)  = %llu\r\n", param, k_us_to_cyc_ceil64(param));

	printf("k_us_to_ticks_floor32(%d) = %u\r\n", param,   k_us_to_ticks_floor32(param));
	printf("k_us_to_ticks_floor64(%d) = %llu\r\n", param, k_us_to_ticks_floor64(param));
	printf("k_us_to_ticks_near32(%d)  = %u\r\n", param,   k_us_to_ticks_near32(param));
	printf("k_us_to_ticks_near64(%d)  = %llu\r\n", param, k_us_to_ticks_near64(param));
	printf("k_us_to_ticks_ceil32(%d)  = %u\r\n", param,   k_us_to_ticks_floor32(param));
	printf("k_us_to_ticks_ceil64(%d)  = %llu\r\n", param, k_us_to_ticks_ceil64(param));

	printf("k_ns_to_cyc_floor32(%d) = %u\r\n", param,   k_ns_to_cyc_floor32(param));
	printf("k_ns_to_cyc_floor64(%d) = %llu\r\n", param, k_ns_to_cyc_floor64(param));
	printf("k_ns_to_cyc_near32(%d)  = %u\r\n", param,   k_ns_to_cyc_near32(param));
	printf("k_ns_to_cyc_near64(%d)  = %llu\r\n", param, k_ns_to_cyc_near64(param));
	printf("k_ns_to_cyc_ceil32(%d)  = %u\r\n", param,   k_ns_to_cyc_floor32(param));
	printf("k_ns_to_cyc_ceil64(%d)  = %llu\r\n", param, k_ns_to_cyc_ceil64(param));

	printf("k_ns_to_ticks_floor32(%d) = %u\r\n", param,   k_ns_to_ticks_floor32(param));
	printf("k_ns_to_ticks_floor64(%d) = %llu\r\n", param, k_ns_to_ticks_floor64(param));
	printf("k_ns_to_ticks_near32(%d)  = %u\r\n", param,   k_ns_to_ticks_near32(param));
	printf("k_ns_to_ticks_near64(%d)  = %llu\r\n", param, k_ns_to_ticks_near64(param));
	printf("k_ns_to_ticks_ceil32(%d)  = %u\r\n", param,   k_ns_to_ticks_floor32(param));
	printf("k_ns_to_ticks_ceil64(%d)  = %llu\r\n", param, k_ns_to_ticks_ceil64(param));

	printf("k_cyc_to_ms_floor32(%d) = %u\r\n", param,   k_cyc_to_ms_floor32(param));
	printf("k_cyc_to_ms_floor64(%d) = %llu\r\n", param, k_cyc_to_ms_floor64(param));
	printf("k_cyc_to_ms_near32(%d)  = %u\r\n", param,   k_cyc_to_ms_near32(param));
	printf("k_cyc_to_ms_near64(%d)  = %llu\r\n", param, k_cyc_to_ms_near64(param));
	printf("k_cyc_to_ms_ceil32(%d)  = %u\r\n", param,   k_cyc_to_ms_ceil32(param));
	printf("k_cyc_to_ms_ceil64(%d)  = %llu\r\n", param, k_cyc_to_ms_ceil64(param));

	printf("k_cyc_to_us_floor32(%d) = %u\r\n", param,   k_cyc_to_us_floor32(param));
	printf("k_cyc_to_us_floor64(%d) = %llu\r\n", param, k_cyc_to_us_floor64(param));
	printf("k_cyc_to_us_near32(%d)  = %u\r\n", param,   k_cyc_to_us_near32(param));
	printf("k_cyc_to_us_near64(%d)  = %llu\r\n", param, k_cyc_to_us_near64(param));
	printf("k_cyc_to_us_ceil32(%d)  = %u\r\n", param,   k_cyc_to_us_ceil32(param));
	printf("k_cyc_to_us_ceil64(%d)  = %llu\r\n", param, k_cyc_to_us_ceil64(param));

	printf("k_cyc_to_ns_floor32(%d) = %u\r\n", param,   k_cyc_to_ns_floor32(param));
	printf("k_cyc_to_ns_floor64(%d) = %llu\r\n", param, k_cyc_to_ns_floor64(param));
	printf("k_cyc_to_ns_near32(%d)  = %u\r\n", param,   k_cyc_to_ns_near32(param));
	printf("k_cyc_to_ns_near64(%d)  = %llu\r\n", param, k_cyc_to_ns_near64(param));
	printf("k_cyc_to_ns_ceil32(%d)  = %u\r\n", param,   k_cyc_to_ns_ceil32(param));
	printf("k_cyc_to_ns_ceil64(%d)  = %llu\r\n", param, k_cyc_to_ns_ceil64(param));

	printf("k_cyc_to_ticks_floor32(%d) = %u\r\n", param,   k_cyc_to_ticks_floor32(param));
	printf("k_cyc_to_ticks_floor64(%d) = %llu\r\n", param, k_cyc_to_ticks_floor64(param));
	printf("k_cyc_to_ticks_near32(%d)  = %u\r\n", param,   k_cyc_to_ticks_near32(param));
	printf("k_cyc_to_ticks_near64(%d)  = %llu\r\n", param, k_cyc_to_ticks_near64(param));
	printf("k_cyc_to_ticks_ceil32(%d)  = %u\r\n", param,   k_cyc_to_ticks_ceil32(param));
	printf("k_cyc_to_ticks_ceil64(%d)  = %llu\r\n", param, k_cyc_to_ticks_ceil64(param));

	printf("k_ticks_to_ms_floor32(%d) = %u\r\n", param,   k_ticks_to_ms_floor32(param));
	printf("k_ticks_to_ms_floor64(%d) = %llu\r\n", param, k_ticks_to_ms_floor64(param));
	printf("k_ticks_to_ms_near32(%d)  = %u\r\n", param,   k_ticks_to_ms_near32(param));
	printf("k_ticks_to_ms_near64(%d)  = %llu\r\n", param, k_ticks_to_ms_near64(param));
	printf("k_ticks_to_ms_ceil32(%d)  = %u\r\n", param,   k_ticks_to_ms_ceil32(param));
	printf("k_ticks_to_ms_ceil64(%d)  = %llu\r\n", param, k_ticks_to_ms_ceil64(param));

	printf("k_ticks_to_us_floor32(%d) = %u\r\n", param,   k_ticks_to_us_floor32(param));
	printf("k_ticks_to_us_floor64(%d) = %llu\r\n", param, k_ticks_to_us_floor64(param));
	printf("k_ticks_to_us_near32(%d)  = %u\r\n", param,   k_ticks_to_us_near32(param));
	printf("k_ticks_to_us_near64(%d)  = %llu\r\n", param, k_ticks_to_us_near64(param));
	printf("k_ticks_to_us_ceil32(%d)  = %u\r\n", param,   k_ticks_to_us_ceil32(param));
	printf("k_ticks_to_us_ceil64(%d)  = %llu\r\n", param, k_ticks_to_us_ceil64(param));

	printf("k_ticks_to_ns_floor32(%d) = %u\r\n", param,   k_ticks_to_ns_floor32(param));
	printf("k_ticks_to_ns_floor64(%d) = %llu\r\n", param, k_ticks_to_ns_floor64(param));
	printf("k_ticks_to_ns_near32(%d)  = %u\r\n", param,   k_ticks_to_ns_near32(param));
	printf("k_ticks_to_ns_near64(%d)  = %llu\r\n", param, k_ticks_to_ns_near64(param));
	printf("k_ticks_to_ns_ceil32(%d)  = %u\r\n", param,   k_ticks_to_ns_ceil32(param));
	printf("k_ticks_to_ns_ceil64(%d)  = %llu\r\n", param, k_ticks_to_ns_ceil64(param));

	printf("k_ticks_to_cyc_floor32(%d) = %u\r\n", param,   k_ticks_to_cyc_floor32(param));
	printf("k_ticks_to_cyc_floor64(%d) = %llu\r\n", param, k_ticks_to_cyc_floor64(param));
	printf("k_ticks_to_cyc_near32(%d)  = %u\r\n", param,   k_ticks_to_cyc_near32(param));
	printf("k_ticks_to_cyc_near64(%d)  = %llu\r\n", param, k_ticks_to_cyc_near64(param));
	printf("k_ticks_to_cyc_ceil32(%d)  = %u\r\n", param,   k_ticks_to_cyc_ceil32(param));
	printf("k_ticks_to_cyc_ceil64(%d)  = %llu\r\n", param, k_ticks_to_cyc_ceil64(param));
#endif
	return 0;
}
