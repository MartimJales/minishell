/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:00:05 by mjales            #+#    #+#             */
/*   Updated: 2023/06/06 11:28:43 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t	next_pos(char const *s, int c)
{
	size_t	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (i);
	return (i);
}

size_t	string_number(char const *s, int c)
{
	size_t	i;
	size_t	num;
	size_t	flag;

	flag = 0;
	i = -1;
	num = 0;
	while (s[++i])
	{
		if (s[i] != c)
		{
			if (!flag)
				num++;
			flag = 1;
		}
		else
			flag = 0;
	}
	return (num);
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

char	*junta_strings(char *s1, char *s2)
{
	char	*buffer;

	buffer = malloc(ft_strlen(s1) + 1);
	ft_strlcpy(buffer, s1, ft_strlen(s1) + 1);
	free(s1);
	s1 = ft_strjoin(buffer, s2);
	free(buffer);
	return (s1);
}

//char	**ft_split(char const *s, int c)
//{
//	size_t	num;
//	size_t	i;
//	size_t	len;
//	char	**arr;

//	num = string_number(s, ' ');
//	arr = malloc(sizeof(char *) * (num + 1));
//	if (!arr)
//		return (NULL);
//	i = -1;
//	while (++i < num)
//	{
//		while (*s == c)
//			s++;
//		len = next_pos(s);
//		arr[i] = malloc(len + 1);
//		ft_strlcpy(arr[i], s, len + 1);
//		s = s + len + 1;
//	}
//	arr[i] = NULL;
//	return (arr);
//}

char	**lexer(char *s)
{
	(void)s;
	return NULL;
}
