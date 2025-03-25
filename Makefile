# Makefile for hw1shell

CC = gcc
CFLAGS = -Wall -Werror

# List all your source files here
SRCS = linuxshell.c
OBJS = $(SRCS:.c=.o)

# The default target
all: hw1shell

hw1shell: $(OBJS)
	$(CC) $(CFLAGS) -o hw1shell $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f hw1shell $(OBJS)
