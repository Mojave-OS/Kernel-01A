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
ASFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -lgcc
LDFLAGS = -nostdlib
CC = /home/diego/Documents/Cross-Compilers/aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu

#
# Directives
#
all: setdirs $(OBJ_FILES)

$(BUILD_DIR)/%.o: %.S
	$(CC)-gcc -c $(ASFLAGS) -o $@ $<

setdirs:
	- mkdir -p $(BUILD_DIR)/$(SRC_DIR)
