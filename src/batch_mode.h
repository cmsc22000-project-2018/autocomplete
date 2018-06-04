#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"

// Given a prefix and a dictionary file, returns a given number of valid children
char** get_n_children_in_dict(char* s, char* dict_file, int n);

// Given a prefix and a dictionary file, returns a count of all valid children
int num_children_in_dict(char* s, char* dict_file);

// Prints the batch output, including n words if b.
void print_children(int b, int n, char* s, char* dict_file);

#endif //AUTOCOMPLETE_H
