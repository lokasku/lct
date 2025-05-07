#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "ast.h"

typedef struct {
    size_t index;
    const char* input;
} Parser;

Term* parse(char* input);

#endif
