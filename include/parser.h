#ifndef PARSER_H
#define PARSER_H

/* Again, parser is from cs152coin except testing functions */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void init_parser();

void next_token();

int read_string(char** s);

#endif //PARSER_H
