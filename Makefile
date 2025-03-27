# Compiler
CC = gcc

# Compiler flags (without debug info)
# CFLAGS = -Wall -Wextra -Werror -g #-std=c99

# Compiler flags (with debug info)
CFLAGS = -Wall -Wextra -Werror -g #-std=c99

# Target executable name
TARGET = my_readline

# Source files (add all your .c files here)
SRC = my_readline.c  

# Object files (derived automatically from SRC)
OBJ = $(SRC:.c=.o)

# Header files (optional but useful for dependency management)
HEADERS =  #utils.h 

# Build target
all: $(TARGET)

# Linking the object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile each .c file into an object file
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

re: fclean all