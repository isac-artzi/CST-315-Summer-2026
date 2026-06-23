#!/usr/bin/env bash
PASS=0; FAIL=0
check() {
    local desc="$1" expected="$2" actual="$3"
    if [[ "$actual" == *"$expected"* ]]; then echo "PASS: $desc"; PASS=$((PASS+1))
    else echo "FAIL: $desc"; echo "   expected substring: $expected"; FAIL=$((FAIL+1)); fi
}
if [[ ! -x ./scheduler ]]; then echo "FAIL: ./scheduler not found"; exit 1; fi

out=$(./scheduler 2 2>/dev/null)
check "default workload reports algorithm header" "Round Robin, quantum=2" "$out"
check "default workload completes P1"             "P1  arrival=0"          "$out"
check "default workload completes P4"             "P4  arrival=5"          "$out"
check "default workload reports avg waiting"      "avg waiting:"           "$out"
check "default workload reports throughput"       "throughput:"            "$out"
check "default workload reports context switches" "context switches:"      "$out"

echo "9 0 3 5" > /tmp/scheduler_w1.txt
out=$(./scheduler 2 /tmp/scheduler_w1.txt 2>/dev/null)
check "single process completes" "P9  arrival=0  burst=3  completion=3" "$out"
rm -f /tmp/scheduler_w1.txt

echo
echo "Topic 5 starter smoke test: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
