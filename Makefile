# Compiler Related Variables

CC = /home/diego/Documents/Cross-Compilers/aarch64-none-elf/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf

# Directory Related Variables

BUILD = build
KERN = kernel
BOOT = bootblock

# Make Directives

all: kernel8.img

kernel8.img: $(BUILD)/$(BOOT)/boot.o $(BUILD)/$(KERN)/kernel.o
	echo "IMG"

$(BUILD)/$(KERN)/kernel.o: $(KERN)/Makefile
	echo "KERNEL"

$(BUILD)/$(BOOT)/boot.o: $(BOOT)/boot.S
	echo "BOOT"

clean: 
	- rm -rf $(BUILD)
	- mkdir $(BUILD)
