/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrameau <jrameau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 05:44:00 by jrameau           #+#    #+#             */
/*   Updated: 2017/05/21 01:01:53 by jrameau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Runs the binary file by creating a new process and handles signals in case
** of interruptions then returns whether it executed successfully or not
**
** @param		path	The path to the binary file
** @param		args	The arguments to pass to the system command
** @return		-1 on failure, 1 on success
*/

static int		run_cmd(char *path, char **args)
{
    pid_t	pid;

    pid = fork();
    signal(SIGINT, proc_signal_handler);
    if (pid == 0)
        execve(path, args, g_envv);
    else if (pid < 0) {
        free(path);
        ft_putendl("Fork failed to create a new process.");
        return (-1);
    }
    wait(&pid);
    if (path)
        free(path);
    return (1);
}

/*
** Checks if the first word in the input is one of the builtin commands, if it
** is, it executes it and returns -1, 0, or 1
**
** @param		command		The array of strings that contains the command
** @return		-1 on exit, 0 if it's not a builtin, 1 otherwise
*/

static int		check_builtins(char **command)
{
    if (ft_strequ(command[0], "exit"))
        return (-1);
    else if (ft_strequ(command[0], "interactive"))
        return (lets_tab_builtin(command + 1));
    else if (ft_strequ(command[0], "batch"))
        return (batch_mode_builtin(command + 1));
    return (0);
}

/*
** Executes a command and prints the result on the screen then returns whether
** there was a an exit or not
** The steps are:
**		1. Check if it's a builtin command, if not...
**		2. Check if it's a system command, if not ...
**		3. Check if it's a folder or an executable, if not...
**		4. Display an error (not found) message.
**
** @param		command		The command to execute
** @return		-1 if there was an interruption (exit) or 0/1 if not
*/

int				exec_command(char **command)
{
    struct stat	f;
    int			is_builtin;

    if ((is_builtin = check_builtins(command)) == 1)
        return (0);
    if (is_builtin < 0)
        return (-1);
    if (lstat(command[0], &f) != -1) {
        if (f.st_mode & S_IXUSR)
            return (run_cmd(ft_strdup(command[0]), command));
    }
    ft_putstr("That is not a command!\n");
    return (0);
}
