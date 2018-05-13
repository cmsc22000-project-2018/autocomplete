/* Code based on code from Team Spellcheck */

#ifndef INCLUDE_MOCK_TRIE_H_
#define INCLUDE_MOCK_TRIE_H_

#define LEN 30


/* A mock-trie struct */
typedef struct {
	char** words;
} trie_t;


/*
 * Allocates a new trie in the heap.
 *
 * Parameters:
 *  - word: Words in the trie.
 *
 * Returns:
 *  - A trie, or NULL if a point
 *    cannot be allocated
 */
trie_t* trie_new();


/* 
 * Initializes a trie
 * 
 * Parameters: 
 *  - new_trie: A trie. Must point to already allocated memory.
 * 
 * Returns:
 *  - 0 on success, 1 if an error occurs.
 */
int trie_init(trie_t *t);


/* 
 * Frees the resources associated with a trie
 *
 * Parameters:
 *  - trie: A trie. Must point to a point allocated with trie_new.
 *
 * Returns: 
 *  - Always returns 0.
 */
int trie_free(trie_t *t);


/*
 * Checks if a word is in a trie
 * 
 * Parameters: 
 *  - str: a string
 *  - t: a trie
 * 
 * Returns:
 *  - 1 if word is in trie, 0 if not
 */
int in_trie(char* str, trie_t *t);

/*
 * Adds a word to trie
 *
 * Parameters:
 *  - str: a string
 *  - t: a trie.
 *
 * Returns:
 *  1 if word is in the trie
 *  0 if word is not in the trie
 */
int add_to_trie(char *str, trie_t *t);




#endif /* INCLUDE_MOCK_TRIE_H_ */ 