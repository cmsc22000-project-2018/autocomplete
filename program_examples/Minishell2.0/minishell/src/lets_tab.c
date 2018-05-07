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

/* Jonas 05.06: eventually, we're going to have to change
 * "length" to the number of completions from the trie
 * for now though, I don't want to implement a dummy datastructure
 */
void autocomplete(char *word, int length)
{
  
  int x, y;
  getyx(stdscr, y, x);

  printw("\nHere are suggestions to automplete \"%s\"\n", word);
  if (length > 10) {
    printw("Only the first ten possibilities displayed\n");
    length = 10;
  }
  int b;
  for (b = 0; b < length; b++)
    printw("%d: %s%c\n", b, word, b+97);
  printw("enter which number you want");
 
  int c;
  c = getch();
  wmove(stdscr, y, x);
  wrefresh(stdscr);
  // Temporary print, when we get the tries we'll figure
  printw("%c ", c+49);
  clrtobot();
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
    if (c != 9) {
      printw("%c", c);
      word = ll_new(word);
      word->letter = c;
      length++;
    }
    
    if (c == 32) {
      length = 0;
      word = NULL;
      word = ll_new(word);
    }
    if (c == 9) {
      char *wordTyped = malloc(sizeof(char)*length);
      int i = length;
      while (i != 0) {
        wordTyped[i-1] = word->letter;
        i--;
        word = ll_pop(word);
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