# =========================
# IronicOS Production Makefile
# =========================

# === Toolchain ===
CROSS_PREFIX ?= $(HOME)/opt/cross/bin/i686-elf-
AS      := nasm
CC      := $(CROSS_PREFIX)gcc
LD      := $(CROSS_PREFIX)ld
OBJCOPY := $(CROSS_PREFIX)objcopy

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
	lib/screen.c lib/keyboard.c lib/string.c lib/timer.c lib/shell.c lib/serial.c

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

# =========================
# Build Targets
# =========================

.PHONY: all clean run iso

all: $(BUILD_DIR)/kernel.elf
	@echo "[OK] Build complete."

# Link kernel
$(BUILD_DIR)/kernel.elf: $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "[OK] Kernel linked -> $@"

# Compile C files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "[OK] Compiled C -> $<"

# Assemble ASM files
$(BUILD_DIR)/%_asm.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@
	@echo "[OK] Assembled ASM -> $<"

# =========================
# ISO Creation
# =========================

iso: $(BUILD_DIR)/kernel.elf
	@echo "[INFO] Creating ISO directory structure..."
	@mkdir -p $(GRUB_DIR)
	@cp $(BUILD_DIR)/kernel.elf $(ISO_DIR)/boot/
	@if [ -f grub/grub.cfg ]; then \
	    cp grub/grub.cfg $(GRUB_DIR)/; \
	elif [ -f grub.cfg ]; then \
	    cp grub.cfg $(GRUB_DIR)/; \
	else \
	    echo 'menuentry "IronicOS" {\n    multiboot /boot/kernel.elf\n    boot\n}' > $(GRUB_DIR)/grub.cfg; \
	    echo "[INFO] No grub.cfg found, created default one."; \
	fi
	@echo "[INFO] Building ISO image..."
	grub-mkrescue -o IronicOS.iso $(ISO_DIR) --directory=/usr/lib/grub/i386-pc
	@ls -lh IronicOS.iso
	@echo "[OK] ISO created -> IronicOS.iso"

# =========================
# QEMU Run
# =========================

run: iso
	@echo "[INFO] Running in QEMU..."
	qemu-system-i386 -cdrom IronicOS.iso

# =========================
# Cleanup
# =========================

clean:
	@echo "[INFO] Cleaning build files..."
	rm -rf $(BUILD_DIR) $(ISO_DIR) IronicOS.iso
	