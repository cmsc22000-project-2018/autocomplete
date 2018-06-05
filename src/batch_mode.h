#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"
#include "dictionary.h"

#include "../api/include/trie.h"
#define DEFAULT_DICTIONARY_FILE "default"

// Given a prefix and a dictionary, returns a given number of valid children
char** get_n_children_in_dict(char* s, dict_t* dict, int n);

// Given a prefix and a dictionary, returns a count of all valid children
int num_children_in_dict(char* s, dict_t* d);

// Prints the batch output, including n words if b.
void print_children(int b, int n, char* s, dict_t* dict);

#endif //AUTOCOMPLETE_H
