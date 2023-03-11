// SPDX-License-Identifier: GPL-2.0-only                                        
/*                                                                              
 *      my-timer.c - kernel timer test module
 *                                                                              
 *      Jung-JaeJoon <rgbi3307@gmail.com>                                                                              
 */   

#define pr_fmt(fmt)     "MY-TIMER: " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define TIMEOUT 5000    //milliseconds to expire

static struct timer_list my_timer;
static unsigned int cnt = 0;

void my_timer_callback(struct timer_list *timer)
{
        pr_info("Called(%u): my_timer_callback().\n", cnt++);

        mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

static int __init my_timer_init(void)
{
        pr_info("init_module()...\n");

        timer_setup(&my_timer, my_timer_callback, 0);

        mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));

        pr_info("Done: my timer setup.\n");

        return 0;
}

static void __exit my_timer_exit(void)
{
        pr_info("cleanup_module()...\n");

        //del_timer(&my_timer);
        del_timer_sync(&my_timer);
        
        pr_info("Done: my timer delete.\n");
}

module_init(my_timer_init);
module_exit(my_timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JaeJoon Jung<rgbi3307@gmail.com>");
