#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"

// Given a prefix and a dictionary file, returns all valid children
char** get_children_in_dict(char* s, char* dict_file);

// Prints the batch output, including n words if b.
void print_children(int b, int n, char* s, char* dict_file);

#endif //AUTOCOMPLETE_H
