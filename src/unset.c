#include "../inc/minishell.h"

extern int exit_status;

void delete_var_from_envp(char *var)
{
	char	**envp;
	int		index;
	int		i;

	index = 0;
	envp = vars()->envp;
	while (envp[index] != NULL)
	{
		if (strncmp(envp[index], var, strlen(var)) == 0 &&
			envp[index][strlen(var)] == '=')
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
		if (strncmp(envp[i], var, strlen(var)) == 0 && \
envp[i][strlen(var)] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	exec_unset(struct execcmd *ecmd)
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
	exit_status = 0;
	if (vars()->forked)
	{
		exit(0);
	}
	return (0);
}