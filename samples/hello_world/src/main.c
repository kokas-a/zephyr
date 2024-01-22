/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <stdio.h>

static struct k_thread my_test_thread;

K_THREAD_STACK_DEFINE(my_test_thread_stack, 512);

static void tentry(void *a, void *b, void *c)
{
	printf("Hello World from %s (%s)\n",
		       k_is_user_context() ? "UserSpace!" : "privileged mode.",
		       CONFIG_BOARD);
}



int main(void)
{
	//register uint32_t r28 asm ("sp");

	k_tid_t current_thread = k_current_get();
	uint8_t *c;
	uint32_t counter = 0;
	uint32_t sp_save;

	void* stack_hi_addr = (void *)(current_thread->stack_info.start + current_thread->stack_info.size);
	void* stack_low_addr=  (void *)current_thread->stack_info.start;

	printk("start (hi_addr) %p, end(low_addr) %p\r\n", stack_hi_addr, stack_low_addr);

	 __asm__ __volatile__ ("mov %0, %%sp\n\t" :
			 "=r" (sp_save):: "memory");

	for(c = (uint8_t *)sp_save; c != stack_low_addr; c--){
		*c = 0xFE;
	}


	k_thread_create(
			&my_test_thread,
			my_test_thread_stack,
			K_THREAD_STACK_SIZEOF(my_test_thread_stack),
			tentry,	/* entry*/
			NULL, 		/* p1 */
			NULL, 		/* p2 */
			NULL,		/* p3 */
			5 			/* prio */,
			K_INHERIT_PERMS, /* options */
			 K_FOREVER
			//K_NO_WAIT	/* delay */
			);


	k_thread_start(&my_test_thread);

	k_thread_join(&my_test_thread, K_FOREVER);

	for(c = (uint8_t *)sp_save; c != stack_low_addr; c--){
		if(*c != 0xFE){
			counter++;
		}
	}

	printk("counter = %d\r\n", counter);

	return 0;
}
