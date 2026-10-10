ifneq ($(KERNELRELEASE),)
# 内核构建系统调用时走这里
obj-m := devcfg.o
else
# 外部手动编译时走这里
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
