obj-m := gpio-io.o
#VER := v6.6
VER := $1
ARCH := riscv
ARCC := riscv64-unknown-linux-gnu-
#KDIR := /home/riscv/Projects/starfive/build/kernel-$(VER)-jh7110
KDIR := /home/jh7110/projects/kernel/build/kernel-v6.6-jh7110/

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
