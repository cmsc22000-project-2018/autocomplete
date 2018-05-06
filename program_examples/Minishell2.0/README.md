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
