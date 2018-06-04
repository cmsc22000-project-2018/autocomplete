/* Code based off of Team Spellcheck's dictionary module */

/*
 * A dictionary data structure based on tries
 *
 * See dictionary.h for function documentation.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "dictionary.h"

/* See dictionary.h */
dict_t* dict_new() {

    dict_t *d;
    int rc;

    d = malloc(sizeof(dict_t));

    if (d == NULL) {
        fprintf(stderr, "dict_new: malloc failed\n");
	return NULL;
    }

    rc = dict_init(d);
    if (rc != EXIT_SUCCESS) {
 	fprintf(stderr, "dict_new: dict init failed\n");
        return NULL;
    }

    return d;
}

/* See dictionary.h */
int dict_init(dict_t *d) {
    assert(d != NULL);

    trie_t *t = trie_new("dict");
    if (t == NULL) {
   	fprintf(stderr, "dict_init: trie_new failed\n"); 
       return EXIT_FAILURE;
    }
    d->dict = t;

    return EXIT_SUCCESS;
}

/* See dictionary.h */
int dict_free(dict_t *d) {
    assert(d != NULL);
    assert(d->dict != NULL);

    trie_free(d->dict);
    free(d);

    return EXIT_SUCCESS;
}

/* See dictionary.h */
int dict_exists(dict_t *d, char *str) {
    if (d == NULL || d->dict == NULL || str == NULL || str[0] == '\0') {
	fprintf(stderr, "dict_exists: returning EXIT_FAILURE\n");
        return EXIT_FAILURE;
    }

    int rc = trie_contains(d->dict, str);

    if (rc == 0) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

/* See dictionary.h */
int dict_add(dict_t *d, char *str) {
    if (d == NULL || d->dict == NULL || str == NULL) {
	fprintf(stderr, "dict_add: returning EXIT_FAILURE\n");
        return EXIT_FAILURE;
    }

	int rc = trie_insert(d->dict, str);

	if (rc == 0) {
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

/* See dictionary.h */
int dict_read(dict_t *d, char *file) {

    // From here: https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word
    char buffer[61];
    FILE *f = fopen(file, "r");

    if (f == NULL) {
	fprintf(stderr, "dict_read: EXIT_FAILURE, fopen = NULL\n"); 
       return EXIT_FAILURE;
    }

    while (fscanf(f, "%100s", buffer) == 1) {
        if (dict_add(d, buffer) != EXIT_SUCCESS) {
		fprintf(stderr, "dict_read: EXIT_FAILURE, dict_add failed\n");
            return EXIT_FAILURE;
        }
    }

    fclose(f);

    return EXIT_SUCCESS;
}
