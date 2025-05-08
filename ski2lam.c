/*
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: Copyright 2025 lokasku <lukasku@proton.me>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE \
    " ski2lam [-?h] [string]\n\
Convert an SKI expression to standard lambda calculus.\n\n\
  string   The SKI expression to convert. (e.g. 'SKKS')\n\
  -? -h    Help\n\n"

static const char* s = "(\\x.\\y.\\z.xz(yz))";
static const char* k = "(\\x.\\y.x)";
static const char* i = "(\\x.x)";

void
help(int argc,
     char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "?h")) != -1)
        switch (c)
        {
            case '?':
            case 'h':
                puts("Usage:");
                fputs(USAGE, stdout);
                exit(0);
            default:
                fputs("Usage: ski2lam", stderr);
                fputs(USAGE, stderr);
                exit(1);
        }
}

int
main(int argc,
     char *argv[])
{
    help(argc, argv);

    const char* input = argc ? argv[1] : "ski";

    int idx = 0;
    char c;

    while ((c = input[idx]) != '\0') {
        switch (c)
        {
            case 's':
            case 'S':
                fputs(s, stdout);
                idx++;
                break;
            case 'k':
            case 'K':
                fputs(k, stdout);
                idx++;
                break;
            case 'i':
            case 'I':
                fputs(i, stdout);
                idx++;
                break;
            case '(':
                fputs("(", stdout);
                idx++;
                break;
            case ')':
                fputs(")", stdout);
                idx++;
                break;
            default:
                fputs("Only i, I, s, S, k, K, ( and ) allowed.\n", stderr);
                exit(1);
        }
        fputs(" ", stdout);
    }
    fputs("\n", stdout);
}
