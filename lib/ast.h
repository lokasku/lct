#ifndef AST_H
#define AST_H

typedef struct Term {
    enum { ABS, APP, VAR } type;
    union {
        struct { char var; struct Term* body; } abs;
        struct { struct Term *lhs, *rhs; } app;
        char var;
    };
} Term;

Term* make_term(int type);
void free_term(Term* t);

#endif // AST_H
