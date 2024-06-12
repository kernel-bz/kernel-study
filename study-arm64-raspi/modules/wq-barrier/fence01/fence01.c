/*      SPDX-License-Identifier: GPL-2.0-only */
/*
 *      Based on arch/riscv/include/asm/barrier.h
 *      Jung,JaeJoon<rgbi3307@naver.com> on the www.kernel.bz 
 *
 */
#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <asm/barrier.h>

int s0;

static void wq_thread0(struct work_struct *work)
{
	pr_info("wq_thread0(): starting...\n");

        while(1) {
                int t1, t2, t3;
                int a0;

                t1 = 1;
                s0 = t1;

                t2 = 2;
                s0 = t2;

                a0 = s0;
                if (a0 != 2)
                        pr_info("a0 = %d\n", a0);

                t3 = 3;
                s0 = t3;
        }

	pr_info("wq_thread0(): end.\n");
}

static void wq_thread1(struct work_struct *work)
{
	pr_info("wq_thread1(): starting...\n");

        while(1) {
                int t4, t5;

                t4 = 4;
                s0 = t4;

                t5 = 5;
                s0 = t5;
        }

	pr_info("wq_thread1(): end.\n");
}

static DECLARE_WORK(work_thread0, wq_thread0);
static DECLARE_WORK(work_thread1, wq_thread1);

static int wq_thread_init(void)
{
	bool ret1, ret2;
	pr_info("wq_thread_init(): starting...\n");

	ret1 = queue_work(system_wq, &work_thread0);
	ret2 = queue_work(system_wq, &work_thread1);
	//ret2 = queue_work(system_highpri_wq, &work_thread1);
	if (!ret1 || !ret2)
		pr_info("queue_work() error!\n");

	pr_info("wq_thread_init(): end.\n");
	return 0;
}

static void wq_thread_exit(void)
{
	pr_info("wq_thread_exit().\n");
}

module_init(wq_thread_init);
module_exit(wq_thread_exit);
MODULE_AUTHOR("JaeJoon Jung <rgbi3307@nate.com>");
MODULE_LICENSE("GPL");
