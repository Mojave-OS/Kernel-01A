#
# Directory Related Variables
#
BUILD_DIR = build
BOOTB_DIR = bootblock
SRC_DIR = $(BOOTB_DIR)/src

#
# File Aggregation
#
SRC_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(patsubst %.S, $(BUILD_DIR)/%.o, $(SRC_FILES))


#
# Flags for Compiling
#
ASFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib
CC = /home/diego/Documents/Cross-Compilers/aarch64-none-elf/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf

#
# Directives
#
all: setdirs $(OBJ_FILES)

$(BUILD_DIR)/%.o: %.S
	$(CC)-gcc -c $(ASFLAGS) -o $@ $<

setdirs:
	- mkdir -p $(BUILD_DIR)/$(SRC_DIR)
