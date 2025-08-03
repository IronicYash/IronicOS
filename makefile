# === TOOLS ===
AS      := nasm
CC      := gcc
LD      := ld
GRUBDIR := isodir/boot/grub
BUILD   := build
ISO     := IronicOS.iso
INCLUDES := -Ilib

# === FLAGS ===
CFLAGS  := -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
		   -fno-exceptions -fno-pic -fno-stack-protector \
		   -fno-builtin -nostdlib $(INCLUDES) 
		   
LDFLAGS := -T linker.ld -nostdlib -m elf_i386

# === SOURCES ===
KERNEL_SRC := kernel/kernel_main.c
LIB_SRC    := lib/screen.c

KERNEL_OBJ := \
	$(BUILD)/multiboot_header.o \
	$(BUILD)/entry.o \
	$(BUILD)/kernel_main.o \
	$(BUILD)/screen.o \
	$(BUILD)/memory.o \
	$(BUILD)/keyboard.o \
	$(BUILD)/ports.o \
	$(BUILD)/string.o \
	$(BUILD)/isr.o \
	$(BUILD)/idt.o \
	$(BUILD)/irq.o \
	$(BUILD)/idt_flush.o \
	$(BUILD)/irq_asm.o


KERNEL_ELF := $(BUILD)/kernel.elf

# === DEFAULT ===
all: iso

# === COMPILE ===
$(BUILD)/kernel_main.o: kernel/kernel_main.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/screen.o: lib/screen.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/multiboot_header.o: kernel/multiboot_header.asm
	mkdir -p $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/entry.o: kernel/entry.asm
	mkdir -p $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/memory.o: kernel/memory.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard.o: lib/keyboard.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ports.o: cpu/ports.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/string.o: lib/string.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c lib/string.c -o $(BUILD)/string.o

$(BUILD)/isr.o: cpu/isr.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: cpu/idt.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/irq.o: cpu/irq.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt_flush.o: cpu/idt_flush.asm
	mkdir -p $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/irq_asm.o: cpu/irq.asm
	mkdir -p $(BUILD)
	$(AS) -f elf32 $< -o $@

# === LINK ELF KERNEL ===
$(KERNEL_ELF): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJ)

# === BUILD ISO ===
iso: $(KERNEL_ELF)
	mkdir -p $(GRUBDIR)
	cp grub.cfg $(GRUBDIR)/
	cp $(KERNEL_ELF) isodir/boot/kernel.elf
	grub-mkrescue -o $(ISO) isodir

# === RUN ===
run: iso
	qemu-system-i386 -cdrom $(ISO)

# === CLEAN ===
clean:
	rm -rf build isodir $(ISO)

.PHONY: all clean iso run

# === INSTALL ===
install: iso	
	@echo "Installing IronicOS ISO to /usr/local/share/ironicos"
	mkdir -p /usr/local/share/ironicos
	cp $(ISO) /usr/local/share/ironicos/
	@echo "Installation complete. You can now run IronicOS using 'make run'."

# === UNINSTALL ===
uninstall:
	@echo "Uninstalling IronicOS ISO from /usr/local/share/ironicos"
	rm -f /usr/local/share/ironicos/$(ISO)
	@echo "Uninstallation complete."

# === HELP ===
help:
	@echo "Makefile for IronicOS"
	@echo "Usage:"
	@echo "  make all       - Build the ISO image"
	@echo "  make run       - Run IronicOS in QEMU"
	@echo "  make clean     - Clean build artifacts"
	@echo "  make install   - Install IronicOS ISO to /usr/local/share/ironicos"
	@echo "  make uninstall - Uninstall IronicOS ISO from /usr/local/share/ironicos"
	@echo "  make help      - Show this help message"