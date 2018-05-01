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

char** get_children(char* s, int* n)
{
    char** children = malloc(3 * 50); //Temporary hard value
    children[0] = (char *) s;
    children[1] = "second";
    children[2] = "prefixthree";
    *n = 3;
    return children;
}

//Needs tweaking to print to proper file
void print_children(void* s)
{
    int num_children;
    char** children = get_children((char*) s, &num_children);
    fprintf(stdout, "%s", (char *) s);
    fprintf(stdout, ": %d [", num_children);
    for (int i = 0; i < num_children; i++) {
        fprintf(stdout, "%s", children[i]);
        fprintf(stdout, ", ");
    }
    fprintf(stdout, "]\n");

    free(children);
}

int main(int argc, char* argv[])
{
    init_parser();
    struct Node* prefixes = malloc(50); //TODO Fix this malloc

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

    fmap(prefixes, print_children);

    return 0;
}
