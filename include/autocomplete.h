#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"

//This signature definition is here only for now are temporary only
prefix_t* get_children(char* s);

// Prints the batch output, including n words if b.
void print_children(int b, int n, char* s);

#endif //AUTOCOMPLETE_H
