# Compiler
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g

# Target executable name
TARGET  := my_blockchain

# Source and object files
SRC     := my_blockchain.c dsa.c
OBJ     := $(SRC:.c=.o)

# Headers
HEADERS := dsa.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)
	rm -rf tests/actual tests/expected runner backup.txt

re: fclean all

# ————————————————————————————————————————————————————————————————
.PHONY: prepare-tests
prepare-tests:
	@mkdir -p tests/expected

	@printf '%s\n' \
	  "No Backup Found: Starting New Blockchain" \
	  "[s0]> OK" "[s1]> OK" "[s2]> OK" "[s3]> 1" "2" "3" \
	  "[s3]> node 1: blocks []" "node 2: blocks []" "node 3: blocks []" \
	  "[s3]> Backing up blockchain..." > tests/expected/out1.txt

	@printf '%s\n' \
	  "[s0]> OK" "[s1]> node 1: blocks []" "node 2: blocks [1111]" \
	  "[s1]> Backing up blockchain..." > tests/expected/out2.txt

	@printf '%s\n' \
	  "[s0]> ERROR: no such node" "[s0]> OK" \
	  "[s1]> node 1: blocks []" "node 2: blocks []" "node 3: blocks [4444]" \
	  "[s1]> Backing up blockchain..." > tests/expected/out3.txt

	@printf '%s\n' \
	  "[s0]> OK" "[s0]> node 1: blocks []" "node 3: blocks []" \
	  "[s0]> Backing up blockchain..." > tests/expected/out4.txt

	@printf '%s\n' \
	  "[s0]> ERROR: no such node" \
	  "[s0]> Backing up blockchain..." > tests/expected/out5.txt

	@printf '%s\n' \
	  "[s0]> OK" "[s0]> node 1: blocks []" \
	  "[s0]> Backing up blockchain..." > tests/expected/out6.txt

	@printf '%s\n' \
	  "[s0]> OK" "[s1]> OK" \
	  "[s2]> node 1: blocks []" "node 3: blocks [3333]" "node 5: blocks []" \
	  "[s2]> Backing up blockchain..." > tests/expected/out7.txt

	@printf '%s\n' \
	  "[s0]> OK" "[s1]> node 1: blocks []" \
	  "node 3: blocks [3333]" "node 5: blocks []" \
	  "[s1]> Backing up blockchain..." > tests/expected/out8.txt

	@printf '%s\n' \
	  "[s3]> node 1: blocks []" "node 3: blocks [3333]" "node 5: blocks []" \
	  "[s3]> Backing up blockchain..." > tests/expected/out9.txt

# ————————————————————————————————————————————————————————————————
.PHONY: test
test: prepare-tests all
	@rm -f backup.txt
	@mkdir -p tests/actual
	@echo "Running 9 tests…"
	@PASS=0; FAIL=0; \
	for n in 1 2 3 4 5 6 7 8 9; do \
	  printf "Test %d: " $$n; \
	  case $$n in \
	    1) \
	      ./$(TARGET) > tests/actual/out1.txt <<-EOF
	      add node 1
	      add node 2
	      add node 3
	      ls
	      ls -l
	      quit
			EOF
	      ;; \
	    2) \
	      ./$(TARGET) > tests/actual/out2.txt <<-EOF
	      add block 1111 2
	      ls -l
	      quit
			EOF
	      ;; \
	    3) \
	      ./$(TARGET) > tests/actual/out3.txt <<-EOF
	      add block 2222 4
	      add block 4444 3
	      ls -l
	      quit
			EOF
	      ;; \
	    4) \
	      ./$(TARGET) > tests/actual/out4.txt <<-EOF
	      rm node 2
	      ls -l
	      quit
			EOF
	      ;; \
	    5) \
	      ./$(TARGET) > tests/actual/out5.txt <<-EOF
	      rm node 4
	      quit
			EOF
	      ;; \
	    6) \
	      ./$(TARGET) > tests/actual/out6.txt <<-EOF
	      rm block 4444 1
	      ls -l
	      quit
			EOF
	      ;; \
	    7) \
	      ./$(TARGET) > tests/actual/out7.txt <<-EOF
	      add block 3333 3
	      add node 5
	      ls -l
	      quit
			EOF
	      ;; \
	    8) \
	      ./$(TARGET) > tests/actual/out8.txt <<-EOF
	      sync
	      ls -l
	      quit
			EOF
	      ;; \
	    9) \
	      ./$(TARGET) > tests/actual/out9.txt <<-EOF
	      ls -l
	      quit
			EOF
	      ;; \
	  esac; \
	  if diff -u tests/expected/out$$n.txt tests/actual/out$$n.txt > /dev/null; then \
	    echo "PASS"; PASS=$$((PASS+1)); \
	  else \
	    echo "FAIL"; FAIL=$$((FAIL+1)); \
	    diff -u tests/expected/out$$n.txt tests/actual/out$$n.txt; \
	  fi; \
	done; \
	echo; echo "$$PASS of 9 tests passed, $$FAIL failed."; \
	test $$FAIL -eq 0
