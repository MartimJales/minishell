#include "../inc/minishell.h"

extern int exit_status;

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
		new_envp[i] = strdup(vars()->envp[i]);
		if (!new_envp[i])
			panic("strdup", 1);
		i++;
	}
	new_envp[new_envp_size] = strdup(new_var);
	new_envp[new_envp_size + 1] = NULL;
	vars()->envp = new_envp;
}

int	validate_format(const char *input)
{
	const char	*equal_sign;
	size_t		var_length;
	int			i;
	char		*var;

	equal_sign = strchr(input, '=');
	if (equal_sign == NULL || equal_sign == input)
	{
		return (0);
	}
	var_length = equal_sign - input;
	var = malloc(var_length + 1);
	strncpy(var, input, var_length);
	var[var_length] = '\0';
	i = 0;
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
			return (0);
		i++;
	}
	return (1);
}

void	update_var_to_envp(char *var, char *new_value)
{
	char	**envp;
	int		index;

	index = 0;
	envp = vars()->envp;
	while (envp[index] != NULL)
	{
		if (strncmp(envp[index], var, strlen(var)) == 0 &&
			envp[index][strlen(var)] == '=')
		{
			free(envp[index]);
			envp[index] = (char *)malloc(strlen(new_value));
			if (!envp[index])
			{
				perror("malloc");
				exit(1);
			}
			envp[index] = strdup(new_value);
			break ;
		}
		index++;
	}
}

int	exec_export(struct execcmd *ecmd)
{
	int		i;
	char	*var;

	i = 1;
	if (ecmd->argv[1] == NULL)
		return (exec_env(1));
	while (ecmd->argv[i] != NULL)
	{
		var = ft_split(ecmd->argv[i], '=')[0];
		if (!check_alnum(var))
		{
			write(2, " not a valid identifier\n", \
			ft_strlen(" not a valid identifier\n"));
			return (safe_exit(EXIT_FAILURE));
		}
		if (validate_format(ecmd->argv[i]))
		{
			if (var_exists(vars()->envp, var))
				update_var_to_envp(var, ecmd->argv[i]);
			else
				add_variable_to_envp(ecmd->argv[i]);
		}
		i++;
	}
	return (safe_exit(EXIT_SUCCESS));
}

int	exec_env(int declare)
{
	int	i;

	i = 0;
	while (vars()->envp[i] != NULL)
	{
		if (declare)
			printf("declare -x %s=\"%s\"\n", \
ft_split(vars()->envp[i], '=')[0], strchr(vars()->envp[i], '=') + 1);
		else
			printf("%s\n", vars()->envp[i]);
		i++;
	}
	exit_status = 0;
	if (vars()->forked)
		exit(0);
	return (0);
}