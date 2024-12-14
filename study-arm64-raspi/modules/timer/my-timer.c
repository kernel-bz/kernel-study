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

static struct timer_list my_timer1;
static struct timer_list my_timer2;
static unsigned int cnt = 0;

void my_timer1_callback(struct timer_list *timer)
{
        pr_info("Called(%u): my_timer1_callback().\n", cnt++);

        mod_timer(&my_timer1, jiffies + msecs_to_jiffies(TIMEOUT));
}

void my_timer2_callback(struct timer_list *timer)
{
        pr_info("Called(%u): my_timer2_callback().\n", cnt++);

        mod_timer(&my_timer2, jiffies + msecs_to_jiffies(TIMEOUT));

        if (cnt > 3) {
                int pending;

                pending = timer_pending(&my_timer1);
                pr_info("timer1 pending: %d\n", pending);

                del_timer_sync(&my_timer1);
                //timer_delete_sync(&my_timer1);

                //timer->func = NULL (shutdown)
                //timer_shutdown_sync(&my_timer1);

                pending = timer_pending(&my_timer1);
                pr_info("timer1 pending: %d\n", pending);

                mod_timer(&my_timer1, jiffies + msecs_to_jiffies(TIMEOUT));

                pending = timer_pending(&my_timer1);
                pr_info("timer1 pending: %d\n", pending);

                cnt = 0;
        }
}

static int __init my_timer_init(void)
{
        int pending;

        pr_info("init_module()...\n");

        timer_setup(&my_timer1, my_timer1_callback, 0);
        timer_setup(&my_timer2, my_timer2_callback, 0);

        pending = timer_pending(&my_timer1);
        pr_info("timer1 pending: %d\n", pending);      //0

        mod_timer(&my_timer1, jiffies + msecs_to_jiffies(TIMEOUT));
        mod_timer(&my_timer2, jiffies + msecs_to_jiffies(TIMEOUT));

        pending = timer_pending(&my_timer1);
        pr_info("timer1 pending: %d\n", pending);     //1

        pr_info("Done: my timer setup.\n");

        return 0;
}

static void __exit my_timer_exit(void)
{
        pr_info("cleanup_module()...\n");

        del_timer_sync(&my_timer1);
        //timer_shutdown_sync(&my_timer1);
        del_timer_sync(&my_timer2);
        //timer_shutdown_sync(&my_timer2);
        
        pr_info("Done: my timer delete.\n");
}

module_init(my_timer_init);
module_exit(my_timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JaeJoon Jung<rgbi3307@gmail.com>");
