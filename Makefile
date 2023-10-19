#
# Directory Related Variables
#
BUILD_DIR = build
KERNEL = kernel
BOOT = bootblock

INCLUDE_DIR = $(KERNEL)/include
KERN_SRC_DIR = $(KERNEL)/src
BOOT_SRC_DIR = $(BOOT)/src
SDCARD = bootfs

#
# File Aggregation
#
KERN_OBJ_FILES = $(wildcard $(BUILD_DIR)/$(KERN_SRC_DIR)/*.o)
BOOT_OBJ_FILES = $(wildcard $(BUILD_DIR)/$(BOOT_SRC_DIR)/*.o)

#
# Flags for Compiling
#
CCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib
LINKF = linker.ld
CC = /home/diego/Documents/Cross-Compilers/aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu

#
# Directives
#
all: kernel8.img

kernel8.img: 
	$(CC)-ld $(LDFLAGS) -T $(LINKF) $(BOOT_OBJ_FILES) $(KERN_OBJ_FILES) -o $(patsubst %.img, %.elf, $@)
	$(CC)-objcopy -O binary $(patsubst %.img, %.elf, $@) $@

$(BUILD_DIR)/%.o: %.c
	$(CC)-gcc -c -I$(INCLUDE_DIR) $(CCFLAGS) -o $@ $<

update:
	make -f $(KERNEL)/Makefile
	make -f $(BOOT)/Makefile

clean:
	- rm -rf build/*
	- rm *.elf
	- rm *.img

transfer:
	cp ./kernel8.img /media/diego/$(SDCARD)/kernel8.img
	md5sum ./kernel8.img /media/diego/$(SDCARD)/kernel8.img
