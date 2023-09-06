/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:28:03 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:16:37 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	create_sc(void)
{
	vars()->sc = malloc(sizeof(char *) * 14);
	vars()->sc[0] = "#";
	vars()->sc[1] = "$";
	vars()->sc[2] = "&&";
	vars()->sc[3] = "*";
	vars()->sc[4] = "@";
	vars()->sc[5] = "\\";
	vars()->sc[6] = ":";
	vars()->sc[7] = ";";
	vars()->sc[8] = "<<";
	vars()->sc[9] = ">>";
	vars()->sc[10] = "|";
	vars()->sc[11] = "~";
	vars()->sc[12] = "<";
	vars()->sc[13] = ">";
}

int	all_space(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != 32 && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (src[i] && i < (size - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}

int	ft_isalnum(int c)
{
	if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c < '0' || c > '9'))
		return (0);
	return (1);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!little[0])
		return ((char *) big);
	if (!little)
		return (NULL);
	while (big[i] && i < len)
	{
		j = 0;
		while (little[j] == big[i + j] && (i + j < len) && big[i + j])
			j++;
		if (!little[j])
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
