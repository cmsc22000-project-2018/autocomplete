/* This parser was taken from the cs152coin project
   in CS 15200, winter 2018. The testing files, and
   a select few changes, are not part of the original parser. */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser_dict.h"

// TODO Determine whether this is large enough
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


