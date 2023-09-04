#include "../inc/minishell.h"

extern int exit_status;

size_t is_delimiter(const char *s, char **sc, size_t *len)
{
	size_t	i;

	i = 0;
	while (i < 14)
	{
		*len = strlen(sc[i]);
		if (strncmp(s, sc[i], *len) == 0)
			return (1);
		i++;
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
			continue ;
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