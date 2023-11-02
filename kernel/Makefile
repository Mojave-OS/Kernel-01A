DRIVER_DIRECTORIES=$(foreach driver, $(DRIVERS), $(wildcard $(DDRIVER)/$(driver)/$(BOARD)/))
SRC_FILES = $(wildcard $(DKERN)/src/*.c)
OBJ_FILES = $(patsubst $(DKERN)/src/%.c, $(BUILD_LOC)/%.o, $(SRC_FILES))

all: $(OBJ_FILES) md5summary

$(BUILD_LOC)/%.o: $(DKERN)/src/%.c
	@$(CC) -c $(CCFLAGS) -I$(DRIVER_DIRECTORIES) -o $@ $<

.PHONY: md5summary
md5summary:
	@echo -e -n "$(CPRINT_BLUE)"
	@md5sum $(OBJ_FILES)
	@echo -e "$(CPRINT_END)"