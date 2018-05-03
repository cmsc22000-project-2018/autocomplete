#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "linked_list.h"
#include "prefix.h"
#include "autocomplete.h"

#define SHOWNWORDS 10

/* This is a placeholder function until we can call a function with
   the same signature from the API team */
prefix_t* get_children(char* s)
{
    char** children = malloc(4*sizeof(char*)); //Temporary hard value
    children[0] = (char *) s;
    children[1] = "second";
    children[2] = "prefixthree";
    children[3] = "another";
    prefix_t* prefix = prefix_new(s, children, 4); //Temporary hard value
    free(children);
    return prefix;
}

/* Prints the prefix, the number of children, if b==1 also the first n children.
   TODO improve spacing to be more uniform across prefix lengths, i.e.
       prefixlong:    n [words, go, here]
       pref:          n [other, words, here]
*/
void print_children(void* s, const void* b, const void* n)
{
    prefix_t* prefix = get_children((char*) s);
    printf("%s", prefix->prefix);
    printf(": %d", prefix->nComps);

    if (*((int*) b)) {
        printf(" [");
        for (int i = 0; i < prefix->nComps && i < *((int*) n); i++) {
            printf("%s", prefix->completions[i]);
            if (i != prefix->nComps - 1) {
                printf(", ");
            }
            if (i == *((int*) n) - 1 && *((int*) n) < prefix->nComps) {
                printf("...");
            }
        }
        //Prevent [] display with n = 0
        if (*((int*) n) == 0) {
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
        struct Node* prefixes = malloc(sizeof(struct Node));
        char* s = NULL;

        if (!read_string(&s)) return 1;

        //This changes prefixes to point to the first node with data
        struct Node* store_old = prefixes;
        append(&prefixes, s, sizeof(s));
        free(store_old);

        next_token();
        struct Node* last_prefix = prefixes;
        while (read_string(&s)) {
            append(&last_prefix, s, sizeof(s) + 1);
            next_token();
        }

        fmap2(prefixes, print_children, (const void*) &showWords, (const void*) &nWords);

        free_linked_list(prefixes);
    }

    return 0;
}
