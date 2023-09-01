/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:28:03 by mjales            #+#    #+#             */
/*   Updated: 2023/09/01 12:35:30 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int exit_status;

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

int is_builtin(char *cmd)
{
	if (strcmp("export", cmd) == 0)
		return (1);
	if (strcmp("cd", cmd) == 0)
		return (1);
	if (strcmp("unset", cmd) == 0)
		return (1);
	if (strcmp("env", cmd) == 0)
		return (1);
	if (strcmp("exit", cmd) == 0)
		return (1);
	return (0);
}

char	**check_path(t_list *args, char **path_arg)
{
	char	**teste;
	char	*buffer;
	int		i;

	teste = list_to_array(args);
	i = 0;
	// Tenho que alterar isto para ./ e outras exceções
	if (is_builtin(teste[i]) || teste[i][0] == '/')
		return (teste);
	while (path_arg[i])
	{
		buffer = malloc(ft_strlen(path_arg[i]) + 1);
		ft_strlcpy(buffer, path_arg[i], ft_strlen(path_arg[i]) + 1);
		buffer = junta_strings(buffer, teste[0]);
		if (access(buffer, F_OK) == 0)
		{
			free(teste[0]);
			teste[0] = malloc(ft_strlen(buffer) + 1);
			ft_strlcpy(teste[0], buffer, ft_strlen(buffer) + 1);
			free(buffer);
			return (teste);
		}
		i++;
		free(buffer);
	}
	return (teste);
}

char	**create_path(char *envp[])
{
	char	*path;
	char	**path_arg;
	int		i;

	i = 0;
	while (envp[i])
	{
		path = ft_strnstr(envp[i], "PATH=", 5);
		if (path)
			break ;
		i++;
	}
	path_arg = ft_split(path + 5, ':');
	i = -1;
	while (path_arg[++i])
		path_arg[i] = junta_strings(path_arg[i], "/");
	return (path_arg);
}

void	signal_cmd(int sig)
{
	if (sig == 2)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
		exit (1);
	}
}

char *ft_itoa(int nbr)
{
	int		n;
	int		len;
	char	*result;

	n = nbr;
	if (nbr == -2147483648)
		return ("-2147483648\0");
	len = 0;
	if (nbr <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (result == NULL)
		return (NULL);
	result[len] = '\0';
	if (nbr == 0)
	{
		result[0] = '0';
		return (result);
	}
	if (nbr < 0)
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr)
	{
		result[--len] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (result);
}

long long int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}
