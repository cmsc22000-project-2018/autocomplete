#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"
#include "autocomplete.h"

#define SHOWNWORDS 10

//TODO replace with real children in dict function (and add header in .h file)
char** get_children_in_dict(char* s, FILE* dictionary)
{
    char** children = malloc(4*sizeof(char*)); //Temporary hard value
    children[0] = malloc(sizeof(s) + 1);
    strcpy(children[0], s);
    children[1] = malloc(sizeof("second") + 1);
    strcpy(children[1], "second");
    children[2] = malloc(sizeof("prefixthree") + 1);
    strcpy(children[2], "prefixthree");
    children[3] = malloc(sizeof("another") + 1);
    strcpy(children[3], "another");

    dictionary = NULL;

    return children;
}

//TODO replace with real children in dict function (and add header in .h file)
int num_children_in_dict(char* s, FILE* dictionary) {
    dictionary = NULL;
    return 4;
}

/* Prints the prefix, the number of children, if b==1 also the first n children.
   TODO improve spacing to be more uniform across prefix lengths, i.e.
       prefixlong:    n [words, go, here]
       pref:          n [other, words, here]
*/
void print_children(int b, int n, char* s, FILE* dictionary)
{
    prefix_t* prefix;
    char** children = get_children_in_dict(s, dictionary);
    int num_children = num_children_in_dict(s, dictionary);
    prefix = prefix_new(s, children, num_children);

    //Currently children ins a memory leak because get_children_in_dict mallocs strings that are never freed
    //This should be corrected by a 'real' get_children which simply returns pointers to its
    //own data which is freed elsewhere

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

//TODO: create flag arguments to ensure proper values pulled.
//TODO: add else to main if, leading to interactive mode
int main(int argc, char* argv[])
{
    int showWords = 0;
    int nWords = SHOWNWORDS;
    FILE* dictionary; //Once we can, should be initialized to the redis dictionary
    FILE* prefixFile;
    FILE* outputFile;
    int prefixFileSet = 0;

    /*Parse input instructions into global variables.
      Note that argv[0] is ./autocomplete itself, so it is ignored.
      Currently uses asserts to avoid reading missing arguments,
      we may want to make this more robust later.
    */
    for (int i = 1; i < argc; i+=2) {
        if (!strncmp(argv[i], "-w", 2)) {
            assert(i + 1 < argc);
            showWords = atoi(argv[i + 1]);
        }
        if (!strncmp(argv[i], "-n", 2)) {
            assert(i + 1 < argc);
            nWords = atoi(argv[i + 1]);
        }
        if (!strncmp(argv[i], "-d", 2)) {
            assert(i + 1 < argc);
            dictionary = fopen(argv[i + 1], "r");
        }
        if (!strncmp(argv[i], "-f", 2)) {
            assert(i + 1 < argc);
            prefixFile = fopen(argv[i + 1], "r");
            prefixFileSet = 1;
        }
        if (!strncmp(argv[i], "-o", 2)) {
            assert(i + 1 < argc);
            freopen(argv[i + 1], "w", stdout); //Sets stdout to be the file instead of the terminal
        }
    }

    if (prefixFileSet) {
        char* currentPrefix = malloc(64 * sizeof(char)); //Temporary constant value
        while (fgets(currentPrefix, 64, prefixFile)) {   //Temporary constant value
            currentPrefix[strlen(currentPrefix) - 1] = '\0';
            print_children(showWords, nWords, currentPrefix, dictionary);
        }
        free(currentPrefix);
    }
    else {
        //Run interactive mode
    }

    //Currently there are no fcloses because of valgrind issues, should probably be added though

    return 0;
}
