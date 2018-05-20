/*
Program which implements a tab-based command
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minishell.h"
#include "ll.h"
#include "batch_mode.h"
#include "dictionary.h"

#define DEFAULT_DICTIONARY_FILE "./src/test_dict.txt"

/*
Checks if the input has two or more arguments and acts accordingly

@param args The list of arguments to check
@return 0 if there is no second argument, 1 if there is
*/
static int has_n_args(char **args, int n)
{
	int i;
  for (i = 1; i <= n; i++)
  {
    if (i == n) {
      ft_putendl("hello: too many arguments");
      return (1);
    }
    if (!args[i])
      break;
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


void autocomplete(char *word, char *dict, int length)
{

	if (dict == NULL)
	   dict = DEFAULT_DICTIONARY_FILE;
	int len = strlen(dict);
  char *fileType = &dict[len-4];
	if (strncmp(fileType, ".txt", 4) != 0)
	  dict = DEFAULT_DICTIONARY_FILE;


  int x, y;
	int x_org, y_org; //used for clearing screen
  getyx(stdscr, y, x);
	getyx(stdscr, y_org, x_org); //used for clearing screen

  printw("\nHere are suggestions to automplete \"%s\"\n", word);

  char **children = get_children_in_dict(word, dict);
  // In order to restrict the number of options printed, change "num_children" to
  // some number that was inputed
  int num_children = num_children_in_dict(word, dict);
  int i;
  for (i = 0; i < num_children; i++)
    printw("%d: %s\n", i, children[i]);

	//print this after autocomplete options to make tabbing less messy
	//if (length > 10) {
  //  printw("Only the first ten possibilities displayed\n");
  //  length = 10;
  //}

  int c;
	y++;
	int moved = 0; //keep track of how many tabs hit for cycling
	int og_y = y; //used for moving cursor to correct position for tab-completion

  //move to position
  x = 0;
	y++;
	moved++;
	wmove(stdscr, y, x);
  // tab through options, hit enter to select
  while(10 != (c = getch())) {
		if(c == 9) {
			if (moved < num_children) {
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
  wmove(stdscr, y_org, (x_org-length)); // prints over the typed word
  wrefresh(stdscr);
  printw("%s ", children[moved-1]);
  clrtobot();
}

// command to enter interactive autocomplete mode
int lets_tab_builtin(char **args)
{
  if (has_n_args(args, 2) == 1) {} //does nothing for now
  struct word *word = NULL; //list

	//bool server = false;
	char *dict;

	if (strncmp(args[0], "-s", 2) == 0) {
    dict = "./src/test_prefixes.txt"; //placeholder for server location of dictionary
		//server = true;
	} else {
		dict = args[1];
	}


	/*
	char *flag = args[1];
	if (strncmp(flag, "-s", 2) == 0) {
	  //placeholder for server
		dict = args[2];
	} else {
		dict = args[2];
	}
	*/

  int length = 0;
  int c;
  initscr();    // Start Curses Mode
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
      char *wordTyped = malloc(sizeof(char)*(length+1));
      wordTyped[length] = '\0'; // Without this, there are rando characters at the end of words
      int i = length;
      while (i != 0) {
        wordTyped[i-1] = word->letter;
        i--;
        word = ll_pop(word);
    }
      autocomplete(wordTyped, dict, length);
      length = 0;
    }
    cbreak();
  }
  clear();
  endwin();
  return(1);
}
