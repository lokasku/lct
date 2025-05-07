#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "semantic.h"

static void skip(Parser*);
static int is_alpha(char);
static Term* parse_var(Parser*);
static Term* parse_atom(Parser*, int);
static Term* parse_app(Parser*, int);
static Term* parse_expr(Parser*, int);
char* sanitize(const char*);

/************************@;
§ expr ::= \ var . expr
!       | app
|
! app ::= app atom
!       | atom
!
| atom ::= var | ( expr )
!
§ var ::= [a-zA-Z]
#+**********************°*/

void
skip(Parser* p)
{
    while (p->input[p->index] == ' ')
        p->index++;
}

int
is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

Term*
parse_var(Parser* p)
{
    skip(p);

    if (!is_alpha(p->input[p->index]))
        return NULL;

    Term* t = make_term(VAR);
    if (!t) return NULL;
    t->var = p->input[p->index++];

    return t;
}

Term*
parse_atom(Parser* p,
           int depth)
{
    skip(p);

    int index_copy = p->index;

    Term* t = parse_var(p);
    if (t) return t;

    p->index = index_copy;

    if (p->input[p->index] != '(')
        return NULL;

    p->index++;

    t = parse_expr(p, depth + 1);
    if (!t) return NULL;

    skip(p);

    if (p->input[p->index] != ')')
        return NULL;

    p->index++;

    return t;
}

Term*
parse_app(Parser* p,
          int depth)
{
    skip(p);

    Term* t = parse_atom(p, depth);
    if (!t) return NULL;

    while (1) {
        skip(p);

        if (p->input[p->index] == '\0'
            || (!is_alpha(p->input[p->index])
                && p->input[p->index] != '('))
            break;

        Term* rhs = parse_atom(p, depth);
        if (!rhs) break;

        Term* _t = make_term(APP);
        _t->app.lhs = t;
        _t->app.rhs = rhs;
        t = _t;
    }

    return t;
}

Term*
parse_expr(Parser* p,
           int depth)
{
    if (depth >= 1000) {
        fprintf(stderr, "Maximum recursion depth exceeded\n");
        return NULL;
    }

    skip(p);

    if (p->input[p->index] == '\\') {
        p->index++;

        skip(p);

        Term* var = parse_var(p);
        if (!var) return NULL;
        char var_name = var->var;
        free(var);

        skip(p);

        if (p->input[p->index] != '.') return NULL;
        p->index++;

        Term* body = parse_expr(p, depth + 1);
        if (!body) return NULL;

        Term* abs = make_term(ABS);
        if (!abs) {
            free_term(body);
            return NULL;
        }
        abs->abs.var = var_name;
        abs->abs.body = body;
        return abs;
    }

    return parse_app(p, depth);
}

char*
sanitize(const char* str)
{
    if (!str) return NULL;

    size_t len = strlen(str);
    char* clean = malloc(len + 1);
    if (!clean) return NULL;

    char* dst = clean;
    while (*str) {
        if (*str >= 32 && *str != 127) {
            *dst++ = *str;
        }
        str++;
    }
    *dst = '\0';

    return clean;
}

Term*
parse(char* input)
{
    Parser p = {
        .index = 0,
        .input = sanitize(input)
    };

    return parse_expr(&p, 0);
}
