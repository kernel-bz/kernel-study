obj-m := gpio-int.o

VER := v6.6
ARCH := arm64
BOARD := pi5
BDO := hailo

ARCC := aarch64-linux-gnu-

BDIR := build-$(VER)-$(ARCH)-$(BOARD)-$(BDO)
KDIR := /home/raspi/Projects/kernel/build/$(BDIR)

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
