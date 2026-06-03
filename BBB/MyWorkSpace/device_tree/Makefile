obj-m := led_act_driver.o
KDIR  := /opt/poky/4.0.34/sysroots/arm1176jzfshf-vfp-poky-linux-gnueabi/lib/modules/5.15.92/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-poky-linux-gnueabi- clean

.PHONY: all clean
