# SPDX-License-Identifier: ISC
# SPDX-FileCopyrightText: Copyright 2025 lokasku <lukasku@proton.me>

CFLAGS := -Wall -Wextra
CFLAGS += -O2
CFLAGS += -pedantic

PROGS = lam2bru ski2lam breduc

all: $(PROGS)

$(PROGS): %: %.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGS)

.PHONY: all clean
