obj-m := pci-test.o

KDIR := /home/jungjaejoon/Projects/kernel/build/build_v4.14.146_x86_64

all:
	make -C $(KDIR) M=${shell pwd} modules

clean:
	make -C $(KDIR) M=${shell pwd} clean
