/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:28:03 by mjales            #+#    #+#             */
/*   Updated: 2023/08/03 20:04:11 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	create_sc(void)
{
	vars()->sc = malloc(sizeof(char *) * 14);
	vars()->sc[0] = "#";
	vars()->sc[1] = "$";
	vars()->sc[2] = "&";
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

size_t	is_delimiter(const char *s, char **sc, size_t *len)
{
	for (size_t i = 0; i < 14; i++)
	{
		*len = strlen(sc[i]);
		if (strncmp(s, sc[i], *len) == 0)
			return (1);
	}
	*len = 0;
	return (0);
}

size_t	next_pos_sc(char const *s, char **sc)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (s[i])
	{
		if (is_delimiter(&s[i], sc, &len))
			return (i);
		if (len == 0)
			i++;
		else
			i += len;
	}
	return (i);
}

size_t	string_number_sc(char const *s, char **sc)
{
	size_t	i;
	size_t	num;
	size_t	flag;
	size_t	len;

	flag = 0;
	i = 0;
	num = 0;
	while (s[i])
	{
		if (is_delimiter(&s[i], sc, &len))
		{
			flag = 0;
			i += len;
			continue;
		}
		if (!flag)
			num++;
		flag = 1;
		if (len == 0)
			i++;
		else
			i += len;
	}
	return (num);
}

char	**split_sc(char const *s, char **sc)
{
	size_t	num;
	size_t	i;
	size_t	len;
	char	**arr;

	num = string_number_sc(s, sc);
	arr = malloc(sizeof(char *) * (num + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (i < num)
	{
		while (is_delimiter(s, sc, &len))
			s += len;
		len = next_pos_sc(s, sc);
		arr[i] = malloc(len + 1);
		ft_strlcpy(arr[i], s, len + 1);
		s = s + len;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

