obj-m += kbdirq.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	-sudo rmmod kbdirq
	sudo dmesg -C
	sudo insmod kbdirq.ko
#	sudo rmmod kbdirq.ko
	dmesg

