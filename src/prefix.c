#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"

static const unsigned int MAXLENGTH = 64;
static const unsigned int MAXPREFS = 2048;

prefix_t* prefix_new(char* p, char** c, int n)
{
	printf("entering prefix_new\n");
    prefix_t* pref = malloc(sizeof(prefix_t));
	printf("prefix_new: about to assert that malloc isnt null\n");
    assert(pref != NULL);

    pref->prefix = malloc(MAXLENGTH > (strlen(p) + 1) ? (strlen(p) + 1) : MAXLENGTH);
    strncpy(pref->prefix, p, ((MAXLENGTH > (strlen(p) + 1)) ? (strlen(p) + 1) : MAXLENGTH));
	printf("prefix_new: set pref->prefix\n");
    pref->nComps = n;
	printf("prefix_new: set pref->nComps\n");
    pref->completions = malloc(MAXLENGTH * MAXPREFS);
	printf("prefix_new: set pref->completions\n");
    if (c != NULL) {
	printf("prefix_new: about to enter for loop to fill array, n=%d\n", n);
        for(int i = 0; i < n; i++) {
		printf("prefix_new: loop #%d\n", i);
		printf("prefix_new: word for this loop is %s\n", c[i]);
		unsigned int clen = (strlen(c[i]) + 1);
		printf("prefix_new: length of word(%s) to be malloc/copied=%d\n", c[i], clen);
            pref->completions[i] = malloc((MAXLENGTH > clen) ? clen : MAXLENGTH);
		printf("prefix_new: in loop#%d, malloced pref->completions[%d]\n", i, i);
            strncpy(pref->completions[i], c[i], ((MAXLENGTH > clen) ? clen : MAXLENGTH));
		printf("prefix_new: in loop#%d, strncpy'd %s into pref-completions[%d]\n", i, c[i], i);
        }
    }
	printf("prefix_new: about to return pref\n");
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
        strncpy(pref->completions[i], c[i], (MAXLENGTH > (strlen(c[i]) + 1) ? strlen(c[i]) + 1 : MAXLENGTH));
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
