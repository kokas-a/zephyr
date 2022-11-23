#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

#ifdef __llvm__
	#define ARC_GET_AUX_REG(reg)      _lr(reg)
	#define ARC_SET_AUX_REG(reg, val) _sr((unsigned int)val, reg)
#else
	#define ARC_GET_AUX_REG(reg)      __builtin_arc_lr(reg)
	#define ARC_SET_AUX_REG(reg, val) __builtin_arc_sr((unsigned int)val, reg)
#endif

#define ARC_AUX_RF_BUILD				(0x6e)
#define     AUX_RF_BUILD_NUM_RF_MASK	(0x7)
#define     AUX_RF_BUILD_NUM_RF_OFFS	(11)
#define ARC_AUX_TIMER1_COUNT			(0x100)
#define ARC_AUX_TIMER1_CONTROL			(0x101)
#define     AUX_TIMER_CONTROL_IE_BIT		(1 << 0)	/* Int Enable */
#define     AUX_TIMER_CONTROL_NH_BIT		(1 << 1)	/* Not Halted */
#define     AUX_TIMER_CONTROL_W_BIT			(1 << 2)	/* WatchDog Mode */
#define     AUX_TIMER_CONTROL_IP_BIT		(1 << 3)	/*  */
#define     AUX_TIMER_CONTROL_TD_BIT		(1 << 4)	/*  */
#define ARC_AUX_TIMER1_LIMIT			(0x102)
#define ARC_AUX_IRQ_HINT				(0x201)
#define ARC_AUX_IRQ_BUILD				(0xf3)
#define     AUX_IRQ_BUILD_PRIO_OFFS     (24)
#define     AUX_IRQ_BUILD_PRIO_MASK     (0xF)


#define TEST_IRQ_NUM					(20)

/*****************************************************************************/
#define NUM_OF_MEASEREMENTS				(20)
#define IRQ_PRIO						(1)

/* size of stack area used by each thread */
#define STACKSIZE                       (1024)
#define HP_THREAD_PRIO					(2)
#define LP_THREAD_PRIO					(5)

static volatile uint32_t prolog[NUM_OF_MEASEREMENTS];
static volatile uint32_t total[NUM_OF_MEASEREMENTS];
static volatile uint32_t iteration;



/******************************************************************************
 *
 */
static void test_irq_routine(const void *args)
{
	(void)args;

	prolog[iteration] = ARC_GET_AUX_REG(ARC_AUX_TIMER1_COUNT);
}

/******************************************************************************
 *
 */
static int check_num_of_rf(void)
{
	int reg_val = ARC_GET_AUX_REG(ARC_AUX_RF_BUILD);
	int res = 0;

	reg_val >>= AUX_RF_BUILD_NUM_RF_OFFS;
	reg_val &=  AUX_RF_BUILD_NUM_RF_MASK;

	/*
	 * ARC_AUX_RF_BUILD shows numbers of additional RF.
	 * Take into account Main RF (+1)
	 */
	reg_val += 1;

	printk("Built-in num of Reg Files = %d\r\n", reg_val);

	if (CONFIG_RGF_NUM_BANKS != reg_val) {
		printk("Wrong Reg File config: HW(%d), SW(%d)\r\n",
				reg_val, CONFIG_RGF_NUM_BANKS);
		res = (-1);
	}

	//return res;
	return 0;
}

/******************************************************************************
 *
 */
static int check_num_of_irq_levels(int test_prio)
{
	int reg_val = ARC_GET_AUX_REG(ARC_AUX_IRQ_BUILD);
	int res = 0;

	reg_val >>= AUX_IRQ_BUILD_PRIO_OFFS;
	reg_val &=  AUX_IRQ_BUILD_PRIO_MASK;

	printk("Built-in num of IRQ Levels = %d\r\n", reg_val + 1);

	if(test_prio > reg_val){
		res =  (-1);
	}

	return res;
}

K_THREAD_STACK_DEFINE(thread_a_stack_area, STACKSIZE);
K_THREAD_STACK_DEFINE(thread_b_stack_area, STACKSIZE);

struct k_thread a_thread_hdlr;
struct k_thread b_thread_hdlr;

K_SEM_DEFINE(done_sem, 0, 1);

/******************************************************************************
 *
 */
