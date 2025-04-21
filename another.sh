#!/usr/bin/env bash
set -euo pipefail

# build fresh
make my_blockchain

# create a clean runner dir
rm -rf runner
mkdir runner
cp my_blockchain runner/
cd runner

PROGRAM=./my_blockchain
TEST_DIR=./tests
mkdir -p "$TEST_DIR/expected" "$TEST_DIR/actual"

PASS=0
FAIL=0

# helper to run one test
# $1 = test number
# here‑doc provides the stdin, output goes to actual/outN.txt
run_case() {
  local id=$1
  shift

  # run the program, feed the here-doc as stdin
  "$PROGRAM" > "$TEST_DIR/actual/out${id}.txt" 2>&1 <<EOF
$(cat)
EOF

  # compare
  if diff -u "$TEST_DIR/expected/out${id}.txt" "$TEST_DIR/actual/out${id}.txt" >/dev/null; then
    echo "Test $id: PASS"
    ((PASS++))
  else
    echo "Test $id: FAIL"
    ((FAIL++))
    diff -u "$TEST_DIR/expected/out${id}.txt" "$TEST_DIR/actual/out${id}.txt"
  fi
}

#
# 1) No backup banner, then add nodes, ls, ls -l, quit
#
cat > "$TEST_DIR/expected/out1.txt" <<'EOF'
No Backup Found: Starting New Blockchain
[s0]> OK
[s1]> OK
[s2]> OK
[s3]> 1
2
3
[s3]> node 1: blocks []
node 2: blocks []
node 3: blocks []
[s3]> Backing up blockchain...
EOF

run_case 1 <<'EOF'
add node 1
add node 2
add node 3
ls
ls -l
quit
EOF

#
# 2) add block 1111 to node 2, ls -l, quit
#
cat > "$TEST_DIR/expected/out2.txt" <<'EOF'
[s0]> OK
[s1]> node 1: blocks []
node 2: blocks [1111]
[s1]> Backing up blockchain...
EOF

run_case 2 <<'EOF'
add block 1111 2
ls -l
quit
EOF

#
# 3) error add to node 4, then valid add, ls -l, quit
#
cat > "$TEST_DIR/expected/out3.txt" <<'EOF'
[s0]> ERROR: no such node
[s0]> OK
[s1]> node 1: blocks []
node 2: blocks []
node 3: blocks [4444]
[s1]> Backing up blockchain...
EOF

run_case 3 <<'EOF'
add block 2222 4
add block 4444 3
ls -l
quit
EOF

#
# 4) rm node 2, ls -l, quit
#
cat > "$TEST_DIR/expected/out4.txt" <<'EOF'
[s0]> OK
[s0]> node 1: blocks []
node 3: blocks []
[s0]> Backing up blockchain...
EOF

run_case 4 <<'EOF'
rm node 2
ls -l
quit
EOF

#
# 5) rm node 4 (error), quit
#
cat > "$TEST_DIR/expected/out5.txt" <<'EOF'
[s0]> ERROR: no such node
[s0]> Backing up blockchain...
EOF

run_case 5 <<'EOF'
rm node 4
quit
EOF

#
# 6) rm block 4444 from node 1, ls -l, quit
#
cat > "$TEST_DIR/expected/out6.txt" <<'EOF'
[s0]> OK
[s0]> node 1: blocks []
[s0]> Backing up blockchain...
EOF

run_case 6 <<'EOF'
rm block 4444 1
ls -l
quit
EOF

#
# 7) add block + add node + ls -l + quit
#
cat > "$TEST_DIR/expected/out7.txt" <<'EOF'
[s0]> OK
[s1]> OK
[s2]> node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s2]> Backing up blockchain...
EOF

run_case 7 <<'EOF'
add block 3333 3
add node 5
ls -l
quit
EOF

#
# 8) sync, ls -l, quit
#
cat > "$TEST_DIR/expected/out8.txt" <<'EOF'
[s0]> OK
[s1]> node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s1]> Backing up blockchain...
EOF

run_case 8 <<'EOF'
sync
ls -l
quit
EOF

#
# 9) restart from backup, ls -l, quit
#
cat > "$TEST_DIR/expected/out9.txt" <<'EOF'
[s2]> node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s2]> Backing up blockchain...
EOF

run_case 9 <<'EOF'
ls -l
quit
EOF

echo
echo "All done: $PASS passed, $FAIL failed."

if [ "$FAIL" -eq 0 ]; then
  echo "🎉 ALL TESTS PASSED"
  exit 0
else
  echo "❌ SOME TESTS FAILED"
  exit 1
fi