#!/usr/bin/env bash
PASS=0; FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then echo "PASS: $desc"; PASS=$((PASS+1))
    else echo "FAIL: $desc"; echo "   expected substring: $expected"; echo "   got:                ${actual:0:200}"; FAIL=$((FAIL+1)); fi
}
if [[ ! -x ./fs ]]; then echo "FAIL: ./fs not found"; exit 1; fi

# mkdir + ls
out=$(printf 'mkdir docs\nmkdir src\nls\nexit\n' | ./fs)
check "mkdir creates directories listed by ls" "src/" "$out"
check "mkdir creates docs"                      "docs/" "$out"

# touch + tree
out=$(printf 'mkdir src\ntouch src/main.c\ntree\nexit\n' | ./fs)
check "tree shows nested file" "main.c" "$out"
check "tree indentation"       "  src/" "$out"

# rmdir -r
out=$(printf 'mkdir docs\ntouch docs/note\nrmdir docs\nrmdir -r docs\nls\nexit\n' | ./fs)
check "rmdir refuses non-empty" "not empty" "$out"

# cd + pwd
out=$(printf 'mkdir a\ncd a\nmkdir b\ncd b\npwd\nexit\n' | ./fs)
check "cd descends and pwd reflects it" "/a/b" "$out"

# find
out=$(printf 'mkdir a\nmkdir a/b\ntouch a/b/target.txt\nfind target.txt\nexit\n' | ./fs)
check "find locates a nested file" "target.txt" "$out"

# mv
out=$(printf 'mkdir a\ntouch a/x\nmkdir b\nmv a/x b/x\nls b\nexit\n' | ./fs)
check "mv moves a file across dirs" "x" "$out"

# cp recursive (deep)
out=$(printf 'mkdir a\ntouch a/x\ncp a b\ntree b\nexit\n' | ./fs)
check "cp deep-copies a directory" "x" "$out"

# info
out=$(printf 'mkdir docs\ntouch docs/r\ninfo docs\nexit\n' | ./fs)
check "info on dir shows children count" "children: 1" "$out"

echo
echo "Topic 6 milestone smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
