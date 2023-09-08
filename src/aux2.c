/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:02 by mjales            #+#    #+#             */
/*   Updated: 2023/09/08 16:02:57 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	is_delimiter(const char *s, char **sc, size_t *len)
{
	size_t	i;

	i = 0;
	while (i < 14)
	{
		*len = ft_strlen(sc[i]);
		if (ft_strncmp(s, sc[i], *len) == 0)
			return (1);
		i++;
	}
	*len = 0;
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	size1;
	size_t	size2;
	char	*new;

	size1 = ft_strlen((char *)s1);
	size2 = ft_strlen((char *)s2);
	new = malloc(size1 + size2 + 1);
	if (!new)
		return (NULL);
	i = -1;
	while (++i < size1)
		new[i] = s1[i];
	while (i < size1 + size2)
	{
		new[i] = s2[i - size1];
		i++;
	}
	new[i] = 0;
	return (new);
}

int	ft_strcmp(char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while ((*s1 || *s2) && n > 0)
	{
		if (*s1 != *s2)
		{
			return ((unsigned char)*s1 - (unsigned char)*s2);
		}
		n--;
		s1++;
		s2++;
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	len;
	char	*new;

	len = ft_strlen((char *)s);
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	i = -1;
	while (s[++i])
		new[i] = s[i];
	new[i] = 0;
	return (new);
}
