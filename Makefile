# SPDX-License-Identifier: ISC
# SPDX-FileCopyrightText: Copyright 2025 lokasku <lukasku@proton.me>


CFLAGS := -Wall -Wextra
CFLAGS += -O2
CFLAGS += -pedantic
CFLAGS += -Iparser

LIB_SRCS := lib/ast.c lib/parser.c lib/semantic.c
LIB_OBJS := $(LIB_SRCS:.c=.o)

PROGS = lam2bru

all: $(PROGS)

lam2bru: lam2bru.o $(LIB_OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJS) lam2bru.o $(PROGS)

.PHONY: all clean
