#
# Directory Related Variables
#
BUILD_DIR = build
INCLUDE_DIR = kernel/include
SRC_DIR = kernel/src

#
# File Aggregation
#
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))


#
# Flags for Compiling
#
CCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -lgcc
LDFLAGS = -nostdlib
CC = /home/diego/Documents/Cross-Compilers/aarch64-none-elf/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf

#
# Directives
#
all: setdirs $(OBJ_FILES)

$(BUILD_DIR)/%.o: %.c
	$(CC)-gcc -c -I$(INCLUDE_DIR) $(CCFLAGS) -o $@ $<

setdirs:
	- mkdir -p $(BUILD_DIR)/$(SRC_DIR)
