/* Code based off of Team Spellcheck's dictionary module */

/*
 * A dictionary data structure based on tries
 *
 * See dictionary.h for function documentation.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "dictionary.h"

//#include "../api/src/trie/trie.c"

/* See dictionary.h */
dict_t* dict_new() {
	printf("entering dict_new\n");

    dict_t *d;
    int rc;

    d = malloc(sizeof(dict_t));

    if (d == NULL) {
	printf("dict_new: dict malloc failed\n");
        return NULL;
    }

    rc = dict_init(d);
    if (rc != EXIT_SUCCESS) {
	printf("dict_new: dict_init failed\n");
        return NULL;
    }
	
    printf("dict_new succeeded\n");
    return d;
}

/* See dictionary.h */
int dict_init(dict_t *d) {
	printf("entering dict_init\n");
    assert(d != NULL);

    trie_t *t = trie_new("dict");
    if (t == NULL) {
	printf("dict_init: trie_new failed\n");
        return EXIT_FAILURE;
    }
    d->dict = t;
	
	printf("dict_init succeeded\n");
    return EXIT_SUCCESS;
}

/* See dictionary.h */
int dict_free(dict_t *d) {
	printf("entering dict_free\n");
    assert(d != NULL);
    assert(d->dict != NULL);

    trie_free(d->dict);
    free(d);

    return EXIT_SUCCESS;
}

/* See dictionary.h */
dict_t* dict_official() {
	printf("entering dict_official()\n");

	dict_t *d;

	d = malloc(sizeof(dict_t));

	if (d == NULL) {
		printf("dict_official: dict malloc failed\n");
		return NULL;
	}

	trie_t *t = trie_new("dictionary");
	if(t==NULL) {
		printf("dict_official: trie_new failed\n");
		return NULL;
	}

	d->dict = t;

	return d;
}

/* See dictionary.h */
int dict_exists(dict_t *d, char *str) {
	printf("entering dict_exists\n");
    if (d == NULL || d->dict == NULL || str == NULL || str[0] == '\0') {
	printf("dict_exists: returning EXIT_FAILURE\n"); 
       return EXIT_FAILURE;
    }

	printf("dict_exists: about to enter trie_contains, word is %s\n", str);
    int rc = trie_contains(d->dict, str);

	if (rc==0) {
		printf("dict_exists: trie_contains succeeded\n");
		return EXIT_SUCCESS;
	}
	printf("dict_exists: trie_contains failed, returning EXIT_FAILURE\n");
	return EXIT_FAILURE;
}

/* See dictionary.h */
int dict_add(dict_t *d, char *str) {
	printf("entering dict_add\n");
    if (d == NULL || d->dict == NULL || str == NULL) {
	printf("dict_add: returning exitfailure\n");
        return EXIT_FAILURE;
    }


	if(strnlen(str, MAXSTRLEN+1) == MAXSTRLEN+1) {
		printf("dict_add: returning EXITFAILURE\n");
		return EXIT_FAILURE;
	}

	printf("dict_add: about to enter trie_insert, word is %s\n", str);
	int rc = trie_insert(d->dict, str);
	
	if (rc==0) {
		printf("dict_add: trie_insert succeeded\n");
		return EXIT_SUCCESS;
	}
	
	printf("dict_add: returning EXITFAILURE\n");
	return EXIT_FAILURE;
}

/* See dictionary.h */
int dict_read(dict_t *d, char *file) {
	printf("entering dict_read\n");
    // From here: https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word
    char buffer[MAXSTRLEN + 1];
    FILE *f = fopen(file, "r");
	printf("dict_read: fopen was just called on file: %s\n", file);

    if (f == NULL) {
	printf("dict_read: f==NULL, returning EXITFAILURE\n");
        return EXIT_FAILURE;
    }

    while (fscanf(f, "%100s", buffer) == 1) {
        if (dict_add(d, buffer) != EXIT_SUCCESS) {
		printf("dict_read: returning EXITFAILURE\n");
            return EXIT_FAILURE;
        }
    }
	printf("dict_read: no EXITFAILURES, about to call fclose on file: %s\n", file);
    fclose(f);
	printf("dict_read: fclose successful\n");
    return EXIT_SUCCESS;
}


/* See dictionary.h */
char **dict_suggestions(dict_t *d, char *str, int max_edits, int n) {
	printf("entering dict_suggestions\n");
 if (d == NULL || d->dict == NULL || str == NULL) {
         printf("dict_suggestions returning EXIT_FAILURE\n");
         return NULL;
     }
 
	printf("dict_suggestions: about to enter trie_approx, word is %s\n", str);
     char **results = trie_approx(d->dict, str, max_edits, n);
 
     return results;
 }


/* See dictionary.h */
dict_t* new_dict_from_file(char *filename) {
	printf("entering new_dict_from_file\n");
	
	char* dict = malloc(UNIX_MAX_PATH * sizeof(char *));
	strcpy(dict, filename);

	dict_t *new_dict;
	int msg;

	if(strcmp(filename, "default") == 0) {
		printf("new_dict_from_file: about to initialize default dict\n");
		new_dict = dict_official();

		if(new_dict == NULL) {
			msg = EXIT_FAILURE;
		} else {
			msg = EXIT_SUCCESS;
		}
	} else {
		printf("new_dict_from_file: about to create new custom dict\n");
		new_dict = dict_new();
		msg = dict_read(new_dict, filename);
		printf("new_dict_from_file: dict_read of custom finished\n");
		assert (msg == EXIT_SUCCESS);
		printf("new_dict_from_file: dict_read of custom successful\n");
	}

	if(msg == EXIT_FAILURE) {
		fprintf(stderr, "new_dict_from_file: invalid dict file\n");
		exit(0);
	}

	printf("new_dict_from_file: no EXIT_FAILURES, returning dict_t\n");
//	free(dict);
	return new_dict;
}


