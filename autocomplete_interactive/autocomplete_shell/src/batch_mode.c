/*
Program which implements bash mode autocomplete
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minishell.h"
#include "prefix.h"
#include "batch_mode.h"
#include "dictionary.h"

#define SHOWNWORDS 10


// See autocomplete.h
char** get_children_in_dict(char* s, char* dict_file)
{
	dict_t *d = dict_new();
	int rc = dict_read(d, dict_file);
	assert (rc == EXIT_SUCCESS);

	//TODO: function that checks whether given string is a valid prefix
	// since a prefix could exist that isn't a full word in the dictionary
	assert((dict_exists(d, s)) == EXIT_SUCCESS);

	int i = 0;
	int j = 0;
	char** children = malloc(10*sizeof(char)); //Temporary hard value

	while((d->dict)->words[i] != NULL) {
		//if the prefix is contained fully in a dictionary word
		if(strncmp(s, (d->dict)->words[i], strlen(s)) == 0) {
			children[j] = malloc(sizeof((d->dict)->words[i] + 1));
			strcpy(children[j], (d->dict)->words[i]);
			j++;
			i++;
		} else {
			i++;
		}
	}

	return children;
}

// See batch_mode.h
int num_children_in_dict(char* s, char* dict_file) {
	dict_t *d = dict_new();
        int rc = dict_read(d, dict_file);
        assert (rc == EXIT_SUCCESS);

        assert((dict_exists(d, s)) == EXIT_SUCCESS);

        int i = 0;
        int j = 0;

        while((d->dict)->words[i] != NULL) {
                //if the prefix is contained fully in a dictionary word
                if(strncmp(s, (d->dict)->words[i], strlen(s)) == 0) {
                        j++;
                        i++;
                } else {
                        i++;
                }
	}

	return j;
}

/* Prints the prefix, the number of children, if b==1 also the first n children.
   TODO improve spacing to be more uniform across prefix lengths, i.e.
       prefixlong:    n [words, go, here]
       pref:          n [other, words, here]
*/
void print_children(int b, int n, char* s, char* dict_file)
{
    prefix_t* prefix;
    char** children = get_children_in_dict(s, dict_file);
    int num_children = num_children_in_dict(s, dict_file);
    prefix = prefix_new(s, children, num_children);

    //Currently children ins a memory leak because get_children_in_dict mallocs strings that are never freed
    //This should be corrected by a 'real' get_children which simply returns pointers to its
    //own data which is freed elsewhere

    ft_putstr(prefix->prefix);
    ft_putstr(": ");
		char* temp_string = malloc(64 * sizeof(char));
		sprintf(temp_string,"%d",prefix->nComps);
		ft_putstr(temp_string);
		free(temp_string);

    if (b) {
			  ft_putstr(",");
        ft_putstr(" [");
        for (int i = 0; i < prefix->nComps && i < n; i++) {
            ft_putstr(prefix->completions[i]);
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

// command to enter interactive autocomplete mode
int batch_mode_builtin(char **args)
{
	int showWords = 0;
	int nWords = SHOWNWORDS;
	char* dictionary; //Once we can, should be initialized to the redis dictionary
	FILE* prefixFile;
	int fileSet = 0;

	/*Parse input instructions into global variables.
		Note that args[0] is ./autocomplete itself, so it is ignored.
		Currently uses asserts to avoid reading missing arguments,
		we may want to make this more robust later.
	*/
	for (int i = 0; args[i] != NULL; i+=2) {
			if (!strncmp(args[i], "-w", 2)) {
					assert(args[i + 1] != NULL);
					showWords = atoi(args[i + 1]);
			}
			if (!strncmp(args[i], "-n", 2)) {
					assert(args[i + 1] != NULL);
					nWords = atoi(args[i + 1]);
			}
			if (!strncmp(args[i], "-d", 2)) {
					assert(args[i + 1] != NULL);
				  //  dictionary = fopen(args[i + 1], "r");
					dictionary = malloc(sizeof(args[i+1]) + 1);
	        strcpy(dictionary, args[i+1]);
			}
			if (!strncmp(args[i], "-f", 2)) {
					assert(args[i + 1] != NULL);
					prefixFile = fopen(args[i + 1], "r");
					fileSet = 1;
			}
			if (!strncmp(args[i], "-o", 2)) {
					assert(args[i + 1] != NULL);
					freopen(args[i + 1], "w", stdout); //Sets stdout to be the file instead of the terminal
			}
	}
    if (fileSet) {
		    char* currentPrefix = malloc(64 * sizeof(char)); //Temporary constant value
		    while (fgets(currentPrefix, 64, prefixFile)) {   //Temporary constant value
		    		currentPrefix[strlen(currentPrefix) - 1] = '\0';
		    		print_children(showWords, nWords, currentPrefix, dictionary);
		    }
	    	free(currentPrefix);
    }

	//Currently there are no fcloses because of valgrind issues, should probably be added though

  return(1);
}
