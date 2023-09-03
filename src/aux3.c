#include "../inc/minishell.h"

extern int exit_status;

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
