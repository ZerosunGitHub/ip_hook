target = ip_hook
obj-m := $(target).o
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	rm -rf *.o *.mod.c
	rm -rf Module.symvers .*cmd .tmp_versions
	rm -rf *.order
install:
	insmod $(target).ko
uninstall:
	rmmod $(target).ko
clean:
	rm -rf *.o *.mod.c *.ko
	rm -rf Module.symvers .*cmd .tmp_versions
	rm -rf *.order
