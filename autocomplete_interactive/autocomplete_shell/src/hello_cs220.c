/*
Small program which prints "Hello CS220!" or "Hello World!" based on input
*/

#include "minishell.h"

static void print_statement(int which) // may need to be char
{
  if (which == 1)
    ft_putstr("Hello CS220!\n");
  else
    ft_putstr("Hello World!\n");
}

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

/*
Executes the hello command
*/

int hello_builtin(char **args)
{
  if (!args[0])
  {
    print_statement(0);
    return(1);
  }
  if (has_two_args(args))
    return(1);
  else
  {
    if(strcmp(args[0], "1") == 0)
      print_statement(1);
    else if (strcmp(args[0], "0") == 0)
      print_statement(0);
    else
      print_statement(0);
    return(1);
  }
}
