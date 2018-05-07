#ifndef PREFIX_H
#define PREFIX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct prefix {
    char* prefix;
    char** completions;
    int nComps;
} prefix_t;

prefix_t* prefix_new(char* p, char** c, int n);

int prefix_init(prefix_t* pref, char* p, char** c, int n);

void prefix_free(prefix_t* pref);

#endif //PREFIX_H
