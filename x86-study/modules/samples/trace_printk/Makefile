#loading out-of-tree module taints kernel.
#module verification failed: signature and/or required key missing
CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n

obj-m := trace-printk.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
