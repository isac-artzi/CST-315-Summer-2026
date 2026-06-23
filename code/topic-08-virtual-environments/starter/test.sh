#!/usr/bin/env bash
PASS=0; FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then echo "PASS: $desc"; PASS=$((PASS+1))
    else echo "FAIL: $desc"; FAIL=$((FAIL+1)); fi
}
if [[ ! -x ./lopeShell ]]; then echo "FAIL: ./lopeShell not found"; exit 1; fi

out=$(printf 'echo hello\nexit\n' | ./lopeShell 2>/dev/null)
check "single command runs"      "hello"  "$out"
out=$(printf 'echo aaa; echo bbb\nexit\n' | ./lopeShell 2>/dev/null)
check "multi-command runs both"  "aaa"   "$out"
check "multi-command runs both 2" "bbb"  "$out"
out=$(printf 'cd /tmp; pwd\nexit\n' | ./lopeShell 2>/dev/null)
check "cd works"                 "/tmp"   "$out"
out=$(printf 'echo dollar$$\nexit\n' | ./lopeShell 2>&1)
check "sanitization still active" "rejected" "$out"

if [[ -f Dockerfile ]]; then echo "PASS: Dockerfile present"; PASS=$((PASS+1))
else echo "FAIL: Dockerfile missing"; FAIL=$((FAIL+1)); fi

echo
echo "Topic 8 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
