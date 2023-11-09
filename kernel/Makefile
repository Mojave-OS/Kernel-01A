DRIVER_DIRECTORIES=$(foreach driver, $(DRIVERS), $(wildcard $(DDRIVER)/$(driver)/$(BOARD)/))
SRC_FILES = $(wildcard $(DKERN)/src/*.c)
OBJ_FILES = $(patsubst $(DKERN)/src/%.c, $(BUILD_LOC)/%.o, $(SRC_FILES))

DRIVER_INCLUDES=$(foreach driver, $(DRIVERS), $(wildcard -I$(DDRIVER)/$(driver)/$(BOARD)/))
KERNEL_INCLUDES=$(DKERN)/include

all: $(OBJ_FILES) md5summary

$(BUILD_LOC)/%.o: $(DKERN)/src/%.c
	@$(CC) -c $(CCFLAGS) -I$(DDRIVER) -I$(KERNEL_INCLUDES) -o $@ $<

.PHONY: md5summary
md5summary:
	@echo -e -n "$(CPRINT_BLUE)"
	@md5sum $(OBJ_FILES)
	@echo -e "$(CPRINT_END)"