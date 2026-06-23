#!/usr/bin/env bash
# from_scratch_container.sh — CST-315 · Topic 8 · demos · INSTRUCTOR ONLY
# Build a tiny container-like environment using just chroot + unshare,
# without Docker. Run on Linux; may require sudo.
#
# What it does:
#   1. Make /tmp/mini-container with a minimal filesystem.
#   2. Copy busybox in if available; otherwise just sh + a few bins.
#   3. Use unshare to isolate UTS/PID namespaces.
#   4. chroot in and drop you at a shell.
#
# Press exit (or CTRL-D) to leave.

set -e

if [[ "$(uname -s)" != "Linux" ]]; then
    echo "Linux only. macOS analog: sandbox-exec(1)."
    exit 0
fi

ROOT=/tmp/mini-container
sudo rm -rf "$ROOT"
sudo mkdir -p "$ROOT"/{bin,lib,lib64,proc,tmp}

# Copy a small set of binaries and their libraries into the chroot.
for bin in /bin/sh /bin/ls /bin/cat /bin/echo /bin/hostname; do
    [[ -e "$bin" ]] || continue
    sudo cp "$bin" "$ROOT/bin/"
    for lib in $(ldd "$bin" 2>/dev/null | awk '{ for (i=1; i<=NF; i++) if ($i ~ /^\//) print $i }'); do
        sudo mkdir -p "$ROOT$(dirname "$lib")"
        sudo cp -L "$lib" "$ROOT$lib"
    done
done

echo "Entering mini-container at $ROOT ..."
echo "(inside, 'hostname' / 'ls /' / 'ps' will show the isolated view)"
sudo unshare --uts --pid --fork chroot "$ROOT" /bin/sh
echo "Returned to host."
