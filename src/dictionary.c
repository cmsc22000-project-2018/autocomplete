/* Code based off of Team Spellcheck's dictionary module */

/* dictionary data structure based on tries
 *
 * See dictionary.h for function documentation.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "dictionary.h"
#include "log.h"

/* See dictionary.h */
dict_t* dict_new() {
    log_debug("dict_new: ENTER FUNC");
    dict_t *d;
    int rc;

    d = malloc(sizeof(dict_t));

    if (d == NULL) {
        log_fatal("dict_new dictionary malloc failed");
        return NULL;
    }

    rc = dict_init(d);
    if (rc != EXIT_SUCCESS) {
        log_fatal("dict_new dictionary init failed");
        return NULL;
    }
    log_debug("dict_new: EXIT FUNC");
    return d;
}

/* See dictionary.h */
int dict_init(dict_t *d) {
    log_debug("dict_init: ENTER FUNC");
    assert(d != NULL);

    trie_t *t = trie_new("dict");
    if (t == NULL) {
        log_fatal("dict_init trie_new failed");
        return EXIT_FAILURE;
    }
    d->dict = t;
    log_debug("dict_init: EXIT FUNC");
    return EXIT_SUCCESS;
}

/* See dictionary.h */
int dict_free(dict_t *d) {
    assert(d != NULL);
    assert(d->dict != NULL);

    trie_free(d->dict);
    free(d);

    log_debug("dict_free success");
    return EXIT_SUCCESS;
}

/* See dictionary.h */
int dict_exists(dict_t *d, char *str) {
    if (d == NULL || d->dict == NULL || str == NULL || str[0] == '\0') {
        log_trace("dict_exists returning EXIT_FAILURE");
        return EXIT_FAILURE;
    }

    log_debug("dict_exists entering trie_contains");
    int rc = trie_contains(d->dict, str);

    if (rc == 0) {
        log_trace("dict_exists returning EXIT_SUCCESS");
        return EXIT_SUCCESS;
    }

    log_trace("dict_exists returning EXIT_FAILURE");
    return EXIT_FAILURE;
}

/* See dictionary.h */
int dict_add(dict_t *d, char *str) {
    log_debug("dict_add: ENTER FUNC");
    if (d == NULL || d->dict == NULL || str == NULL) {
        log_trace("dict_add: returning EXIT_FAILURE");
        return EXIT_FAILURE;
    }

    if (strnlen(str, MAXSTRLEN+1) == MAXSTRLEN+1) {
        log_trace("dict_add: returning EXIT_FAILURE");
        return EXIT_FAILURE;
    }
    log_debug("dict_add: entering trie_insert");
    log_trace("dict_add: attempting to insert '%s'", str);
    int rc = trie_insert(d->dict, str);
    log_debug("dict_add: exiting trie_insert");
    if (rc == 0) {
        log_trace("dict_add returning EXIT_SUCCESS");
        return EXIT_SUCCESS;
    }

    log_trace("dict_add returning EXIT_FAILURE");
    return EXIT_FAILURE;
}

/* See dictionary.h */
int dict_read(dict_t *d, char *file) {

    // From here: https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word
    char buffer[MAXSTRLEN + 1];
    FILE *f = fopen(file, "r");
    log_debug("opened file %s", file);

    if (f == NULL) {
        log_trace("dict_read: file was NULL");
        return EXIT_FAILURE;
    }
    log_trace("dict_read: entering while loop");
    int i = 1;
    while (fscanf(f, "%100s", buffer) == 1) {
 
    log_trace("dict_read: while loop interation %d", i);
        if (dict_add(d, buffer) != EXIT_SUCCESS) {
            log_trace("dict_read: dict_add failed");
            return EXIT_FAILURE;
        }
    i++;
    }

    fclose(f);

    log_trace("dict_read returning EXIT_SUCCESS");
    return EXIT_SUCCESS;
}

/* See dictionary.h */
char **dict_suggestions(dict_t *d, char *str, int max_edits, int n) {
    if (d == NULL || d->dict == NULL || str == NULL) {
        log_trace("returning EXIT_FAILURE");
        return NULL;
    }

    char **results = trie_approx(d->dict, str, max_edits, n);

    return results;
}


