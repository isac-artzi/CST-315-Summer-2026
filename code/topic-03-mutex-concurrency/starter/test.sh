#!/usr/bin/env bash
# Smoke test for Topic 3 starter (lopeShell v2).
# Will FAIL until you complete the three TODOs.

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
check "single command still works" "hello" "$out"

out=$(printf 'echo aaa; echo bbb; echo ccc\nexit\n' | ./lopeShell 2>/dev/null)
check "multi: first command"  "aaa" "$out"
check "multi: second command" "bbb" "$out"
check "multi: third command"  "ccc" "$out"

out=$(printf 'cd /tmp; pwd\nexit\n' | ./lopeShell 2>/dev/null)
check "cd changes the shell's cwd" "/tmp" "$out"

out=$(printf 'echo before\n\x18\n' | ./lopeShell 2>/dev/null)
check "CTRL-X exits cleanly" "before" "$out"

BATCH=$(mktemp)
printf 'echo line-A; echo line-B\necho line-C\n' > "$BATCH"
out=$(./lopeShell "$BATCH" 2>/dev/null)
check "batch: first concurrent line A" "line-A" "$out"
check "batch: first concurrent line B" "line-B" "$out"
check "batch: second line"             "line-C" "$out"
rm -f "$BATCH"

echo
echo "Topic 3 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
