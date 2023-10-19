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
CC = /home/diego/Documents/Cross-Compilers/aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu

#
# Directives
#
all: setdirs $(OBJ_FILES)

$(BUILD_DIR)/%.o: %.c
	$(CC)-gcc -c -I$(INCLUDE_DIR) $(CCFLAGS) -o $@ $<

setdirs:
	- mkdir -p $(BUILD_DIR)/$(SRC_DIR)
