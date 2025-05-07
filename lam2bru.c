/*
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: Copyright 2025 lokasku <lukasku@proton.me>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USAGE \
    " lam2bru [-?h] [string]\n\
Convert a lambda calculus string in standard notation to De Bruijn notation. \n\n\
  string   The lambda calculus string to convert. (e.g. '\\x.\\y.x')\n\
  -? -h    Help\n\n\
EXAMPLES\n\
    $ Y=\"\\f.(\\x.f (xx)) (\\x. f (xx))\"\n\
    $ lam2bru $Y\n\
    \\[\\1 [0 0]] [\\1 [0 0]]\n\
    $ lam2bru \"(\\x.x x) (\\y.y y)\"\n\
    [\\0 0] [\\0 0]\n"

#define STACK_CAPACITY 32

#define isalpha(c) (((c) & ~0x20) >= 'A' && ((c) & ~0x20) <= 'Z')

enum {
    UNDEF_ERR,
    UNEXP_ERR,
};

typedef struct {
    uint_fast16_t pos;
    const char *stream;
} cursor;

typedef struct {
    uint8_t sp;
    char pile[STACK_CAPACITY];
} stack;

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
                fputs("Usage: lam2bru", stderr);
                fputs(USAGE, stderr);
                exit(1);
        }
}

int
lookup(stack* env,
       char var)
{
    int i = env->sp;
    while (i >= 0) {
        if (env->pile[i] == var)
            return env->sp - i - 1;
        i--;
    }
    exit(UNDEF_ERR);
}

void
show(stack* env,
     stack* bds,
     cursor* input)
{
    int req_space = 0;

    while (input->stream[input->pos] != '\0') {
        char curr = input->stream[input->pos];

        switch (curr) {
            case ' ':
                input->pos++;
                break;

            case '\\':
                if (req_space) fputc(' ', stdout);
                fputc('\\', stdout);
                input->pos++;
                env->pile[env->sp++] = input->stream[input->pos++];
                bds->pile[bds->sp]++;
                if (input->stream[input->pos++] != '.') exit(UNEXP_ERR);
                req_space = 0;
                break;

            case '(':
                if (req_space) fputc(' ', stdout);
                fputc('[', stdout);
                bds->sp++;
                input->pos++;
                req_space = 0;
                break;

            case ')':
                fputc(']', stdout);
                env->sp -= bds->pile[bds->sp];
                input->pos++;
                bds->sp--;
                req_space = 1;
                break;

            default:
                if (isalpha(curr)) {
                    if (req_space) fputc(' ', stdout);
                    int idx = lookup(env, curr);
                    printf("%d", idx);
                    input->pos++;
                    req_space = 1;
                    break;
                }
                exit(UNEXP_ERR);
        }
    }

    fputc('\n', stdout);
}

int
main(int argc, char *argv[])
{
    help(argc, argv);

    char* expr = argc > 1 ? argv[1] : "\\x.\\y.\\z.xy(xz)";

    cursor input = {0, expr};
    stack env = {0};
    stack bds = {0};

    show(&env, &bds, &input);

    return 0;
}
