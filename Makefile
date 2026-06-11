# Name of executable
EXEC = assembler

# Compilator
CC = gcc

# Options of compilation 
CFLAGS = -Wall -ansi -pedantic

# All of Source files
SRCS = main.c pre_assembler.c first_pass.c second_pass.c symbol_table.c utils.c instruction_set.c

# All of the object files
OBJS = $(SRCS:.c=.o)

# Main rule
all: $(EXEC)

# Link of object files to create executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation of each file .c and .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning of compiled files
clean:
	rm -f $(EXEC) $(OBJS) *.am *.ob *.ext *.ent

# Keeps make from using 'clean' file already existing
.PHONY: all clean
