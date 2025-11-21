// SPDX-License-Identifier: GPL-2.0  

#define pr_fmt(fmt) "param_test: " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int value;
module_param(value, int, 0600);

static char *name;
module_param(name, charp, 0600);

static int arr[4];
module_param_array(arr, int, NULL, 0600);

static int cb_value = 0;


static int param_ops_set(const char *val, const struct kernel_param *kp)
{
	int res = param_set_int(val, kp);

	if(res == 0)
	{
		printk(KERN_INFO "Call back function called...\n");
		printk(KERN_INFO "New value of cb_value = %d\n", cb_value);
		return 0;
	}
	return -1;
}

static int param_ops_get(char *buffer, const struct kernel_param *kp)
{
	return 0;
}

const struct kernel_param_ops my_param_ops = {
	.set = &param_ops_set,
	.get = &param_ops_get,
};

module_param_cb(cb_value, &my_param_ops, &cb_value, S_IRUGO|S_IWUSR);

static int __init my_module_init(void)
{
	int i;

	printk(KERN_INFO "===== Print Params =====\n");
	printk(KERN_INFO "value = %d \n", value);
	printk(KERN_INFO "cb_value = %d \n", cb_value);
	printk(KERN_INFO "name = %s\n", name);

	for(i = 0; i < sizeof(arr)/sizeof(int); i++)
	{
		printk(KERN_INFO "arr[%d] = %d \n", i, arr[i]);
	}
	return 0;
}

static void __exit my_module_exit(void)
{
	printk(KERN_INFO "Kernel Module Removed ...\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kernel.bz");
MODULE_DESCRIPTION("A simple parameter test module");
MODULE_VERSION("1:1.0");
