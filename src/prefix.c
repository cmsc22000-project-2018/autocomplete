#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"

static const int MAXLENGTH = 64;
static const int MAXPREFS = 2048;

prefix_t* prefix_new(char* p, char** c, int n)
{
    prefix_t* pref = malloc(sizeof(prefix_t));
    assert(pref != NULL);

    pref->prefix = malloc(MAXLENGTH);
    strncpy(pref->prefix, p, (MAXLENGTH > strlen(p) ? strlen(p) : MAXLENGTH));

    pref->nComps = n;

    pref->completions = malloc(MAXLENGTH * MAXPREFS);

    for(int i = 0; i < n; i++) {
        pref->completions[i] = malloc(MAXLENGTH);
        strncpy(pref->completions[i], c[i], (MAXLENGTH > strlen(c[i]) ? strlen(c[i]) : MAXLENGTH));
    }

    return pref;
}

int prefix_init(prefix_t* pref, char* p, char** c, int n)
{
    size_t prefplength = MAXLENGTH;
    size_t plength = strlen(p);
    if (plength < MAXLENGTH) {
        prefplength = plength;
    }
    strncpy(pref->prefix, p, prefplength);

    pref->nComps = n;

    for(int i = 0; i < n; i++) {
        strncpy(pref->completions[i], c[i], MAXLENGTH);
    }

    return 0;
}

void prefix_free(prefix_t* pref)
{
    for(int i = 0; i < pref->nComps; i++) {
        free(pref->completions[i]);
    }
    free(pref->completions);
    free(pref->prefix);
    free(pref);
}
