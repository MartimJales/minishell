/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:07:00 by mjales            #+#    #+#             */
/*   Updated: 2023/09/08 23:08:22 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	delete_var_from_envp(char *var)
{
	char	**envp;
	int		index;
	int		i;

	index = 0;
	envp = vars()->envp;
	while (envp[index] != NULL)
	{
		if (ft_strncmp(envp[index], var, ft_strlen(var)) == 0 && \
		envp[index][ft_strlen(var)] == '=')
		{
			free(envp[index]);
			i = index;
			while (envp[i] != NULL)
			{
				envp[i] = envp[i + 1];
				i++;
			}
			break ;
		}
		index++;
	}
}

int	var_exists(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0 && \
envp[i][ft_strlen(var)] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	exec_unset(struct s_execcmd *ecmd)
{
	int	i;

	i = 1;
	while (ecmd->argv[i] != NULL)
	{
		if (var_exists(vars()->envp, ecmd->argv[i]))
		{
			delete_var_from_envp(ecmd->argv[i]);
		}
		i++;
	}
	g_exit_status = 0;
	if (vars()->forked)
	{
		exit(0);
	}
	return (0);
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	if (i < n && src[i] == '\0')
	{
		while (dest[i] != '\0')
		{
			dest[i] = '\0';
			++i;
		}
	}
	return (dest);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	ft_memcpy(new_ptr, ptr, size);
	return (new_ptr);
}
