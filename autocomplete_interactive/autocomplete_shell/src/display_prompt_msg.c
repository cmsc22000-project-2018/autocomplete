/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_prompt_msg.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrameau <jrameau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 04:49:25 by jrameau           #+#    #+#             */
/*   Updated: 2017/05/21 01:04:42 by jrameau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Kills the shell gracefully
**
** @param		N/A
** @return		N/A
*/

void	exit_shell(void)
{
	ft_freestrarr(g_envv);
	write(1, "\n", 1);
	exit(0);
}

void	display_prompt_msg(void)
{
	//char	*cwd;
	//char	buff[4096 + 1];
	//char	*parsed_cwd;

	//cwd = getcwd(buff, 4096);
	//parsed_cwd = parse_home_path(cwd, 0);
	//ft_putstr(parsed_cwd);
	//free(parsed_cwd);
	ft_putstr("AUTOCOMPLETE Instructions: interactive <flags> || batch <flags> || exit\n");
	ft_putstr(">>>: ");
        //ft_putstr(" \033[31m︻\033[0m\033[32m┳\033[0m\033[33mデ");
	//ft_putstr("\033[0m\033[34m═\033[0m\033[35m—\033[0m$ ");
}
