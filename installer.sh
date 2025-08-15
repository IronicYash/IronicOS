#!/usr/bin/env bash
set -euo pipefail

PREFIX="$HOME/opt/cross"
mkdir -p "$PREFIX"

URLS=(
  "https://github.com/lordmilko/i686-elf-tools/releases/download/13.2.0/i686-elf-tools-linux.zip"
  "https://github.com/cfenollosa/os-tutorial/releases/download/cc1/i686-elf-tools.tar.gz"
)

cd /tmp

for url in "${URLS[@]}"; do
  echo "Trying $url"
  if curl -fsSL "$url" -o /tmp/toolchain; then
    echo "Downloaded $url"
    case "$url" in
      *.zip)
        unzip -q /tmp/toolchain -d /tmp/toolchain_dir
        mkdir -p "$PREFIX"
        rsync -a /tmp/toolchain_dir/ "$PREFIX/"
        rm -rf /tmp/toolchain_dir
        ;;
      *.tar.gz)
        mkdir -p "$PREFIX"
        tar -xzf /tmp/toolchain -C "$PREFIX" --strip-components=1
        ;;
    esac
    rm -f /tmp/toolchain
    echo "Installed prebuilt toolchain to $PREFIX"
    echo "Add to PATH: export PATH=\"$PREFIX/bin:\$PATH\""
    exit 0
  else
    echo "Failed to download $url; trying next"
  fi
done

echo "No prebuilt toolchain available from the listed sources."
echo "If you want, run the original heavy installer to build from source."
exit 1
