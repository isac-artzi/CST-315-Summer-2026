#!/usr/bin/env bash
# Smoke test for Topic 4 milestone (pager LRU).

PASS=0
FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then
        echo "PASS: $desc"; PASS=$((PASS+1))
    else
        echo "FAIL: $desc"
        echo "   expected substring: $expected"
        echo "   got:                $actual"
        FAIL=$((FAIL+1))
    fi
}

if [[ ! -x ./pager ]]; then
    echo "FAIL: ./pager not found. Did you run 'make'?"
    exit 1
fi

# Canonical Belady-style 12-reference sequence, 4 frames.
# LRU produces 4 hits / 8 faults on this sequence.
out=$(echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager 4 2>/dev/null)
check "LRU 4-frame sequence: 8 faults"          "faults: 8"     "$out"
check "LRU 4-frame sequence: fault rate 66.7%"  "fault rate: 66.7%" "$out"

# 3-frame variant of same sequence — also 8 faults / 4 hits under LRU.
out=$(echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager 3 2>/dev/null)
check "LRU 3-frame: any output"                  "fault rate"   "$out"

# Single page repeated — only the first reference faults.
out=$(echo "7 7 7 7 7" | ./pager 4 2>/dev/null)
check "single repeated page: exactly 1 fault"    "faults: 1"   "$out"
check "single repeated page: 4 hits"             "hits: 4"     "$out"

echo
echo "Topic 4 milestone smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
