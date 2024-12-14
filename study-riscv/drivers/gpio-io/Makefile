obj-m := gpio-io.o
#VER := v6.6
VER := $1
ARCH := riscv
ARCC := riscv64-unknown-linux-gnu-
#KDIR := /home/riscv/Projects/kernel/build/build-$(VER)-riscv-origin
#KDIR := /mnt/sda3/projects/RISCV/StarFive/linux
KDIR := /home/riscv/Projects/starfive/build/kernel-$(VER)-jh7110

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
