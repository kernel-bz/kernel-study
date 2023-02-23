/**
    M:	JaeJoon Jung <rgbi3307@nate.com>
    W:	https://www.kernel.bz
    F:	linux-modules/test/hello.c
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
    printk("linux-modules: test/hello.c: hello_init().\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk("linux-modules: test/hello.c: hello_exit().\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JaeJoon Jung");
MODULE_DESCRIPTION("A Hello, World Kernel Module");
MODULE_VERSION("0.2.0");
