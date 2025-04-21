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