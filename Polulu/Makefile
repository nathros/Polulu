PKG=/usr

ARCH=arm-none-eabi
CC2=$(ARCH)-gcc
HCC	= /usr/bin/gcc
OBJCOPY2=$(ARCH)-objcopy

SOURCERY=$(PKG)/sourcery-g++-lite-arm-eabi-2010.09.51-i686-1
GNU_VERSION=4.5.0
THUMB2GNULIB=$(SOURCERY)/$(ARCH)/lib/$(GNU_VERSION)/thumb2
THUMB2GNULIB2=$(SOURCERY)/$(ARCH)/lib/thumb2

CC=$(SOURCERY)/bin/$(CC2)
OBJCOPY=$(SOURCERY)/bin/$(OBJCOPY2)

# "Cortex Microcontroller Software Interface Standard" Startup files
CMSIS=$(PKG)/lpc1700-cmsis-lite-2011.01.26-i686-1
CMSISINCLUDES=-I$(CMSIS)/include
CMSISFL=$(CMSIS)/lib/core_cm3.o \
	$(CMSIS)/lib/system_LPC17xx.o \
	$(CMSIS)/lib/startup_LPC17xx.o
LDSCRIPT = $(CMSIS)/lib/ldscript_rom_gnu.ld

CFLAGS=-mcpu=cortex-m3  -mthumb -Wall -mapcs-frame  -D__thumb2__=1 \
  -msoft-float  -fno-hosted  -mtune=cortex-m3 \
  -march=armv7-m  -mfix-cortex-m3-ldrd   -ffunction-sections  -fdata-sections \
          -D__RAM_MODE__=0 $(CMSISINCLUDES) -I. -c

LDFLAGS=$(CMSISFL) -static -mcpu=cortex-m3 -mthumb -mthumb-interwork \
	   -Wl,--start-group -L$(THUMB2GNULIB) -L$(THUMB2GNULIB2) \
           -lc -lg -lgcc -lm  -Wl,--end-group \
	   -Xlinker -Map -Xlinker bin/lpc1700.map -Xlinker -T $(LDSCRIPT) \

LDFLAGS+= -L$(CMSIS)/lib -lDriversLPC17xxgnu -lBSP -lCDL -lnxpUSBlib

EXECNAME	= bin/main

SRC		= main.c debug.c uart.c ir.c lcd.c gpio.c led.c motor.c\
		  delay.c line_sensors.c utilities.c \
		  mouse.c coordinate.c advanced_movement.c rit.c pid.c line_follow.c\
		  common.c odometry.c
OBJ		= $(SRC:.c=.o) usbmouse/KeyboardHost.o

all: 	main
	@echo "Build finished"

indent:
	indent -kr -nut $(SRC)

docs:
	doxygen Doxyfile

main: $(OBJ)
	$(CC) -o $(EXECNAME) $(OBJ) $(LDFLAGS)
	$(OBJCOPY) -I elf32-little -O binary $(EXECNAME) $(EXECNAME).bin

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

# clean out the source tree ready to re-build
.PHONY: clean
clean:
	rm -f `find . | grep \~`
	rm -f *.swp *.o *.log
	rm -f test/*.swp test/*.o test/*.log
	rm -f *.d *.srec */*.a bin/*.map
	rm -f *.elf *.wrn bin/*.bin log *.hex
	rm -f deps.mk
	rm -f $(EXECNAME)

# install software to board, remember to sync the file systems
install:
	rm /media/MBED/* -rf
	@echo "Copying " $(EXECNAME) "to the MBED file system"
	cp $(EXECNAME).bin /media/MBED &
	sync
	@echo "Now press the reset button on all MBED file systems"

deps.mk:
	gcc -MM $(SRC) > deps.mk

include deps.mk
