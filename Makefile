CFLAGS := -Wall -Wextra
CFLAGS += -O2
CFLAGS += -pedantic

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all
all: lam2bru.o
