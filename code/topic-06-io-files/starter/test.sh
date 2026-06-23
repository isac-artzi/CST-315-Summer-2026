#!/usr/bin/env bash
PASS=0; FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then echo "PASS: $desc"; PASS=$((PASS+1))
    else echo "FAIL: $desc"; FAIL=$((FAIL+1)); fi
}
if [[ ! -x ./fs ]]; then echo "FAIL: ./fs not found"; exit 1; fi

out=$(printf 'mkdir docs\nmkdir src\nls\nexit\n' | ./fs)
check "mkdir + ls"               "src/"      "$out"

out=$(printf 'mkdir src\ntouch src/main.c\ntree\nexit\n' | ./fs)
check "tree shows nested file"    "main.c"   "$out"

out=$(printf 'mkdir docs\ntouch docs/note\nrmdir docs\nrmdir -r docs\nls\nexit\n' | ./fs)
check "rmdir refuses non-empty"   "not empty" "$out"

out=$(printf 'mkdir a\ncd a\nmkdir b\ncd b\npwd\nexit\n' | ./fs)
check "cd + pwd"                  "/a/b"     "$out"

echo
echo "Topic 6 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
