#!/usr/bin/env bash
set -e

# === CONFIG ===
TARGET=i686-elf
PREFIX=/usr/local/cross
URL="https://github.com/lordmilko/i686-elf-tools/releases/download/12.2.0/i686-elf-tools-linux.tar.gz"

# === FUNCTIONS ===
check_installed() {
    if command -v $TARGET-gcc >/dev/null 2>&1; then
        echo "[OK] $TARGET-gcc is already installed."
        $TARGET-gcc --version
        exit 0
    fi
}

install_deps() {
    echo "[INFO] Installing required packages..."
    sudo apt update
    sudo apt install -y wget curl tar
}

download_toolchain() {
    echo "[INFO] Downloading prebuilt $TARGET toolchain..."
    wget -O /tmp/cross.tar.gz "$URL"
    echo "[INFO] Extracting to $PREFIX..."
    sudo mkdir -p "$PREFIX"
    sudo tar -xzf /tmp/cross.tar.gz -C "$PREFIX" --strip-components=1
    rm /tmp/cross.tar.gz
}

final_check() {
    echo "[INFO] Verifying installation..."
    if command -v $TARGET-gcc >/dev/null 2>&1; then
        echo "[OK] $TARGET-gcc installed successfully!"
        $TARGET-gcc --version
    else
        echo "[ERROR] Installation failed."
        exit 1
    fi
}

# === MAIN ===
check_installed
install_deps
download_toolchain
final_check

echo "[DONE] Toolchain ready. Add to PATH permanently with:"
echo "    echo 'export PATH=\"$PREFIX/bin:\$PATH\"' >> ~/.bashrc && source ~/.bashrc"
