obj-m += mcp251x.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod mcp251x.ko
	sudo rmmod mcp251x.ko
	dmesg
