/*
Program which implements a tab-based command
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minishell.h"
#include "ll.h"

/*
Checks if the input has two or more arguments and acts accordingly

@param args The list of arguments to check
@return 0 if there is no second argument, 1 if there is
*/
static int has_two_args(char **args)
{
	if (args[1])
	{
		if (args[2])
		{
			ft_putendl("hello: too many arguments");
			return (1);
		}
	}
	return (0);
}

// define struct for linked list library
struct word {
  char letter;
};

//where autocomplete would go
void autocomplete(char *word, int length)
{
	printw("\nHere are suggestions to automplete \"%s\"\n", word);
	for (int b = 0; b < length; b++)
		printw("%s%c\n", word, b+97);
	printw("\n");
}

// command to enter interactive autocomplete mode
int lets_tab_builtin(char **args)
{
  if (has_two_args(args) == 1)
    return(1);
  struct word *word = NULL; /* list */
  int length = 0;
  int c;
  initscr();    /* Start curses mode */
  cbreak();
  noecho();
  while('~' != (c = getch())) {
    printw("%c", c);
    word = ll_new(word);
    word->letter = c;
    length++;
    if (c == 9) {
      char *wordTyped = malloc(sizeof(char)*length);
      length--;
      int i = length;
      while (i != 0)
      {
        word = ll_pop(word);
        wordTyped[i-1] = word->letter;
        i--;
      }
      autocomplete(wordTyped, length);
			length = 0;
    }
    cbreak();
  }
	clear();
  endwin();
  return(1);
}
