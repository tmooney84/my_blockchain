# echo "starting tests..."

# echo ""

# #1)
# #add node 1
# #add node 2
# #add node 3
# #ls
# #ls -l and compare

# #2)
# #add block 1111 2
# #ls -l

# #3)
# #add block 2222 4 >>> should have error
# #add block 4444 *
# #ls -l

# #4)
# #rm node 2
# #ls -l

# #5)
# #rm node 4 >>> should have an error

# #6)
# #rm block 4444 1
# #ls -l

# #7)
# #add block 3333 3
# #add node 5
# #ls -l

# #8)
# #sync
# #ls -l

# #9)
# #quit
# #restart and make sure they all come back up
# #ls -l



#!/bin/bash
#
# tests.sh — run all of your test‑cases against the blockchain program
#

#create temporary 'runner' directory
mkdir runner
make my_blockchain

cp my_blockchain runner

cd runner

# Path to your compiled program:
PROGRAM=./my_blockchain    # e.g. if you compiled with gcc -o blockchain main.c

# Directory to hold expected and actual outputs:
TEST_DIR=./tests
mkdir -p "$TEST_DIR/actual"
mkdir -p "$TEST_DIR/expected"

# A helper to run a single test:
# $1 = test number
# $2 = here-doc commands
run_test() {
  local id=$1
  local here_doc="$2"

  # run the program and capture its stdout+stderr
  printf "%s\n" "$here_doc" | "$PROGRAM" > "$TEST_DIR/actual/out${id}.txt" 2>&1

  # compare to expected
  if diff -u "$TEST_DIR/expected/out${id}.txt" "$TEST_DIR/actual/out${id}.txt" >/dev/null; then
    echo "Test $id: PASS"
  else
    echo "Test $id: FAIL"
    diff -u "$TEST_DIR/expected/out${id}.txt" "$TEST_DIR/actual/out${id}.txt"
  fi
}

# ————————————————————————————————————————————————————————————————
# 1) add node 1,2,3  then ls, ls -l
cat > "$TEST_DIR/expected/out1.txt" <<EOF
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

run_test 1 $'add node 1\nadd node 2\nadd node 3\nls\nls -l\nquit'

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

run_test 2 $'add block 1111 2\nls -l\nquit'

# ————————————————————————————————————————————————————————————————
# 3) error case: add block to non‑existent node 4, then add block, then ls -l
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

run_test 3 $'add block 2222 4\nadd block 4444 3\nls -l\nquit'

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

run_test 4 $'rm node 2\nls -l\nquit'

# ————————————————————————————————————————————————————————————————
# 5) error: remove non‑existent node 4
cat > "$TEST_DIR/expected/out5.txt" <<EOF
[s0]> rm node 4
ERROR: no such node
[s0]> quit
Backing up blockchain...
EOF

run_test 5 $'rm node 4\nquit'

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

run_test 6 $'rm block 4444 1\nls -l\nquit'

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

run_test 7 $'add block 3333 3\nadd node 5\nls -l\nquit'

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

run_test 8 $'sync\nls -l\nquit'

# ————————————————————————————————————————————————————————————————
# 9) quit, restart, confirm persistence
#    here we rely on the fact that the backup.txt was written by the previous
#    quit.  We just run “ls -l” immediately after start.
cat > "$TEST_DIR/expected/out9.txt" <<EOF
[s2]> ls -l
node 1: blocks []
node 3: blocks [3333]
node 5: blocks []
[s2]> quit
Backing up blockchain...
EOF

# First quit wrote backup; now we launch a fresh instance w/o feeding add/remove:
run_test 9 $'ls -l\nquit'

echo
echo "All tests complete."

#clean up testing files
cd ..
rm -r runner
rm dsa.o my_blockchain.o my_blockchain

