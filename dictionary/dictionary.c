/* Code based on code from Team Spellcheck */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include "utils.h"
#include "dictionary.h"

/* See dictionary.h */
dict_t* dict_new() {

    dict_t *d;
    int rc;

    d = malloc(sizeof(dict_t));

    if (d == NULL) {
        error("Could not allocate memory");
        return NULL;
    }

    rc = dict_init(d);
    if (rc != 0) {
        error("Could not initialize dict");
        return NULL;
    }

    return d;
}


/* See dictionary.h */
int dict_init(dict_t *d) {
    assert(d != NULL);

    trie_t *t = trie_new();
    if (t == NULL) {
        return 1;
    }
    d->dict = t;

    return 0;
}

/* See dictionary.h */
int dict_free(dict_t *d) {
    assert(d != NULL);
    assert(d->dict != NULL);

    trie_free(d->dict);
    free(d);

    return 0;
}


/* See dictionary.h */
int in_dict(char *str, dict_t *d) {
    if (d == NULL || d->dict == NULL || str == NULL || str[0] == '\0') {
        error("Invalid input to in_dict");
        return -1;
    }

    return in_trie(str, d->dict);
}


/* TODO: format this with the parser we have currently */
/* See dictionary.h */
int read_to_dict(char *file, dict_t *d) {

    int rc = 1;

    // From here: https://stackoverflow.com/questions/16400886/reading-from-a-file-word-by-word
//    char buffer[1024];
    FILE *f = fopen(file, "r");

    if (f == NULL) {
        return 0;
    }

//    while (fscanf(f, "%1023s", buffer) == 1) {
  //      if (add_to_dict(buffer, d) != 1) {
    //        rc = -1;
      //  }
  //  }

    fclose(f);

    return rc;
}


/* Returns true if the given word contains the given prefix entirely */
/* Code based on code found on https://stackoverflow.com/a/4770992 */
int prefix_in_word(const char *prefix, const char *word)
{	
	return strncmp(prefix, word, strlen(prefix)) == 0;
}


/* Returns children from a custom dictionary */
prefix_t* get_children_dict(char *s, dict_t *d)
{
	assert(d != NULL);
	assert(d->dict != NULL);
	
	if(in_dict(s, d) == 0) {
		fprintf(stderr, "get_children_dict: prefix not in dict.\n");
		exit(1);
	}
	
	int i = 0;
	int j = 0;
	char** children;
	children[0] = malloc(sizeof(s) + 1);

	while(d->dict[i] != NULL) {
		if(prefix_in_word?(s, d->dict[i]) != 0) {
			children[j] = malloc(sizeof(d->dict[i] + 1);
			strcpy(children[j], d->dict[i]);
			j++;
			i++;
			} else {
				i++;	
		}
	prefix_t* p = prefix_new(s, children, j);
	
	for(int n=0; n<j; n++) {
		free(children[n]);
	}
	
	return p;		

}


/* Prints the prefix, the number of children, if b==1 also the first n children.
   TODO improve spacing to be more uniform across prefix lengths, i.e.
       prefixlong:    n [words, go, here]
       pref:          n [other, words, here]
*/
void print_children_dict(int b, int n, char* s, dict_t *d)
{   
    prefix_t* prefix = get_children_dict(s,d);
    printf("%s", prefix->prefix);
    printf(": %d", prefix->nComps);
    
    if (b) {
        printf(" [");
        for (int i = 0; i < prefix->nComps && i < n; i++) {
            printf("%s", prefix->completions[i]);
            if (i != prefix->nComps - 1) {
                printf(", ");
            }
            if (i == n - 1 && n < prefix->nComps) {
                printf("...");
            }
        }
        //Prevent [] display with n = 0
        if (n == 0) {
            printf("...");
        }
        
        printf("]\n");
    }
    else {
        printf("\n");
    }
    
    prefix_free(prefix);
}

//main function
//TODO: still need to figure out how to pipe more than one file into the terminal at once
//For now, the dict file must be an optional command line argument
// The text file will be piped in as usual
int main(int argc, char* argv[])
{
    if (argc > 1) {
        int showWords = atoi(argv[1]);
        int nWords;
	int x = 0;

	//if the only argument passed is an int
	if (argc = 2) { 
		nWords = SHOWNWORDS;
	} 

	//if two ints are passed
	if (argc = 3 && (atoi(argv[2] > 0))) {
		nWords = atoi(argv[2]);
	}

	//if one int and a dict is passed
	if (argc = 3 && (atoi(argv[2]) == 0)) {
		nWords = SHOWNWORDS;
		dict_t *d = dict_new();
		x = read_to_dict(argv[2], d);
		if(x != 1) {
			fprintf("error: unable to read dict\n");
			exit(1);
		}
	} 

	//if two ints and a dict is passed
        if (argc > 2 && (atoi(argv[2]) > 0))  {
            nWords = atoi(argv[2]);
		dict_t *d = dict_new();
		x = read_to_dict(argv[2], d);
                if(x != 1) {
                        fprintf("error: unable to read dict\n");
                        exit(1);
                }
        }

        init_parser();
        char* s = NULL;
	
	if(x == 0) {		
	        while (read_string(&s)) {
	            print_children(showWords, nWords, s);
	            next_token();
	        }
	} else {
		while (read_string(&s)) {
			print_children_dict(showWords, nWords, s);
			next_token();
		}
	}

    return 0;
}




	
