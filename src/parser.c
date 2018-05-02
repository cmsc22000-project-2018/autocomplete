/* This parser was taken from the cs152coin project
   in CS 15200, winter 2018. The testing files, and
   a select few changes, are not part of the original parser. */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"

static const size_t LINE_MAX = 2048;
static const char* SEP = " \t\n";

static char* token = NULL;
static char buffer[2048]; //TODO figure out why this won't take LINE_MAX

void next_token()
{
    if (token != NULL) {
        token = strtok(NULL, SEP);
    }

    while (token == NULL) {
        // leave room so that we can insert extra space before the newline
        char* s = fgets(buffer, LINE_MAX, stdin);
        if (s == NULL) {
            if (ferror(stdin)) {
                fprintf(stderr, "Error reading input\n");
                exit(1);
            }
            assert(feof(stdin));
            break;
        }

        token = strtok(s, SEP);
    }
}

void init_parser()
{
    next_token();
}

int read_string(char** s)
{
    *s = token;
    return token != NULL;
}

// Functions for testing only (though main is mostly accurate)
#include "linked_list.h"

//This constant should also exist in the eventual main file
#define SHOWNWORDS 10;

char** get_children(char* s, int* n)
{
    char** children = malloc(3 * 50); //Temporary hard value
    children[0] = (char *) s;
    children[1] = "second";
    children[2] = "prefixthree";
    *n = 3;
    return children;
}

/* Prints the prefix, the number of children, if b==1 also the first n children.
   TODO improve spacing to be more uniform across prefix lengths, i.e.
       prefixlong:    n [words, go, here]
       pref:          n [other, words, here]
*/
void print_children(void* s, const void* b, const void* n)
{
    int num_children;
    char** children = get_children((char*) s, &num_children);
    printf("%s", (char *) s);
    printf(": %d", num_children);

    if (*((int*) b)) {
        printf(" [");
        for (int i = 0; i < num_children && i < *((int*) n); i++) {
            printf("%s", children[i]);
            if (i != num_children - 1) {
                printf(", ");
            }
            if (i == *((int*) n) - 1 && *((int*) n) < num_children) {
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

    free(children);
}

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
        append(&prefixes, s, sizeof(s));

        next_token();
        struct Node* last_prefix = prefixes;
        while (read_string(&s)) {
            append(&last_prefix, s, sizeof(s));
            next_token();
        }

        fmap2(prefixes, print_children, (const void*) &showWords, (const void*) &nWords);
    }

    return 0;
}
