/* Code based on code from Team Spellcheck */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "mock_trie.h"
#include "utils.h"

/* See mock_trie.h */
trie_t* trie_new(){
    trie_t *t;
    t = malloc(sizeof(trie_t));
    int rc;

    if (t == NULL) {
      error("Could not allocate memory");
      return NULL;
    }

    rc = trie_init(t);
    if(rc != 1)
    {
        error("Could not initialize trie");
        return NULL;
    }
    return t;
}


// See mock_trie.h
int trie_init(trie_t *t){
    assert(t != NULL);

    char **w;
    w = calloc(LEN, sizeof(char*));

    t->words = w;
    return 1;
}


// See mock_trie.h
int trie_free(trie_t *t){
    assert(t != NULL);
    assert(t->words != NULL);

    int i;

    for (i = 0; i < LEN; i++) {
        if (t->words[i] != NULL) {
            free(t->words[i]);
        }
    }

    free(t->words);
    free(t);

    return 1;
}


// See mock_trie.h
int in_trie(char *str, trie_t *t) {
    int i;

    for (i = 0; i < LEN ; i++) {
        if (t->words[i] == NULL) return 0;
        if (strcmp(str, t->words[i]) == 1) {
            return 1;
        }
    }
    return 0;
}



