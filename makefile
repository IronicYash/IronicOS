# === Toolchain ===
AS      := nasm
CC      := i686-elf-gcc
LD      := i686-elf-ld
OBJCOPY := i686-elf-objcopy

# === Flags ===
CFLAGS  := -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
           -fno-stack-protector -fno-pic -fno-builtin -nostdlib
LDFLAGS := -T linker.ld -nostdlib

# === Directories ===
BUILD_DIR := build
ISO_DIR   := isodir
GRUB_DIR  := $(ISO_DIR)/boot/grub

# === Sources ===
C_SOURCES := \
	kernel/kernel_main.c \
	cpu/idt.c cpu/isr.c cpu/irq.c \
	lib/screen.c lib/keyboard.c lib/string.c lib/timer.c lib/shell.c

ASM_SOURCES := \
	kernel/multiboot_header.asm \
	kernel/entry.asm \
	cpu/idt_load.asm \
	cpu/isr.asm \
	cpu/irq.asm

# === Object Files ===
C_OBJS   := $(patsubst %.c,   $(BUILD_DIR)/%.o, $(C_SOURCES))
ASM_OBJS := $(patsubst %.asm, $(BUILD_DIR)/%_asm.o, $(ASM_SOURCES))
OBJS     := $(C_OBJS) $(ASM_OBJS)

# === Targets ===
all: $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Compile C files
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM files
$(BUILD_DIR)/%_asm.o: %.asm
	mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

# === Create ISO ===
iso: all
	mkdir -p $(GRUB_DIR)
	@if [ -f grub/grub.cfg ]; then \
	    cp grub/grub.cfg $(GRUB_DIR)/; \
	elif [ -f grub.cfg ]; then \
	    cp grub.cfg $(GRUB_DIR)/; \
	else \
	    echo 'menuentry "IronicOS" {\n    multiboot /boot/kernel.elf\n    boot\n}' > $(GRUB_DIR)/grub.cfg; \
	    echo "[INFO] No grub.cfg found, created default one."; \
	fi
	cp $(BUILD_DIR)/kernel.elf $(ISO_DIR)/boot/
	grub-mkrescue -o IronicOS.iso $(ISO_DIR)

# === QEMU Run ===
run: iso
	qemu-system-i386 -cdrom IronicOS.iso

# === Clean ===
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) IronicOS.iso

.PHONY: all clean run iso