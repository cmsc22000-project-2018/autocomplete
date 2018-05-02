/* Again, parser is from cs152coin except testing functions */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void init_parser();

void next_token();

int read_string(char** s);

//The following are temporary only
char** get_children(char* s, int* n);

void print_children(void* s, const void* b, const void* n);
