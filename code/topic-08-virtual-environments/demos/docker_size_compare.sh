#!/usr/bin/env bash
# docker_size_compare.sh — CST-315 · Topic 8 · demos · INSTRUCTOR ONLY
# Build a single-stage image and a multi-stage image of lopeShell,
# then print both sizes side by side.
#
# Requires Docker. Run from the topic-08 demos/ directory.

set -e

if ! command -v docker >/dev/null 2>&1; then
    echo "docker not found; install Docker first."
    exit 1
fi

# Locate the milestone Dockerfile + source for the multi-stage build.
MILESTONE_DIR="$(cd "$(dirname "$0")/../../topic-08-virtual-environments/milestone" && pwd)"
STARTER_DIR="$(cd "$(dirname "$0")/../../topic-08-virtual-environments/starter" && pwd)"

echo "Building multi-stage image (lopeshell:multistage) ..."
docker build -t lopeshell:multistage "$MILESTONE_DIR" >/dev/null

echo "Building single-stage image  (lopeshell:singlestage) ..."
docker build -t lopeshell:singlestage "$STARTER_DIR" >/dev/null

echo
echo "image sizes:"
docker images --format "table {{.Repository}}:{{.Tag}}\t{{.Size}}" \
    | grep -E '^lopeshell:|^REPOSITORY'

echo
echo "Try them:"
echo "  docker run --rm -it lopeshell:multistage"
echo "  docker run --rm -it lopeshell:singlestage"
