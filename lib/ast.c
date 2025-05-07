#include <stdlib.h>
#include "ast.h"

Term*
make_term(int type)
{
    Term* t = malloc(sizeof(Term));
    if (!t) return NULL;
    t->type = type;
    return t;
}

void
free_term(Term* t)
{
    if (!t) return;

    switch (t->type) {
        case ABS:
            free_term(t->abs.body);
            break;
        case APP:
            free_term(t->app.lhs);
            free_term(t->app.rhs);
            break;
        case VAR:
            break;
    }

    free(t);
}