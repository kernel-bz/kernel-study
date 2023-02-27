// SPDX-License-Identifier: GPL-2.0-only
/*
 *      hello.c
 *
 *      Jung-JaeJoon <rgbi3307@gmail.com>
 */

#define pr_fmt(fmt)     "MY-MODULE: " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init example_init(void)
{
        pr_info("Hello world.\n");
        return 0;
}

static void __exit example_exit(void)
{
        pr_info("Goodbye world.\n");
}

module_init(example_init);                                                      
module_exit(example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux");
