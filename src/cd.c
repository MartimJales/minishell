/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:28 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 20:11:16 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*expand_var(char **envp, char *var)
{
	int	var_len;
	int	i;

	var_len = ft_strlen(var);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
			return (envp[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

char	*parse_cd_args(struct s_execcmd *ecmd)
{
	if (ecmd->argv[2])
	{
		g_exit_status = 1;
		write(2, "cd: too many arguments\n", 23);
		return (NULL);
	}
	if (ecmd->argv[1] == NULL || *ecmd->argv[1] == '~')
		return (ft_strdup(expand_var(vars()->envp, "HOME")));
	else if (*ecmd->argv[1] == '-')
		return (ft_strdup(expand_var(vars()->envp, "OLDPWD")));
	return (ecmd->argv[1]);
}

int	exec_cd(struct s_execcmd *ecmd)
{
	char	old_path[1024];
	char	new_path[1024];
	char	*path;

	path = parse_cd_args(ecmd);
	if (!path)
		return (1);
	if (getcwd(old_path, sizeof(old_path)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(path) != 0)
	{
		g_exit_status = 1;
		perror("cd");
	}
	if (getcwd(new_path, sizeof(new_path)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	update_var_to_envp("OLDPWD", old_path);
	update_var_to_envp("PWD", new_path);
	return (0);
}

void	process_input(void)
{
	while (1)
	{
		vars()->s = initialize_and_read_input();
		if (vars()->s == NULL)
			break ;
		process_and_execute_input(vars()->s);
	}
}

char	**duplicate_envp(char **envp)
{
	int		envp_size;
	int		i;
	char	**new_envp;

	envp_size = 0;
	while (envp[envp_size] != NULL)
		envp_size++;
	new_envp = (char **)malloc((envp_size + 1) * sizeof(char *));
	i = 0;
	while (envp[i] != NULL)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			perror("ft_strdup");
			exit(1);
		}
		i++;
	}
	new_envp[envp_size] = NULL;
	return (new_envp);
}
