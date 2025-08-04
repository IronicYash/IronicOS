# ===============================
#         IronicOS Makefile
# ===============================

# === Config ===
TARGET        = kernel.elf
ISO_NAME      = IronicOS.iso
BUILD_DIR     = build
ISO_DIR       = isodir
GRUB_DIR      = $(ISO_DIR)/boot/grub

KERNEL_DIR    = kernel
CPU_DIR       = cpu
LIB_DIR       = lib

# === Tools ===
CC      = i686-elf-gcc
LD      = i686-elf-ld
AS      = nasm
QEMU    = qemu-system-i386
CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32
ASFLAGS = -f elf32
LDFLAGS = -T linker.ld -nostdlib

# === Sources ===
C_SOURCES  = $(wildcard $(KERNEL_DIR)/*.c $(CPU_DIR)/*.c $(LIB_DIR)/*.c)
ASM_SOURCES = $(wildcard $(KERNEL_DIR)/*.asm $(CPU_DIR)/*.asm)
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)))

# === Default ===
all: iso

# === Compile C Files ===
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(CPU_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# === Assemble ASM Files ===
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(CPU_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# === Link kernel ===
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# === Make ISO ===
iso: $(BUILD_DIR)/$(TARGET)
	@mkdir -p $(GRUB_DIR)
	cp grub.cfg $(GRUB_DIR)/
	cp $< $(ISO_DIR)/boot/$(TARGET)
	grub-mkrescue -o $(ISO_NAME) $(ISO_DIR)

# === Run kernel in QEMU ===
run: iso
	$(QEMU) -cdrom $(ISO_NAME)

# === Debug with QEMU GDB ===
debug: iso
	$(QEMU) -cdrom $(ISO_NAME) -s -S

# === Clean build and ISO ===
clean:
	rm -rf $(BUILD_DIR) $(ISO_NAME) $(ISO_DIR)/boot/$(TARGET)

# === Help ===
help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build everything (same as make iso)"
	@echo "  iso       - Build the bootable ISO image"
	@echo "  run       - Run the OS using QEMU"
	@echo "  debug     - Run QEMU in GDB debugging mode"
	@echo "  clean     - Remove build artifacts"
	@echo "  help      - Show this help message"

.PHONY: all clean iso run debug help