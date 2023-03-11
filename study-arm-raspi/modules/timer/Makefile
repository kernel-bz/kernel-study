#CONFIG_MODULE_SIG=n
#CONFIG_MODULE_SIG_ALL=n

obj-m := my-timer.o
#VER := v6.2 
VER := $1

KERNEL := kernel7
ARCH := arm
#ARCC := arm-linux-gnueabihf-
ARCC := arm-linux-gnueabi-
KDIR := /home/raspi/Projects/kernel/build/build-$(VER)-arm-rpi

#KERNEL := kernel8
#ARCH := arm64
#ARCC := aarch64-linux-gnu-
#KDIR := /home/raspi/Projects/kernel/build/build-$(VER)-arm64-rpi

MRUN := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARCC)

all: modules
modules:
	$(MRUN) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MRUN) -C $(KDIR) M=${shell pwd} clean
