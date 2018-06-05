/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strendswith.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrameau <jrameau@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/17 20:56:49 by jrameau           #+#    #+#             */
/*   Updated: 2017/04/19 22:54:50 by jrameau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strendswith(char *s1, char *s2)
{
    int		i;

    i = -1;
    while (s1[++i])
        if (s1[i] == s2[0])
            if (ft_strcmp(s1 + i, s2) == 0)
                return (1);
    return (0);
}