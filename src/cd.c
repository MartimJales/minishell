#include "../inc/minishell.h"

extern int exit_status;

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

int exec_cd(struct execcmd *ecmd)
{
	char	old_path[1024];
	char	new_path[1024];

	if (ecmd->argv[2])
	{
		exit_status = 1;
		fprintf(stderr, "cd: too many arguments\n");
		return (1);
	}
	if (ecmd->argv[1] == NULL || *ecmd->argv[1] == '~')
		ecmd->argv[1] = strdup(expand_var(vars()->envp, "HOME"));
	else if (*ecmd->argv[1] == '-')
		ecmd->argv[1] = strdup(expand_var(vars()->envp, "OLDPWD"));
	if (getcwd(old_path, sizeof(old_path)) == NULL) 
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(ecmd->argv[1]) != 0)
	{
		exit_status = 1;
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