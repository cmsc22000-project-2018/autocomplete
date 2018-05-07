 /*
** SCRIPT EXAMPLE BY DAVID YAFFE
** The minishell utilized here can be found at: https://github.com/R4meau/minishell
** This shell was modified to add additional command(s) to the shell functionality
*/

The README.md for minishell itself has been kept in-tact for instructions on how to use the shell.

In order to install and run the shell run make from the minishell directory and execute with ./minishell

COMMAND ADDED:
autocomplete

Files added/edited in the minishell directory:
inc/minishell.h
src/exec_command.c
src/lets_tab.c

SHELL MODIFIED:
- I've modified the shell to a ">>>: " line ending by modifying display src/display_prompt_msg.c

AUTOCOMPLETE explained:
- The autocomplete uses getch() to delay execution of the code, in an interactive mode. This is accomplished by using the ncurses.h built-in library.
- When the mode is entered, every character that is typed is printed back onto the screen.
- When the character captured by getch() is a tab, I have set it to execute a given piece of code. I have acreated a dummy autocomplete function which doesn't even attempt to autocomplete, but shows the capability to.
- Right now my autocomplete function merely takes in a string, and gives as many suggestions are there are characters in that word, adding a, b, c, etc. onto it.
- Exit interactive mode by pressing ~


Known Bugs:
- The interactive mode starts to mess up once the word suggestions reaches the end of the screen

Jonas Ciplickas edits, 05/06

lets_tab_bultin function in lets_tab.c modified slightly:
-only keeps track of the most recent word, so you can type in a series of words and only autocomplete the last one
-doesn't print tab keys when getch() returns a tab key

autocomplete function in lets_tab.c modified:
-displays 10 completions
-user can select a completion using 0-9 keys
-removes autocomplete prompt from screen after selection
-doesn't actually autocomplete anything yet, but that will easy to implement when we get the tries

known bugs - closed.
-in order to properly autocomplete a word, need to enter in an extra character - ie, in order to autocomplete "hello", you need to enter "hello,". This is a result of the lines of code that stop tab from being printed, and I have no idea how to fix that issue EDIT 9:00PM 05/06: THIS BUG IS FIXED.

