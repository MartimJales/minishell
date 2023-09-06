/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:28 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:18:18 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*expand_var(char **envp, char *var)
{
	int	var_len;
	int	i;

	var_len = strlen(var);
	i = 0;
	while (envp[i] != NULL)
	{
		if (strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
			return (envp[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

char	*parse_cd_args(struct execcmd *ecmd)
{
	if (ecmd->argv[2])
	{
		g_exit_status = 1;
		fprintf(stderr, "cd: too many arguments\n");
		return (NULL);
	}
	if (ecmd->argv[1] == NULL || *ecmd->argv[1] == '~')
		return (strdup(expand_var(vars()->envp, "HOME")));
	else if (*ecmd->argv[1] == '-')
		return (strdup(expand_var(vars()->envp, "OLDPWD")));
	return (ecmd->argv[1]);
}

int	exec_cd(struct execcmd *ecmd)
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
	update_var_to_envp("OLDPWD", strdup(old_path));
	update_var_to_envp("PWD", strdup(new_path));
	return (0);
}
