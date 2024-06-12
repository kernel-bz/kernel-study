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
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <asm/barrier.h>
#include <linux/delay.h>

struct user_worker {
        char name[20];
        struct task_struct *task;
};

struct user_worker *worker0;
struct user_worker *worker1;
int s0;

static int wq_thread0(void *work)
{
	pr_info("wq_thread0(): starting...\n");

        while(1) {
                int t1, t2, t3;
                int a0;

                t1 = 1;
                s0 = t1;

                t2 = 2;
                s0 = t2;

                smp_mb();
                msleep(10);
                smp_mb();

                a0 = s0;
                if (a0 != 2)
                        pr_info("a0 = %d\n", a0);

                t3 = 3;
                s0 = t3;

                schedule();
        }

	pr_info("wq_thread0(): end.\n");
        return 0;
}

static int wq_thread1(void *work)
{
	pr_info("wq_thread1(): starting...\n");

        while(1) {
                int t4, t5;

                t4 = 4;
                s0 = t4;

                schedule();

                t5 = 5;
                s0 = t5;

                schedule();
        }

	pr_info("wq_thread1(): end.\n");
        return 0;
}

static int user_kthread0_create(void)
{
        worker0 = kzalloc(sizeof(*worker0), GFP_KERNEL);
        if (!worker0) {
                pr_err("Failed to allocate worker0 at the alloc_worker()\n");
                return -ENOMEM;
        }
        worker0->task = kthread_create(wq_thread0, worker0, "%s", "usr_worker0");
        if (IS_ERR(worker0->task)) {
                pr_err("Failed to allocate worker0 at the kthread_create()\n");
                return -ENOMEM;
        }
        wake_up_process(worker0->task);
        return 0;
}

static int user_kthread1_create(void)
{
        worker1 = kzalloc(sizeof(*worker1), GFP_KERNEL);
        if (!worker1) {
                pr_err("Failed to allocate worker1 at the alloc_worker()\n");
                return -ENOMEM;
        }
        worker1->task = kthread_create(wq_thread1, worker1, "%s", "usr_worker1");
        if (IS_ERR(worker1->task)) {
                pr_err("Failed to allocate worker1 at the kthread_create()\n");
                return -ENOMEM;
        }
        wake_up_process(worker1->task);
        return 0;
}

static int wq_thread_init(void)
{
	bool ret1, ret2;
	pr_info("wq_thread_init(): starting...\n");

        ret1 = user_kthread0_create();
        ret2 = user_kthread1_create();

	if (ret1 || ret2)
		pr_info("wq_thread_init() error!\n");

	pr_info("wq_thread_init(): end.\n");
	return 0;
}

static void wq_thread_exit(void)
{
        //kthread_stop(worker0->task);
        //kthread_stop(worker1->task);
        kfree(worker0);
        kfree(worker1);
	pr_info("wq_thread_exit().\n");
}

module_init(wq_thread_init);
module_exit(wq_thread_exit);
MODULE_AUTHOR("JaeJoon Jung <rgbi3307@nate.com>");
MODULE_LICENSE("GPL");
