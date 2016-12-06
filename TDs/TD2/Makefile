KERNEL_HEADERS=/lib/modules/$(shell uname -r)/build
obj-m := modtest.o
all:
	@$(MAKE) -C $(KERNEL_HEADERS) M=$(PWD) modules
	
envoi : 
	scp * pi@192.168.1.5:/home/pi/mkermarquer/TD2

clean:
	@$(MAKE) -C $(KERNEL_HEADERS) M=$(PWD) clean