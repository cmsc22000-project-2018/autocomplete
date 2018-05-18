/*
Program which implements a tab-based command
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
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
// Yaffe 05.13
// modified to implement a tab-based GUI.
// Note that Shift-Tab is impossible in the ncurses C library as Shift-Tab is not registered differently from TAB
// GUI works by tabbing through the options and the cursor cycles back to the frist when you go over. hit ENTER to select.

void autocomplete(char *word, int length)
{
  int x, y;
	int x_org, y_org; //used for clearing screen
  getyx(stdscr, y, x);
	getyx(stdscr, y_org, x_org); //used for clearing screen

  printw("\nHere are suggestions to automplete %s \n", word);
  int b;
  for (b = 0; b < length; b++)
    printw("%d: %s%c\n", b, word, b+97);
  printw("Tab through the options and hit enter on the correct one");

	//print this after autocomplete options to make tabbing less messy
	if (length > 10) {
    printw("Only the first ten possibilities displayed\n");
    length = 10;
  }

  int c;
	y++;
	int moved = 0; //keep track of how many tabs hit for cycling
	int og_y = y; //used for moving cursor to correct position for tab-completion

  //move to position
  x = 0;
	y++;
	moved++;
	wmove(stdscr, y, x);
	//////////////////

  // tab through options, hit enter to select
  while(10 != (c = getch())) {
		if(c == 9){
			if (moved < length) {
				y++;
				moved++;
			} else {
				y = og_y+1;
				moved = 1;
			}
			wmove(stdscr, y, x);
		}
	}
	c = mvwinch(stdscr, y, x);

  //clear autocomplete portion of screen
  wmove(stdscr, y_org, x_org);
  wrefresh(stdscr);
  // Temporary print, when we get the tries we'll figure
  printw("%c ", c+49);
  clrtobot();
}

// command to enter interactive autocomplete mode
int lets_tab_builtin(char **args)
{
  int lets_tab_builtin(char **args)
{
  if (has_two_args(args) == 1)
    return(1);
  struct word *word = NULL; //list
  int length = 0;
  int c;
  initscr();    // Start Curses Mode
  cbreak();
  noecho();
  while('~' != (c = getch())) {
    
    // Doesn't print tab, bkspace, or del
    if (c != 9 && c != 127 && c != 8) {
      printw("%c", c);
      word = ll_new(word);
      word->letter = c;
      length++;
    }

    if (c == 32) {
      length = 0;
      word = NULL;
    }

    if (c == 9) {
      char *wordTyped = malloc(sizeof(char)*(length+1));
      int i = length;
      /* Without incrementing length by one in the malloc and
       * adding the '/0' to the end, printing wordTyped after
       * deleting characters from it does not work.
       * Jonas 05/16
       */
      wordTyped[length] = '\0';
      while (i != 0) {
        wordTyped[i-1] = word->letter;
        i--;
        word = ll_pop(word);
      }
      autocomplete(wordTyped, length);
      length = 0;
    }

    /* Jonas 05.16: Implement delete key
     * Known bug: as of right now, we can only
     * delete one word at a time - the linked 
     * list only stores one word at a time, so
     * therefore we cannot access the prior word
     * after deleting the most recently typed one.
     * We'll have to change the word storage mechanism,
     * so I'm leaving that to Sprint 4
     */

    if (c == 127 || c == 8) { 
      int x, y;
      getyx(stdscr, y, x);
      x--;
      move(y, x);
      clrtobot();
      refresh();
      word = ll_pop(word);
      length--;
    }
    cbreak();
  }
  clear();
  endwin();
  return(1);
}
}