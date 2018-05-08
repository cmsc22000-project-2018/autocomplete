#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "prefix.h"
#include "autocomplete.h"

#define SHOWNWORDS 10
#define MAXPREFS 2048

//TODO replace with real children in dict function (and add header in .h file)
char** get_children_in_dict(char* s, char* tempGarbage)
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

    tempGarbage = NULL;

    return children;
}

//TODO replace with real children in dict function (and add header in .h file)
int num_children_in_dict(char* s, char* tempGarbage) {
    tempGarbage = NULL;
    return 4;
}

/* Prints the prefix, the number of children, if b==1 also the first n children.
   TODO improve spacing to be more uniform across prefix lengths, i.e.
       prefixlong:    n [words, go, here]
       pref:          n [other, words, here]
*/
void print_children(int b, int n, char* s)
{
    prefix_t* prefix;
    char** children = get_children_in_dict(s, "temp_fake_dict"); //TODO replace with real dict
    int num_children = num_children_in_dict(s, "temp_fake_dict"); //TODO replace with real dict
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
    if (argc > 1) {
        int showWords = atoi(argv[1]);
        int nWords;
        if (argc > 2) {
            nWords = atoi(argv[2]);
        }
        else {
            nWords = SHOWNWORDS;
        }

        init_parser();
        char* s = NULL;

        while (read_string(&s)) {
            print_children(showWords, nWords, s);
            next_token();
        }
    }

    return 0;
}
