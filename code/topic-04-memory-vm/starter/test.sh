#!/usr/bin/env bash
PASS=0; FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then echo "PASS: $desc"; PASS=$((PASS+1))
    else echo "FAIL: $desc"; echo "   expected substring: $expected"; echo "   got:                $actual"; FAIL=$((FAIL+1)); fi
}
if [[ ! -x ./pager ]]; then echo "FAIL: ./pager not found"; exit 1; fi

out=$(echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager 4 2>/dev/null)
check "LRU 4-frame: 8 faults"                "faults: 8"         "$out"
check "LRU 4-frame: fault rate 66.7%"        "fault rate: 66.7%" "$out"

out=$(echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager 3 2>/dev/null)
check "LRU 3-frame: any output"               "fault rate"       "$out"

out=$(echo "7 7 7 7 7" | ./pager 4 2>/dev/null)
check "single repeated: exactly 1 fault"      "faults: 1"        "$out"
check "single repeated: 4 hits"               "hits: 4"          "$out"

echo
echo "Topic 4 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
