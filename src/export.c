/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:53 by mjales            #+#    #+#             */
/*   Updated: 2023/09/09 03:49:19 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	add_variable_to_envp(char *new_var)
{
	int		i;
	int		new_envp_size;
	char	**new_envp;

	new_envp_size = 0;
	while (vars()->envp[new_envp_size] != NULL)
		new_envp_size++;
	new_envp = (char **)malloc((new_envp_size + 2) * sizeof(char *));
	if (!new_envp)
		panic("malloc", 1);
	i = 0;
	while (i < new_envp_size)
	{
		new_envp[i] = ft_strdup(vars()->envp[i]);
		if (!new_envp[i])
			panic("ft_strdup", 1);
		i++;
	}
	new_envp[new_envp_size] = ft_strdup(new_var);
	new_envp[new_envp_size + 1] = NULL;
	free_envp(vars()->envp);
	vars()->envp = new_envp;
}

void	update_var_to_envp(char *var, char *new_value)
{
	char	**envp;
	int		index;

	index = 0;
	envp = vars()->envp;
	while (envp[index] != NULL)
	{
		if (ft_strncmp(envp[index], var, ft_strlen(var)) == 0 && \
			envp[index][ft_strlen(var)] == '=')
		{
			free(envp[index]);
			envp[index] = junta_strings(ft_strdup(var), "=");
			envp[index] = junta_strings(envp[index], new_value);
			if (!envp[index])
			{
				perror("malloc");
				exit(1);
			}
			break ;
		}
		index++;
	}
}

int	clean_var(char **var, int status)
{
	int	i;

	i = 0;
	while (var[i])
	{
		free(var[i]);
		i++;
	}
	free(var);
	return (safe_exit(status));
}

void	clean_var2(char **var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		free(var[i]);
		i++;
	}
	free(var);
}

int	exec_export(struct s_execcmd *ecmd)
{
	int		i;
	char	**var;

	i = 0;
	if (ecmd->argv[1] == NULL)
		return (exec_env(1));
	while (ecmd->argv[++i] != NULL)
	{
		var = ft_split(ecmd->argv[i], '=');
		if (!check_alnum(var[0]))
		{
			write(2, " not a valid identifier\n", \
			ft_strlen(" not a valid identifier\n"));
			return (clean_var(var, EXIT_FAILURE));
		}
		if (validate_format(ecmd->argv[i]))
		{
			if (var_exists(vars()->envp, var[0]))
				update_var_to_envp(var[0], var[1]);
			else
				add_variable_to_envp(ecmd->argv[i]);
		}
		clean_var2(var);
	}
	return (safe_exit(EXIT_SUCCESS));
}
