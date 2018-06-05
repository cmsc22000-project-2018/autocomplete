/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrameau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/22 18:58:25 by jrameau           #+#    #+#             */
/*   Updated: 2016/09/24 03:16:42 by jrameau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
    int			i;
    int			j;

    i = -1;
    j = (int)ft_strlen(s1);
    while (*(s2 + ++i) && i < (int)n)
        *(s1 + j++) = *(s2 + i);
    *(s1 + j) = '\0';
    return (s1);
}