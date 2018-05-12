#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"

// Prints the batch output, including n words if b.
void print_children(int b, int n, char* s, FILE* dictionary);

#endif //AUTOCOMPLETE_H
