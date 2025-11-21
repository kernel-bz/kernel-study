#CONFIG_MODULE_SIG=n
#CONFIG_MODULE_SIG_ALL=n

obj-m := param_test.o

VER := v6.18
#VER := $1
BRD := jh7110
#BRD := $2
ARCH := riscv
ARCC := riscv64-linux-gnu-
KDIR := /home/riscv/projects/kernel/build/kernel-$(VER)-$(BRD)

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
