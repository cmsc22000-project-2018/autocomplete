/* Code based on Team Spellcheck's dictionary.h code */

/*
 * A dictionary structure built using tries
 */

#ifndef INCLUDE_DICTIONARY_H_
#define INCLUDE_DICTIONARY_H_

#include "mock_trie.h"


/* a dictionary represented as a trie */
typedef struct {
	trie_t *dict;
} dict_t;


/*
 * Allocates a new dictionary.
 *
 * Parameters:
 *  - dict: a filename or dictionary file
 *
 * Returns:
 *  - A pointer to the dictionary, or NULL if a 
 *    dictionary cannot be allocated
 */
dict_t* dict_new();


/*
 * Initializes a dictionary
 * 
 * Parameters: 
 *  - d: a dictionary. Must point to already allocated memory.
 *
 * Returns:
 *  - 0 on success, 1 if an error occurs
 */
int dict_init(dict_t *d);


/*
 * Frees the resources associated with a dictionary
 *
 * Parameters:
 *  - d: a dictionary. Must point to a dict_t allocated with dict_new
 *
 * Returns:
 *  - Always returns 0.
 */
int dict_free(dict_t *d);


/* 
 * Checks if a string is contained within a dictionary
 *
 * Parameters: 
 *  - str: a string
 *  - d: a dictionary
 *
 * Returns:
 *  - 1 if the word is contained in the dictionary
 *  - 0 if the words is not contined
 *  - -1 if there was an error
 */
int in_dict(char *str, dict_t *d);


/*
 * Parses a file and adds all words to the dictionary
 * 
 * Parameters:
 *  - d: a dictionary. Must point to already allocated memory.
 *  - file: a filename string
 *
 * Returns:
 *  - 1 on success
 *  - 0 if an error occurs
 *  - -1 if partial error (only able to add some words)
 */
int read_to_dict(char *file, dict_t *d);


#endif /* INCLUDE_DICTIONARY_H_ */

