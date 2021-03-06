/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrameau <jrameau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/24 04:17:47 by jrameau           #+#    #+#             */
/*   Updated: 2017/05/21 01:04:34 by jrameau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Parses a string to convert variables to their value then returns the parsed
** string
**
** @param		str		The input string to parse
** @param		pos		The position from which to start in the string
*/

static char		*parse_env_var(char *str, int pos)
{
    char	*value;
    char	*key;
    char	c;

    key = ft_strnew(1);
    while (str[pos] && !IS_SPACE(str[pos])) {
        c = str[pos];
        key = ft_strjoinchcl(key, c);
        pos++;
    }
    if (!(value = get_env_var(key))) {
        free(key);
        return (NULL);
    }
    free(key);
    return (value);
}

/*
** Parses the input by changing $VAR_NAME to the value of VAR_NAME in the
** environment variable or by nothing if it doesn't exist and by changing ~
** to the value of the user's home path then returns the parsed string
**
** @param		input		The input string
** @return		The parsed string
*/

static char		*parse_input(char *input)
{
    int		i;
    char	*new;

    i = -1;
    new = ft_strnew(1);
    while (input[++i]) {
        if (input[i] == '$' && input[i + 1]) {
            new = ft_strjoincl(new, parse_env_var(input, i + 1), 0);
            while (input[i + 1] && !IS_SPACE(input[i + 1]) &&
                    input[i + 1] != '$')
                i++;
        } else
            new = ft_strjoinchcl(new, input[i]);
    }
    free(input);
    return (new);
}

/*
** Displays a prompt on the screen and fills the input character by character
** then adds it to the referenced variable (input) after parsing the whole
** input if necessary
** TODO: Not the most efficient way, will improve it later
**
** @param	input	The address of the variable to fill with the parsed input
** @return	N/A
*/

static void		get_input(char **input)
{
    int		ret;
    char	buf;
    int		i;
    int		count;

    *input = ft_strnew(1);
    count = 1;
    i = 0;
    while ((ret = read(0, &buf, 1)) && buf != '\n') {
        *(*input + i++) = buf;
        *input = ft_realloc(*input, count, count + 1);
        count++;
    }
    *(*input + i) = '\0';
    if (!ret) {
        free(*input);
        exit_shell();
    }
    if ((ft_strchr(*input, '$') != NULL) || (ft_strchr(*input, '~') != NULL))
        *input = parse_input(*input);
}

/*
** Takes care of multiple commands in the input
**
** @param		commands	The list of commands to execute
** @return		-1 if there was an interruption from one of the commands
**				or 0/1 if not
*/

int				exec_commands(char **commands)
{
    int		i;
    int		ret;
    char	**command;

    i = -1;
    ret = 0;
    while (commands[++i]) {
        command = ft_strsplitall(commands[i]);
        ret = exec_command(command);
        ft_freestrarr(command);
        if (ret == -1)
            break ;
    }
    return (ret);
}

/* Concatenates array of strings into single string */

char *concatenate(size_t size, char *array[size], const char *joint)
{
    size_t jlen, lens[size];
    size_t i, total_size = (size-1) * (jlen=strlen(joint)) + 1;
    char *result, *p;


    for(i=0; i<size; ++i) {
        total_size += (lens[i]=strlen(array[i]));
    }
    p = result = malloc(total_size);
    for(i=0; i<size; ++i) {
        memcpy(p, array[i], lens[i]);
        p += lens[i];
        if(i<size-1) {
            memcpy(p, joint, jlen);
            p += jlen;
        }
    }
    *p = '\0';
    return result;
}

/*
** Initializes minishell
**
** @param	ac		argument counts
** @param	av		argument variables
** @param	envv	environment variables
** @return	0 on completion
*/

int				main(int ac, char **av, char **envv)
{
    char	*input;
    int		ret;
    char	**commands;
    int firstCommand = 0;
    int shouldExit = 0;

    if (ac == 1) {
        ac = 3;
        char *defaultArgs[ac];
        defaultArgs[0] = av[0];
        defaultArgs[1] = "-i";
        av = defaultArgs;
        firstCommand = 0;
    } else if (!strncmp(av[1], "-i", 2)) {
        if (av[2] == NULL) {
            ac = 3;
            char *defaultArgs[ac];
            defaultArgs[0] = av[0];
            defaultArgs[1] = "-i";
            av = defaultArgs;
        }
        firstCommand = 0;
        //This currently doesn't trigger anything in particular, eventually this flag will be required
    }
    //Batch mode
    else if (!strncmp(av[1], "-b", 2)) {
        firstCommand = 1;
    }
    //Help 'mode'
    else if (!strncmp(av[1], "-h", 2) || !strncmp(av[1], "help", 4)) {
        firstCommand = 2;
        printf("For full documentation and help, and details on batch mode, please see the readme at \nhttps://github.com/cmsc22000-project-2018/autocomplete \n");
        printf("To get started with interactive mode, just type ./autocomplete\n");
        shouldExit = 1;
    }

    init_envv(ac, av, envv);
    while (1) {
        if (firstCommand == 0) {
            // Jonas 05.17: Edited to take in more than one argument
            //display_prompt_msg();
            char* interactive = malloc(sizeof(char) * 13);
            strcpy(interactive, "interactive");
            char* arguments = concatenate(ac - 2, &(av[2]), " ");
            input = malloc(strlen(interactive) + strlen(arguments) + 1);
            strcat(input, interactive);
            strcat(input, " ");
            input = parse_input(strcat(input, arguments));
            firstCommand+=2;
        } else if (firstCommand == 1) {
            char* batch = malloc(sizeof(char) * 6);
            strcpy(batch, "batch");
            char* arguments = concatenate(ac - 2, &(av[2]), " ");
            input = malloc(strlen(batch) + strlen(arguments) + 1);
            strcat(input, batch);
            strcat(input, " ");
            input = parse_input(strcat(input, arguments));
            firstCommand+=2;
            shouldExit = 1;
        } else if (shouldExit == 1) {
            input = malloc(sizeof(char)*5);
            strcpy(input, "exit");
            input = parse_input(input);
        } else {
            display_prompt_msg();
            signal(SIGINT, signal_handler);
            get_input(&input);
        }
        if (ft_isemptystr(input, 1)) {
            free(input);
            continue ;
        }
        commands = ft_strsplit(input, ';');
        free(input);
        ret = exec_commands(commands);
        ft_freestrarr(commands);
        if (ret == -1)
            break ;
        firstCommand++;
    }
    ft_freestrarr(g_envv);
    return (0);
}
