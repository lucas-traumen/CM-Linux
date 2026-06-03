obj-m := main.o

PWD := $(shell pwd)

KERNEL_SRC := $(SDKTARGETSYSROOT)/usr/src/kernel

ARCH := arm
CROSS_COMPILE := arm-poky-linux-gnueabi-

arm:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean-arm:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

host:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean-host:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean