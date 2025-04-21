#!/bin/bash
#
# tests.sh — run all of your test‑cases against the blockchain program
#

# create temporary 'runner' directory
mkdir runner
make my_blockchain

cp my_blockchain runner
cd runner

# NOTE: we do NOT create backup.txt here – the first run will see "no backup"
# and print the banner, then save a valid file on quit.

# Path to your compiled program:
PROGRAM=./my_blockchain    # e.g. if you compiled with gcc -o blockchain main.c

# Directory to hold expected and actual outputs:
TEST_DIR=./tests
mkdir -p "$TEST_DIR/actual"
mkdir -p "$TEST_DIR/expected"

# ————————————————————————————————————————————————————————————————
# PASS/FAIL counters
PASS_COUNT=0
FAIL_COUNT=0

# A helper to run a single test:
# $1 = test number
# $2..$n = each line of input to feed to the program
run_test() {
  local id=$1
  shift

  # run the program and capture its stdout+stderr
  printf "%s\n" "$@" | "$PROGRAM" > "$TEST_DIR/actual/out${id}.txt" 2>&1

  echo ""
  echo "---------------------------------------------------------------------------------"
  # compare to expected
  if diff -u "$TEST_DIR/expected/out${id}.txt" "$TEST_DIR/actual/out${id}.txt" >/dev/null; then
    echo "Test $id: PASS"
    ((PASS_COUNT++))
  else
    echo "Test $id: FAIL"
    ((FAIL_COUNT++))
    diff -u "$TEST_DIR/expected/out${id}.txt" "$TEST_DIR/actual/out${id}.txt"
  fi
}

# ————————————————————————————————————————————————————————————————
# 1) add node 1,2,3  then ls, ls -l
cat > "$TEST_DIR/expected/out1.txt" <<EOF
No Backup Found: Starting New Blockchain
[s0]> add node 1
OK
[s1]> add node 2
OK
[s2]> add node 3
OK
[s3]> ls
1
2
3
[s3]> ls -l
node 1: blocks []
node 2: blocks []
node 3: blocks []
[s3]> quit
Backing up blockchain...
EOF
run_test 1 \
  "add node 1" \
  "add node 2" \
  "add node 3" \
  "ls" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 2) add block 1111 to node 2, then ls -l
cat > "$TEST_DIR/expected/out2.txt" <<EOF
[s0]> add block 1111 2
OK
[s1]> ls -l
node 1: blocks []
node 2: blocks [1111]
[s1]> quit
Backing up blockchain...
EOF
run_test 2 \
  "add block 1111 2" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 3) error case: add block to non‑existent node 4, then add & ls -l
cat > "$TEST_DIR/expected/out3.txt" <<EOF
[s0]> add block 2222 4
ERROR: no such node
[s0]> add block 4444 3
OK
[s1]> ls -l
node 1: blocks []
node 2: blocks []
node 3: blocks [4444]
[s1]> quit
Backing up blockchain...
EOF
run_test 3 \
  "add block 2222 4" \
  "add block 4444 3" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 4) remove node 2, then ls -l
cat > "$TEST_DIR/expected/out4.txt" <<EOF
[s0]> rm node 2
OK
[s0]> ls -l
node 1: blocks []
node 3: blocks []
[s0]> quit
Backing up blockchain...
EOF
run_test 4 \
  "rm node 2" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 5) error: remove non‑existent node 4
cat > "$TEST_DIR/expected/out5.txt" <<EOF
[s0]> rm node 4
ERROR: no such node
[s0]> quit
Backing up blockchain...
EOF
run_test 5 \
  "rm node 4" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 6) remove block 4444 from node 1
cat > "$TEST_DIR/expected/out6.txt" <<EOF
[s0]> rm block 4444 1
OK
[s0]> ls -l
node 1: blocks []
[s0]> quit
Backing up blockchain...
EOF
run_test 6 \
  "rm block 4444 1" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 7) add block 3333 to node 3, add node 5, then ls -l
cat > "$TEST_DIR/expected/out7.txt" <<EOF
[s0]> add block 3333 3
OK
[s1]> add node 5
OK
[s2]> ls -l
node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s2]> quit
Backing up blockchain...
EOF
run_test 7 \
  "add block 3333 3" \
  "add node 5" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 8) sync, then ls -l
cat > "$TEST_DIR/expected/out8.txt" <<EOF
[s0]> sync
OK
[s1]> ls -l
node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s1]> quit
Backing up blockchain...
EOF
run_test 8 \
  "sync" \
  "ls -l" \
  "quit"

# ————————————————————————————————————————————————————————————————
# 9) quit, restart, confirm persistence
cat > "$TEST_DIR/expected/out9.txt" <<EOF
[s2]> ls -l
node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s2]> quit
Backing up blockchain...
EOF
run_test 9 \
  "ls -l" \
  "quit"

echo
echo "All tests complete."
echo "Summary: $PASS_COUNT passed, $FAIL_COUNT failed."

# Cleanup
cd ..
rm -r runner
rm dsa.o my_blockchain.o my_blockchain

# Exit code: 0 if all passed, 1 otherwise
if [ "$FAIL_COUNT" -eq 0 ]; then
  echo "🎉 ALL TESTS PASSED"
  exit 0
else
  echo "❌ SOME TESTS FAILED"
  exit 1
fi