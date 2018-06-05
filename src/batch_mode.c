/*
Program which implements batch mode autocomplete within the same framework as bash mode
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "minishell.h"
#include "prefix.h"
#include "batch_mode.h"
#include "dictionary.h"
#include "log.h"

#include "../api/include/trie.h"
#include "../api/lib/redis-tries/include/trie.h"

#define SHOWNWORDS 10
#define MAXPREFLEN 32


// See batch_mode.h
char** get_n_children_in_dict(char* s, char* dict_file, int n)
{
	
	dict_t *d = dict_new();
	int rc = dict_read(d, dict_file);
	assert (rc == EXIT_SUCCESS);

	assert ((trie_contains(d->dict, s) == 2) || (trie_contains(d->dict, s) == 0));
	
	printf("%s", s);
	char** children = trie_approx(d->dict, s, 2, n); 
		//default for max_edit_dist in TRIE.APPROX is 2
	
	return children;
	
}

// See batch_mode.h
int num_children_in_dict(char* s, char* dict_file) 
{
	
	dict_t *d = dict_new();
        int rc = dict_read(d, dict_file);
        assert (rc == EXIT_SUCCESS);

	assert ((trie_contains(d->dict, s) == 2) || (trie_contains(d->dict, s) == 0));

	//int c = trie_count_completion(d->dict, s);

        int c = 2;
	return c;
}

//TODO: case for when dict_file is not provided, -d not selected, and connecting to redis dict
//TODO: do we still need the prefix_t struct?
// Prints the prefix, the number of children, if b==1 also the first n children.
//needs logging
void print_children(int b, int n, char* s, char* dict_file)
{
    log_debug("print_children: ENTERING FUNC");    
    prefix_t* prefix;

		//Lowercasing, from https://stackoverflow.com/questions/2661766/c-convert-a-mixed-case-string-to-all-lower-case
		char* str = malloc(MAXPREFLEN * sizeof(char));
		strcpy(str, s);
		for(int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
    log_trace("print_children: converted input to lowercase: %s to %s", s, str);
    char** children = get_n_children_in_dict(str, dict_file,n);
    log_trace("print_children: retrieved children");
    int num_children = num_children_in_dict(str, dict_file);
    prefix = prefix_new(s, children, num_children);
    log_trace("print_children: created prefix struct");

    //Currently children ins a memory leak because get_children_in_dict mallocs strings that are never freed
    //This should be corrected by a 'real' get_children which simply returns pointers to its
    //own data which is freed elsewhere

    ft_putstr(prefix->prefix);
    log_trace("print_children: printing prefix %s", prefix->prefix);
    ft_putstr(": ");

    
    //If the prefix is relatively short, inserts ... to even out line length.
    int numDots = 0;
		int prefLen = strlen(prefix->prefix);
    if (prefLen < MAXPREFLEN/2) {
			  numDots = MAXPREFLEN/2 - prefLen - 3;
		}
		for (int i = 0; i < numDots; i++) {
			  ft_putstr(".");
		}
		// Sometimes numDots becomes negative, ex. if preflen + 1 = MAXPREFLEN/2
		if(numDots > 0) {
			  ft_putstr(" ");
		}

		char* temp_string = malloc(MAXPREFLEN * sizeof(char));
		sprintf(temp_string,"%d",prefix->nComps);
		ft_putstr(temp_string);
		free(temp_string);

    if (b) {
			  ft_putstr(",");
        ft_putstr(" [");
        for (int i = 0; i < prefix->nComps && i < n; i++) {
					  //Restore capitalization of prefix
					  char* compi = malloc(strlen(prefix->completions[i]) + 1);
						strcpy(compi, prefix->completions[i]);
						for (int j = 0; j < prefLen; j++) {
							  compi[j] = prefix->prefix[j];
						}
            ft_putstr(compi);
            if (i != prefix->nComps - 1) {
                ft_putstr(", ");
            }
            if (i == n - 1 && n < prefix->nComps) {
                ft_putstr("...");
            }
        }
        //Prevent [] display with n = 0
        if (n == 0) {
            ft_putstr("...");
        }

        ft_putstr("]\n");
    }
    else {
        ft_putstr("\n");
    }

    prefix_free(prefix);
   
}

//Function to enter batch autocomplete mode within the interactive framework
//needs logging 
int batch_mode_builtin(char **args)
{
	log_debug("batch_mode_builtin: ENTERING FUNC");
        int showWords = 0;
	int nWords = SHOWNWORDS;
	char* dictionary = "/src/lcase_dict.txt"; //Once we can, should be initialized to the redis dictionary
	FILE* prefixFile;
	int fileSet = 0;

	/*Parse input instructions into global variables.
		Note that the first argument has been stripped already, so i starts at 0.
		Currently uses asserts to avoid reading missing arguments,
		we may want to make this more robust later.
	*/
	for (int i = 0; args[i] != NULL; i++) {
			if (!strncmp(args[i], "-w", 2)) {
					showWords = 1;
			}
			if (!strncmp(args[i], "-n", 2)) {
					assert(args[i + 1] != NULL);
					nWords = atoi(args[i + 1]);
					i++;
			}
			if (!strncmp(args[i], "-d", 2)) {
					assert(args[i + 1] != NULL);
				  //  dictionary = fopen(args[i + 1], "r");
					dictionary = calloc(sizeof(args[i+1]) + 1, sizeof(char));
						//TODO: is this the right use of calloc?? 
	        strcpy(dictionary, args[i+1]);
					i++;
			}
			if (!strncmp(args[i], "-f", 2)) {
					assert(args[i + 1] != NULL);
					prefixFile = fopen(args[i + 1], "r");
					fileSet = 1;
					i++;
			}
			if (!strncmp(args[i], "-o", 2)) {
					assert(args[i + 1] != NULL);
					freopen(args[i + 1], "w", stdout); //Sets stdout to be the file instead of the terminal
					i++;
			}
	}
    if (fileSet) {
		    char* currentPrefix = malloc(MAXPREFLEN * sizeof(char)); //Temporary constant value
        //For a cleaner look in shell
				ft_putstr("\n");
		    while (fgets(currentPrefix, MAXPREFLEN, prefixFile)) {   //Temporary constant value
					  //Strips the newline character by replacing it with the null terminator
		    		currentPrefix[strlen(currentPrefix) - 1] = '\0';
		    		print_children(showWords, nWords, currentPrefix, dictionary);
		    }
	    	free(currentPrefix);
    }

	//Currently there are no fcloses because of valgrind issues, should probably be added though

  return(1);
}
