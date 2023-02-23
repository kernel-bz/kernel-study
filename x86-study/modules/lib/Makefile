#loading out-of-tree module taints kernel.
#module verification failed: signature and/or required key missing
CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n

KBUILD_CFLAGS += -save-temps=obj

obj-m := test_sort.o
obj-m += test_list_sort.o
obj-m += rbtree_test.o
obj-m += test_overflow.o
obj-m += percpu_test.o
obj-m += test_user_copy.o
obj-m += test_xarray.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
