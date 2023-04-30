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

int __init hello2_init(void) {
    printk("linux-modules: test/hello2.c: hello2_init().\n");
    return 0;
}

void __exit hello2_exit(void) {
    printk("linux-modules: test/hello2.c: hello2_exit().\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JaeJoon Jung");
MODULE_DESCRIPTION("A Hello2, World Kernel Module");
MODULE_VERSION("0.1.2");