static void thread_a(void *nu1, void *nu2, void *nu3)
{
	ARG_UNUSED(nu1);
	ARG_UNUSED(nu2);
	ARG_UNUSED(nu3);

	k_thread_start(&b_thread_hdlr);

	while(1){

		k_sched_lock();

		/* Invert priorities for threads */
		k_thread_priority_set(&a_thread_hdlr, LP_THREAD_PRIO);
		k_thread_priority_set(&b_thread_hdlr, HP_THREAD_PRIO);

		/*
		 * Do not preform sched unlock here:
		 * --Priorities have already changed and
		 *   irq will cause threads toggling.
		 * --k_sched_unlock() looks to be a cancellation point and cause
		 *   thread switching itself. This is not preferred in this case.
		 */

		//k_sched_unlock();

		//printk("A\r\n");

		/* reset counter */
		ARC_SET_AUX_REG(ARC_AUX_TIMER1_COUNT, 0);

		/* enable timer 1 in non-halt mode */
		ARC_SET_AUX_REG(ARC_AUX_TIMER1_CONTROL, AUX_TIMER_CONTROL_NH_BIT);

		ARC_SET_AUX_REG(ARC_AUX_IRQ_HINT, TEST_IRQ_NUM);

		if(NUM_OF_MEASEREMENTS == iteration){
			k_sleep(K_FOREVER);
		}
	}
}

/******************************************************************************
 *
 */
static void thread_b(void *nu1, void *nu2, void *nu3)
{
	ARG_UNUSED(nu1);
	ARG_UNUSED(nu2);
	ARG_UNUSED(nu3);

	while(1){
		//printk("B\r\n");

		total[iteration] = ARC_GET_AUX_REG(ARC_AUX_TIMER1_COUNT);

		iteration++;

		k_sched_lock();

		/* Invert priorities for threads */
		k_thread_priority_set(&a_thread_hdlr, HP_THREAD_PRIO);
		k_thread_priority_set(&b_thread_hdlr, LP_THREAD_PRIO);

		k_sched_unlock();

		/*
		 * Do not preform sched yield here:
		 * --k_sched_unlock() looks to be a cancellation point and cause
		 *   thread switching itself
		 */

		//k_yield();

		if(NUM_OF_MEASEREMENTS == iteration){
			/* Release semaphor here to print statistic */
			k_sem_give(&done_sem);

			k_sleep(K_FOREVER);
		}
	}
}

/******************************************************************************
 *
 */
static void test_thread_switching_latency(void)
{


	k_thread_create(
			&b_thread_hdlr,
			thread_b_stack_area,
			K_THREAD_STACK_SIZEOF(thread_b_stack_area),
			thread_b,
			NULL, NULL, NULL,
			LP_THREAD_PRIO, 0, K_FOREVER);

	k_thread_create(
			&a_thread_hdlr,
			thread_a_stack_area,
			K_THREAD_STACK_SIZEOF(thread_a_stack_area),
			thread_a,
			NULL, NULL, NULL,
			HP_THREAD_PRIO, 0, K_FOREVER);


	k_thread_start(&a_thread_hdlr);
	k_sem_take(&done_sem, K_FOREVER);
}

/******************************************************************************
 *
 */
static void test_linear_latency(void)
{
	for (iteration = 0; iteration < NUM_OF_MEASEREMENTS; iteration++) {
		/* reset counter */
		ARC_SET_AUX_REG(ARC_AUX_TIMER1_COUNT, 0);

		/* enable timer 1 in non-halt mode */
		ARC_SET_AUX_REG(ARC_AUX_TIMER1_CONTROL, AUX_TIMER_CONTROL_NH_BIT);

		ARC_SET_AUX_REG(ARC_AUX_IRQ_HINT, TEST_IRQ_NUM);

		total[iteration] = ARC_GET_AUX_REG(ARC_AUX_TIMER1_COUNT);
	}
}

/******************************************************************************
 *
 */
void main(void)
{
	int vector;

	memset((void *)prolog, 0x0, sizeof(prolog));
	memset((void *)total, 0x0, sizeof(total));

	if (check_num_of_rf()) {
		return;
	}

	if(check_num_of_irq_levels(IRQ_PRIO)) {
		return;
	}

	printk("Install IRQ %d\r\n", TEST_IRQ_NUM);

	vector = irq_connect_dynamic(TEST_IRQ_NUM, IRQ_PRIO,
			test_irq_routine, NULL, 0);

	irq_enable(TEST_IRQ_NUM);

	printk("Start measurements\r\n");
#if 0
	test_linear_latency();
#else
	test_thread_switching_latency();
#endif

	printk("End of measurements\r\n\r\n");

	printk("Num of RF(SW) = %d\r\n", CONFIG_RGF_NUM_BANKS);
	printk("IRQ_%d, Prio = %d\r\n", TEST_IRQ_NUM, IRQ_PRIO);
	printk("Prolog, Epilog, Total\r\n");

	for (iteration = 0; iteration < NUM_OF_MEASEREMENTS; iteration++) {

		printk("%6d, %6d, %6d\r\n", prolog[iteration],
				(total[iteration] - prolog[iteration]), total[iteration]);
	}

	printk("----------\r\nDone\r\n");
}
