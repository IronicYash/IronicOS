# === Directories ===
BUILD_DIR = build
ISO_DIR = $(BUILD_DIR)/iso
BOOT_DIR = boot
KERNEL_DIR = kernel

# === Files ===
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
ISO_FILE = $(BUILD_DIR)/IronicOS.iso

# === Tools ===
AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

# === Flags ===
CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

# === Default Target ===
all: $(ISO_FILE)

# === Build ISO ===
$(ISO_FILE): $(KERNEL_ELF)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf
	echo 'set timeout=0'                          > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "IronicOS" {'                >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.elf'          >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}'                                     >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

# === Link kernel (boot + C) ===
$(KERNEL_ELF): $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(BOOT_OBJ) $(KERNEL_OBJ)

# === Assemble bootloader ===
$(BOOT_OBJ): $(BOOT_DIR)/boot.asm
	$(AS) -f elf32 $< -o $@

# === Compile kernel ===
$(KERNEL_OBJ): $(KERNEL_DIR)/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

# === Clean Build Files ===
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.iso $(ISO_DIR)

.PHONY: all clean
