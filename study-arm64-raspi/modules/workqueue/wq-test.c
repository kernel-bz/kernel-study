#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>

static void wq_test_func(struct work_struct *work)
{
	pr_info("wq_test_func(): starting...\n");
}

static void wq_test_func2(struct work_struct *work)
{
	pr_info("wq_test_func2(): starting...\n");
}

static DECLARE_WORK(test_work, wq_test_func);
static DECLARE_DELAYED_WORK(test_work2, wq_test_func2);

static int wq_test01_init(void)
{
	bool ret;
	pr_info("wq_test01_init(): starting...\n");

	ret = queue_work(system_long_wq, &test_work);

	if (!ret)
		pr_info("wq_test01_init(): queue_work() error!\n");

        //delayed 100 jiffies
	ret = queue_delayed_work(system_unbound_wq, &test_work2, 100);
	if (!ret)
		pr_info("wq_test01_init(): queue_delayed_work() error!\n");

	pr_info("wq_test01_init(): end.\n");
	return 0;
}

static void wq_test01_exit(void)
{
	pr_info("wq_test01_exit().\n");
}

module_init(wq_test01_init);
module_exit(wq_test01_exit);
MODULE_AUTHOR("JaeJoon Jung <rgbi3307@nate.com>");
MODULE_LICENSE("GPL");
