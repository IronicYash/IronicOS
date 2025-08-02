# Makefile for IronicOS

# Compiler and tools
AS      := nasm
CC      := gcc
LD      := ld
OBJCOPY := objcopy
GRUBDIR := isodir/boot/grub

# Flags
CFLAGS  := -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS := -T linker.ld -nostdlib -m elf_i386

# Directories
SRC_DIR := src
BOOT_DIR := boot
BUILD_DIR := build
KERNEL_DIR := kernel

# Sources
KERNEL_SRC := $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_OBJ := build/multiboot_header.o build/entry.o build/kernel_main.o

# Output
ISO_NAME := IronicOS.iso
BIN_NAME := kernel.bin

all: iso

# Compile C kernel source files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble bootloader
$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm
	mkdir -p $(BUILD_DIR)
	$(AS) -f bin $< -o $@

# Link kernel
$(BUILD_DIR)/$(BIN_NAME): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.elf $^
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $@

# Create ISO with GRUB
iso: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/$(BIN_NAME)
	mkdir -p $(GRUBDIR)
	mkdir -p isodir/boot
	cp $(BUILD_DIR)/boot.bin $(GRUBDIR)/boot.bin
	cp $(BUILD_DIR)/$(BIN_NAME) isodir/boot/kernel.bin
	cp grub.cfg $(GRUBDIR)/
	grub-mkrescue -o $(ISO_NAME) isodir

# Run in QEMU
run: iso
	qemu-system-i386 -cdrom $(ISO_NAME)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) isodir $(ISO_NAME)

.PHONY: all clean run iso

# Assemble multiboot header
$(BUILD_DIR)/multiboot_header.o: $(KERNEL_DIR)/multiboot_header.asm
	mkdir -p $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Assemble kernel entry
$(BUILD_DIR)/entry.o: $(KERNEL_DIR)/entry.asm
	mkdir -p $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@