#obj-m := pci-test1.o
obj-m := pci-test2.o
VER := v6.6
BRD := jh7110
ARCH := riscv
ARCC := riscv64-unknown-linux-gnu-
#KDIR := /home/riscv/Projects/kernel/build/build-$(VER)-riscv-origin
KDIR := /home/riscv/Projects/starfive/build/kernel-$(VER)-$(BRD)

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
