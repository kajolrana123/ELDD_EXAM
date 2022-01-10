obj-m := main.o

KERNSRC := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

modules:

	make -C $(KERNSRC) M=$(PWD) modules
	gcc -o read_app read_app.c
	gcc -o write_app write_app.c
	gcc -o ioctl_app ioctl_app.c
	mknod -m 666 /dev/DESD_Device c 100 0

modules_install:
	make -C $(KERNSRC) M=$(PWD) modules_install
	depmod -a

clean:
	make -C $(KERNSRC) M=$(PWD) clean
	rm -rf read_app write_app ioctl_app



	
