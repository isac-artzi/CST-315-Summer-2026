#!/usr/bin/env bash
# Smoke test for Topic 2 starter (lopeShell v1).
# Same checks as milestone — will FAIL until you implement the TODOs.

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

if [[ ! -x ./lopeShell ]]; then
    echo "FAIL: ./lopeShell not found. Did you run 'make'?"
    exit 1
fi

out=$(printf 'echo hello\nexit\n' | ./lopeShell 2>/dev/null)
check "interactive: runs echo" "hello" "$out"

out=$(printf 'pwd\nexit\n' | ./lopeShell 2>/dev/null)
check "interactive: runs pwd"  "/"     "$out"

out=$(printf 'unknown_command_xyz\nexit\n' | ./lopeShell 2>&1)
check "unknown command reports error" "command not found" "$out"

BATCH=$(mktemp)
printf 'echo from-batch\necho line-two\n' > "$BATCH"
out=$(./lopeShell "$BATCH" 2>/dev/null)
check "batch: runs first line"  "from-batch" "$out"
check "batch: runs second line" "line-two"   "$out"
rm -f "$BATCH"

echo
echo "Topic 2 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
