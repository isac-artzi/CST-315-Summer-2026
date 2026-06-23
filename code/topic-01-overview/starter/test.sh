#!/usr/bin/env bash
# Smoke test for Topic 1 starter (hello).
# Exits non-zero on any failure.

PASS=0
FAIL=0

check() {
    local desc="$1"
    local expected="$2"
    local actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then
        echo "PASS: $desc"
        PASS=$((PASS+1))
    else
        echo "FAIL: $desc"
        echo "   expected substring: $expected"
        echo "   got:                $actual"
        FAIL=$((FAIL+1))
    fi
}

if [[ ! -x ./hello ]]; then
    echo "FAIL: ./hello not found or not executable. Did you run 'make'?"
    exit 1
fi

out=$(unset USER_OVERRIDE; ./hello)
check "default greeting starts with 'Hello,'" "Hello," "$out"
check "default greeting mentions CST-315"      "CST-315"   "$out"

out=$(./hello Isac)
check "arg 'Isac' produces 'Hello, Isac!'" "Hello, Isac!" "$out"

out=$(USER_OVERRIDE=alice ./hello)
check "USER_OVERRIDE env var overrides USER" "Hello, alice!" "$out"

out=$(USER_OVERRIDE=alice ./hello "")
check "empty arg does not override USER_OVERRIDE" "Hello, alice!" "$out"

echo
echo "Topic 1 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
