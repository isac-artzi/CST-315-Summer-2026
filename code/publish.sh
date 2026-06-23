#!/usr/bin/env bash
#
# publish.sh — copy a topic's milestone + starter to a student-facing directory.
#
# `demos/` is NEVER copied. This is the safety boundary between instructor
# materials and student materials. Run this each week after that week's
# topic is finalized; commit and push the destination directory's git repo
# to your student-facing GitHub remote.
#
# Usage:
#   ./publish.sh <topic-number> <dest-directory>
#
# Example:
#   ./publish.sh 1 ~/CST-315-student-repo
#   ./publish.sh 02 ~/CST-315-student-repo
#
# Both "1" and "01" work for the topic number.

set -euo pipefail

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <topic-number> <dest-directory>"
    echo "Example: $0 1 ~/CST-315-student-repo"
    exit 1
fi

TOPIC_RAW="$1"
DEST="$2"

# Normalize topic number to two digits
TOPIC=$(printf "%02d" "${TOPIC_RAW#0}" 2>/dev/null || echo "$TOPIC_RAW")

# Find the matching directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC=$(find "$SCRIPT_DIR" -maxdepth 1 -type d -name "topic-${TOPIC}-*" | head -n1)

if [[ -z "$SRC" ]]; then
    echo "Error: no directory matching topic-${TOPIC}-* under $SCRIPT_DIR"
    exit 1
fi

mkdir -p "$DEST"
TOPIC_DEST="${DEST}/$(basename "$SRC")"
mkdir -p "$TOPIC_DEST"

echo "Source:      $SRC"
echo "Destination: $TOPIC_DEST"
echo

# Copy milestone and starter; explicitly exclude demos and build artifacts.
for sub in milestone starter; do
    if [[ -d "${SRC}/${sub}" ]]; then
        echo "Copying ${sub}/ ..."
        rsync -av --delete \
              --exclude='*.dSYM' --exclude='*.o' \
              --exclude='/hello' --exclude='/lopeShell' \
              --exclude='/pager' --exclude='/scheduler' \
              --exclude='/fs' --exclude='/scan' --exclude='/virus' \
              "${SRC}/${sub}/" "$TOPIC_DEST/${sub}/"
    fi
done

# Copy topic-level README
if [[ -f "${SRC}/README.md" ]]; then
    cp "${SRC}/README.md" "${TOPIC_DEST}/"
fi

echo
echo "✓ Published topic ${TOPIC} to ${TOPIC_DEST}"
echo "✓ demos/ NOT published (instructor-only)"
