# Makefile for IronicOS

# Compiler and tools
AS      := nasm
CC      := i686-elf-gcc
LD      := i686-elf-ld
OBJCOPY := i686-elf-objcopy
GRUBDIR := isodir/boot/grub

# Flags
CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS := -T linker.ld -nostdlib

# Directories
SRC_DIR := src
BOOT_DIR := boot
BUILD_DIR := build
KERNEL_DIR := kernel

# Sources
KERNEL_SRC := $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_OBJ := $(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SRC))

# Output
ISO_NAME := IronicOS.iso
BIN_NAME := kernel.bin

all: iso

# Compile C kernel source files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble bootloader
$(BUILD_DIR)/boot.o: $(BOOT_DIR)/boot.asm
	mkdir -p $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

# Link kernel
$(BUILD_DIR)/$(BIN_NAME): $(BUILD_DIR)/boot.o $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Create ISO with GRUB
iso: $(BUILD_DIR)/$(BIN_NAME)
	mkdir -p $(GRUBDIR)
	cp $< $(GRUBDIR)/kernel.bin
	cp grub.cfg $(GRUBDIR)/
	grub-mkrescue -o $(ISO_NAME) isodir

# Run in QEMU
run: iso
	qemu-system-i386 -cdrom $(ISO_NAME)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) isodir $(ISO_NAME)

.PHONY: all clean run iso
