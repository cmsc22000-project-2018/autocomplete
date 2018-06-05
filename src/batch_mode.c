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

#include "../api/include/trie.h"
//#include "../api/src/trie/trie.c"

#define SHOWNWORDS 10
#define MAXPREFLEN 32
#define UNIX_MAX_PATH 4096
//#define DEFAULT_DICTIONARY_FILE "default"

// See batch_mode.h
char** get_n_children_in_dict(char* s, dict_t* d, int n)
{
	printf("entering get_n_children_in_dict\n");
// TODO: assert ((trie_contains(d->dict, s) == 2) || (trie_contains(d->dict, s) == 0));
	
	printf("%s", s);
//	char** children = trie_approx(new_dict->dict, s, 2, n); 
		//default for max_edit_dist in TRIE.APPROX is 2
	printf("get_n_child_d: about to call dict_suggestions\n");
	char** children = dict_suggestions(d, s, 6, n);

	return children;
	
}

// See batch_mode.h
int num_children_in_dict(char* s, dict_t* d) 
{
	printf("entering num_children_in_dict\n");
//TODO:	assert ((trie_contains(d->dict, s) == 2) || (trie_contains(d->dict, s) == 0));
	printf("num_child_in_dict: about to call trie_completions\n");
	int c = trie_completions(d->dict, s);
//	printf("num_ch_d: about to assert d!=NULL\n");
//	assert(d != NULL);
//	printf("num_ch_d: successful assetion that d!=NULL\n");
//	int c = strlen(s);
	printf("num_ch_d:finished trie_completions, value= %d\n", c); 
	return c;
}

//TODO: case for when dict_file is not provided, -d not selected, and connecting to redis dict
//TODO: do we still need the prefix_t struct?
// Prints the prefix, the number of children, if b==1 also the first n children.
void print_children(int b, int n, char* s, dict_t* dict)
{
	printf("entering print_children\n");
    prefix_t* prefix;

		//Lowercasing, from https://stackoverflow.com/questions/2661766/c-convert-a-mixed-case-string-to-all-lower-case
		char* str = malloc(MAXPREFLEN * sizeof(char));
		strcpy(str, s);
		for(int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
	printf("print_children: about to call get_n_ch_d and num_ch_d\n");
    char** children = get_n_children_in_dict(str, dict,n);
    int num_children = num_children_in_dict(str, dict);
	printf("print_children: about to call prefix_new, word is %s\n", s);
    prefix = prefix_new(s, children, num_children);
	printf("print_children: finished calling prefix_new\n");
    //Currently children ins a memory leak because get_children_in_dict mallocs strings that are never freed
    //This should be corrected by a 'real' get_children which simply returns pointers to its
    //own data which is freed elsewhere

    ft_putstr(prefix->prefix);
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
	printf("print_children: about to call prefix_free before exiting\n");
    prefix_free(prefix);
   
}

//Function to enter batch autocomplete mode within the interactive framework
int batch_mode_builtin(char **args)
{
	printf("entering batch_mode_builtin\n");
	int showWords = 0;
	int nWords = SHOWNWORDS;
	char* dictionary = malloc(UNIX_MAX_PATH * sizeof(char *));//Once we can, should be initialized to the redis dictionary
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
//					dictionary = calloc(sizeof(args[i+1]) + 1, sizeof(char));
						//TODO: is this the right use of calloc?? 
	     		if(strcmp(args[i+1], DEFAULT_DICTIONARY_FILE) ==0) {
				strcpy(dictionary, DEFAULT_DICTIONARY_FILE);
				i++;
			} else { 
				 strcpy(dictionary, args[i+1]);
				i++;
			}
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

 
	printf("batch_mode_builtin: about to call new_dictionary_from_file\n");
	dict_t* new_dict = new_dict_from_file(dictionary);
	

   if (fileSet) {
		    char* currentPrefix = malloc(MAXPREFLEN * sizeof(char)); //Temporary constant value
        //For a cleaner look in shell
				ft_putstr("\n");
		    while (fgets(currentPrefix, MAXPREFLEN, prefixFile)) {   //Temporary constant value
					  //Strips the newline character by replacing it with the null terminator
		    		currentPrefix[strlen(currentPrefix) - 1] = '\0';
		    		print_children(showWords, nWords, currentPrefix, new_dict);
		    }
	    	free(currentPrefix);
    }

	//Currently there are no fcloses because of valgrind issues, should probably be added though

  return(1);
}
