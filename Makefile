obj-m = lcd.o
lcd-y = src/lcd.o src/io.o

KVERSION = $(shell uname -r)

all: module clean_int

module:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean_int:
	rm */*.o *.o */.*.cmd .*.cmd *.mod *.c *.symvers *.order

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean