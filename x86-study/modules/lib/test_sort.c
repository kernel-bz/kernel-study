/**
    M:	JaeJoon Jung <rgbi3307@nate.com>
    W:	https://www.kernel.bz
    F:	linux-modules/lib/sort/test_sort.c
    K:  /lib/test_sort.c
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
*/
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sort.h>
#include <linux/slab.h>
#include <linux/module.h>

/**
include/linux/sort.h, line 7 (as a prototype)
lib/sort.c, line 62 (as a function)

void sort(void *base, size_t num, size_t size,
	  int (*cmp)(const void *, const void *),
	  void (*swap)(void *, void *, int));
*/
#define DLEN 10

static int __init cmpint(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

static int __init test_sort_init(void)
{
	int *a, i, r = 1, err = -ENOMEM;

	pr_info("test_sort_init() starting...\n");

	a = kmalloc_array(DLEN, sizeof(*a), GFP_KERNEL);
	if (!a) {
        pr_err("kmalloc_array() failed.\n");
        return err;
	}

	pr_info("sort before data[%d]: ", DLEN);
	for (i = 0; i < DLEN; i++) {
		r = (r * 725861) % 6599;
		a[i] = r;
		pr_info("%d, ", a[i]);
	}
	pr_info("\n");

	sort(a, DLEN, sizeof(*a), cmpint, NULL);

	err = -EINVAL;
	pr_info("sort after data[%d]: ", DLEN);
	for (i = 0; i < DLEN-1; i++) {
		if (a[i] > a[i+1]) {
			pr_err("sort test has failed.\n");
			goto exit;
		}
		pr_info("%d, ", a[i]);
    }
    pr_info("%d\n", a[i]);
	err = 0;
	pr_info("sort test succeeded.\n");
exit:
	kfree(a);
	return err;
}

static void __exit test_sort_exit(void)
{
    pr_info("test_sort_exit()\n");
}

module_init(test_sort_init);
module_exit(test_sort_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JaeJoon Jung");
MODULE_DESCRIPTION("kernel module to test sort");
MODULE_VERSION("0.9.0");
