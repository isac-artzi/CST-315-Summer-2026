#!/usr/bin/env bash
# Smoke test for Topic 3 milestone (lopeShell v2).

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

# Topic 2 carryover
out=$(printf 'echo hello\nexit\n' | ./lopeShell 2>/dev/null)
check "single command still works" "hello" "$out"

# Multi-command, semicolon-separated
out=$(printf 'echo aaa; echo bbb; echo ccc\nexit\n' | ./lopeShell 2>/dev/null)
check "multi: first command"  "aaa" "$out"
check "multi: second command" "bbb" "$out"
check "multi: third command"  "ccc" "$out"

# cd builtin (state must persist across the shell)
out=$(printf 'cd /tmp; pwd\nexit\n' | ./lopeShell 2>/dev/null)
check "cd changes the shell's cwd" "/tmp" "$out"

# CTRL-X exit (ASCII 0x18 = '\x18')
out=$(printf 'echo before\n\x18\n' | ./lopeShell 2>/dev/null)
check "CTRL-X exits cleanly" "before" "$out"

# Batch mode still works
BATCH=$(mktemp)
printf 'echo line-A; echo line-B\necho line-C\n' > "$BATCH"
out=$(./lopeShell "$BATCH" 2>/dev/null)
check "batch: first concurrent line A" "line-A" "$out"
check "batch: first concurrent line B" "line-B" "$out"
check "batch: second line"             "line-C" "$out"
rm -f "$BATCH"

echo
echo "Topic 3 milestone smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
