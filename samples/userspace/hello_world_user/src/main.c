/*
 * Copyright (c) 2020 BayLibre, SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <stdio.h>
#define USER_STACKSIZE	2048

struct k_thread user_thread;
K_THREAD_STACK_DEFINE(user_stack, USER_STACKSIZE);

static void user_function(void *p1, void *p2, void *p3)
{
	printf("Hello World from %s (%s)\n",
	       k_is_user_context() ? "UserSpace!" : "privileged mode.",
	       CONFIG_BOARD);
	__ASSERT(k_is_user_context(), "User mode execution was expected");
}


int main(void)
{

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

	k_thread_create(&user_thread,
			user_stack,
			USER_STACKSIZE,
			user_function,
			NULL, NULL, NULL,
			-1, /* prio */
			K_USER, /* options */
			//K_MSEC(0)
			K_FOREVER
			);

	k_thread_start(&user_thread);
	k_thread_join(&user_thread, K_FOREVER);

	for(c = (uint8_t *)sp_save; c != stack_low_addr; c--){
		if(*c != 0xFE){
			counter++;
		}
	}


	printk("counter = %d\r\n", counter);



	return 0;
}
