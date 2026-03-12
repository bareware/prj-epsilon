CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size

SRC = $(wildcard src/*.c)
OBJ = $(subst .c,.o,$(subst src/,.build/obj/,$(SRC)))
DEP = $(subst .c,.d,$(subst src/,.build/dep/,$(SRC)))

CCFLAGS = -Wall -Wno-main -Oz -mthumb -nostdlib -mcpu=cortex-m0 -Imodules
LDFLAGS = -Wl,--gc-sections -Tlinker/stm32g0.ld

.PHONY: all

all: .build/bin/eps.bin

.build/bin/eps.bin: .build/bin/eps.elf
	$(CP) -O binary $< $@

.build/bin/eps.elf: $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) $(CCFLAGS) $(LDFLAGS) -o $@
	$(SZ) $@

.build/dep/%.d: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -M -MP -MT -c $< > $@

.build/obj/%.o: src/%.c .build/dep/%.d
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -rf .build

flash: .build/bin/eps.elf
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c "program .build/bin/eps.elf verify reset exit"

debug: .build/bin/eps.elf
	arm-none-eabi-gdb .build/bin/eps.elf \
		-ex "target extended-remote | openocd -c 'gdb port pipe; log_output openocd.log' -f interface/stlink.cfg -f target/stm32g0x.cfg" \
		-ex "monitor reset halt" \
		-ex "load" \
		-ex "b reset_handler" \
		-ex "continue" \
		-ex "tui new-layout custom_view {-horizontal src 1 asm 1} 2 regs 1 status 0 cmd 1" \
		-ex "layout custom_view"

-include $(DEP)

