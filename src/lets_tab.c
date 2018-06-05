/*
Program which implements a tab-based command
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "minishell.h"
#include "ll.h"
#include "batch_mode.h"
#include "dictionary.h"
#include "log.h"

#include "../api/include/trie.h"
#define DEFAULT_DICTIONARY_FILE "./src/case_dict.txt"
#define DEFAULT_AMT_COMPLETIONS 10
#define DEFAULT_MAX_PREF_LEN 32


// define struct for linked list library
struct word {
  char letter;
  int prefix_length;
};

// GUI works by tabbing through the options and the cursor cycles back to the frist when you go over. hit ENTER to select.
char* autocomplete(char *word, dict_t* d, int length, int maxCompletions)
{
  if (maxCompletions == -1)
   maxCompletions = DEFAULT_AMT_COMPLETIONS;
//  if (dict == NULL) {
//     dict = DEFAULT_DICTIONARY_FILE;
//	}
//  int len = strlen(dict);
//  char *fileType = &dict[len-4];
//  if (strncmp(fileType, ".txt", 4) != 0) {
 //   dict = DEFAULT_DICTIONARY_FILE;
//	}

  int x, y;
  int x_org, y_org; //used for clearing screen
  getyx(stdscr, y, x);
  getyx(stdscr, y_org, x_org); //used for clearing screen

  bool cap = false;
  if (word[0] >= 65 && word[0] <= 90)
    cap = true;

  printw("\nHere are suggestions to automplete \"%s\"\n", word);

  // Generate the lowercase version of the word
  char* lWord = malloc(DEFAULT_MAX_PREF_LEN * sizeof(char));
  strcpy(lWord, word);
  for(int i = 0; lWord[i] != '\0'; i++) {
      lWord[i] = tolower(lWord[i]);
  }
  log_debug("about to retrieve children of'%s'", lWord);
  char **children = get_n_children_in_dict(lWord, d, maxCompletions);

  // In order to restrict the number of options printed, change "num_children" to
  // some number that was inputed
  int num_children = num_children_in_dict(lWord, d);
  //log_debug("exited num_children: %d", num_children);
  // Stores the portion of the child that comes after the typed prefix_t
  char** partialChildren = malloc(num_children*sizeof(char));

  int i;
  int childrenToDisplay;
  if (maxCompletions > num_children) {
    childrenToDisplay = num_children;
  } else {
    childrenToDisplay = maxCompletions;
  }
  for (i = 0; i < childrenToDisplay; i++) {

    //create pointer to "the rest" of the word
    partialChildren[i] = malloc(strlen(children[i]) + 1);
    strcpy(partialChildren[i], children[i]);
    partialChildren[i] += (strlen(word) * sizeof(char));
    //prints the word, as entered, plus the 'rest' from the dictionary
    printw("%d: %s%s\n", i, word, partialChildren[i]);
  }
  if (num_children > maxCompletions)
    printw("Printed [%d] completions out of [%d] available\n", maxCompletions, num_children);

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
      if (moved < childrenToDisplay) {
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
  printw("%s%s", word, partialChildren[moved-1]);
  clrtobot();

  if (cap == true)
    children[moved-1][0] -= ('a' - 'A');

  return children[moved-1];

}

// command to enter interactive autocomplete mode
int lets_tab_builtin(char **args)
{
  log_debug("lets_tab_builtin: ENTER FUNC");
  int amountOfArgs;
	for (amountOfArgs = 0; args[amountOfArgs] != NULL; amountOfArgs++);
  log_trace("lets_tab_builtin: amount0fArgs for loop (%d)", amountOfArgs);

  struct word *word = NULL; //list

	//bool server = false;
//	char *dictionary = malloc(100* sizeof(char *));
//        strcpy(dictionary, "default"); //initialized to default dictionary
//        log_trace("lets_tab_builtin: successfully initialized dictionary to '%s'", dictionary);
//	if (args[0] != NULL) {
//  		if (strncmp(args[0], "-s", 2) != 0) {
//			dictionary = args[1];
//		}
//	}
 //       log_trace("lets_tab_builtin: dictionary is now set to '%s'", dictionary);
        int msg;
        dict_t *d;

                 // Initialize dictionary, declare names of files to be used
    //    if (strcmp(dictionary, "default") == 0) {
    //        log_trace("lets_tab_builtin: use default dictionary");
    //        d = dict_official();

      //      if (d == NULL) {
      //          msg = EXIT_FAILURE;
      //      } else {
      //          msg = EXIT_SUCCESS;
      //      }

        //} else {
            log_trace("lets_tab_builtin: use custom dictionary");
            d = dict_new();

            msg = dict_read(d, DEFAULT_DICTIONARY_FILE);
       // }

        if (msg == EXIT_FAILURE) {
            //shell_error("Invalid dictionary file input.", color);
            log_fatal("Invalid dictionary file input.");
            exit(0);
        }


  int length = 0;
  int total_length = 0;
  int start = 0;
  int c;
  int x, y;
  initscr();    // Start Curses Mode
  cbreak();
  noecho();
  if (start == 0) {
    printw("Welcome to the interactive autocomplete editor!\n");
    printw("While typing a word, press tab to display autocomplete possibilities.\n");
    printw("If you want to save your work to a text file, press ` .\n");
    printw("To exit interactive mode, press ~ . Press enter to begin!");
    int c_start;
    while (10 != (c_start = getch()))
      ;
    clear();
    start = 1;
  }

  while('~' != (c = getch())) {
    // Doesn't print tab, bkspace, or del
    if (c != 9 && c != 127 && c != 8 && c != 96) {
      printw("%c", c);
      word = ll_new(word);
      word->letter = c;
      length++;
      word->prefix_length = length;
      total_length++;
    }

    if (c == 32 || c == 10 || c == 11 || c == 13) {
      word->prefix_length = -1;
      length = 0;
    }

    if (c == 9 && length > 0) {

      char *wordTyped = malloc(sizeof(char)*(word->prefix_length+1));
      int i = word->prefix_length;
      wordTyped[word->prefix_length] = '\0';
      while (i != 0) {
        wordTyped[i-1] = word->letter;
        i--;
        word = ll_pop(word);
      }

		  int maxCompletions = -1;
			bool n_flag_found = false;
			int flag = 0;
			for (; flag < amountOfArgs; flag++) {
				if (args[flag] != NULL) {
					if (strncmp(args[flag], "-n", 2) == 0){
				    n_flag_found = true;
					  break;
					}
				}
			}

			if (n_flag_found == true)
			  if (args[flag+1] != NULL)
				  maxCompletions = atoi(args[flag+1]);
		  //if (amountOfArgs >= 3)
				//if(args[2] != NULL)
				  //maxCompletions = atoi(args[2]);
      char *complete_word = autocomplete(wordTyped, d, length, maxCompletions);
      length = 0;
      for (i = 0; complete_word[i] != '\0'; i++) {
        word = ll_new(word);
        word->letter = complete_word[i];
        length++;
        word->prefix_length = length;
        total_length++;
      }
    }

    if (c == 127 || c == 8) {
        getyx(stdscr, y, x);
        x--;
        move(y, x);
        clrtobot();
        refresh();
        word = ll_pop(word);
        if (total_length != 0)
          total_length--;
        if (word) {
           if (word->prefix_length == -1)
            length = 0;
          else
            length = word->prefix_length;
        }
        else
          length = 0;
    }
    

    // Pressing '`' saves the screen to a file
    if (c == 96) {
      getyx(stdscr, y, x);
      printw("\nenter your file name and press enter:\n");
      int c_0;
      int name_length = 0;
      int j;
      struct word *filename = NULL;
      // Gets the filename from user input
      while (10 != (c_0 = getch())) {
        // Cannot add space, forwardslash, or delete key to the filename
        if (c_0 != 32 && c_0 != 92 && c_0 != 127 && c_0 != 8) {
          printw("%c", c_0);
          name_length++;
          filename = ll_new(filename);
          filename->letter = c_0;
        }
        // Delete key functionality
        if (c_0 == 127 || c_0 == 8) {
          if (name_length != 0) {
            int x_0, y_0;
            getyx(stdscr, y_0, x_0);
            x_0--;
            move(y_0, x_0);
            clrtobot();
            refresh();
            filename = ll_pop(filename);
            name_length--;
          }
        }
      }
      char *complete_filename;
      // transfers the linked list to a string
      if (filename) {
        complete_filename = malloc(sizeof(char)*(name_length+1));
        complete_filename[name_length] = '\0';
        for (j=name_length-1; j >=0; j--) {
          complete_filename[j] = filename->letter;
          filename = ll_pop(filename);
        }
      }
      // If no filename is entered, saves to a generic file
      else {
        printw("no filename entered, saving to autocomplete_save.txt");
        complete_filename = "autocomplete_save.txt";
      }
      char screen[total_length];
      j = total_length;
      struct word *tmp;
      for (tmp = word; tmp != NULL; tmp = ll_next(tmp)) {
        screen[j] = tmp->letter;
        j--;
      }
      FILE *fp = fopen(complete_filename, "w");
      if (fp) {
        for (j = 1; j <= total_length; j++) {
          fprintf(fp, "%c", screen[j]);
        }
         fclose(fp);
        printw("\nfile save successful! press enter to continue");
        int c_1;
        while (10 != (c_1 = getch()))
          ;
        move(y, x);
        refresh();
        clrtobot();
        refresh();
      }

      else
        printw("could not open file, saving aborted");
    }

    cbreak();
  }
  clear();
  endwin();
  return(1);
}
