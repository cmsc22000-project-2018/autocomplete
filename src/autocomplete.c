#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "simclist.h"
#include "prefix.h"
#include "autocomplete.h"

#define SHOWNWORDS 10
#define MAXPREFS 2048

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
void print_children(int b, int n, char* s)
{
    prefix_t* prefix = get_children(s);
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
//TODO: solve valgrind errors and magical print statement
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
        list_t prefixes;
        list_init(&prefixes);
        char* s = NULL;
        char** s_array = malloc(MAXPREFS);

        list_attributes_copy(&prefixes, list_meter_string, 1);

        int i = 0;

        while (read_string(&s) && i < MAXPREFS) {
            s_array[i] = malloc(sizeof(s));
            strcpy(s_array[i], s);
            list_append(&prefixes, &(s_array[i]));

            //This loop -- and in fact the print statement within it -- is
            //currently necessary for the code to not segfault. That includes
            //changing this to a fprintf from a printf ???????

            //Further notes: fprintf to stdout works. However, fprintf to stderr
            //prints 'another' -- and only 'another' -- as (null) and segfaults
            //when 'another''s children would be accessed. ??????
            for (int j = -1; j < i; j++) {
                fprintf(stdout,"%s\n", *(char **) list_get_at(&prefixes, j+1));
            }
            i++;
            next_token();
        }

        char* pref;
        list_iterator_start(&prefixes);
        while (list_iterator_hasnext(&prefixes)) {
            pref = *(char **)list_iterator_next(&prefixes);
            print_children(showWords, nWords, pref);
        }
        list_iterator_stop(&prefixes);

        list_destroy(&prefixes);
        free(s_array);
    }

    return 0;
}
