#CONFIG_MODULE_SIG=n
#CONFIG_MODULE_SIG_ALL=n

obj-m := hello.o
#VER := v6.2
VER := $1
ARCH := riscv
ARCC := riscv64-unknown-linux-gnu-
#KDIR := /home/riscv/Projects/kernel/build/build-$(VER)-riscv-origin
KDIR := /mnt/sda3/projects/RISCV/StarFive/linux

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
