#include "../inc/minishell.h"

extern int exit_status;

t_vars	*vars(void)
{
	static t_vars	var;

	return (&var);
}

t_elems	*elems(void)
{
	static t_elems	elem;

	return (&elem);
}

void	setup_signals(void)
{
	signal(SIGINT, signal_cmd);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup(void)
{
	free(vars()->s);
	free_tokens();
}

int	is_special(const char *str, char **special)
{
	int	i;

	i = 0;
	while (i < vars()->num_sc)
	{
		if (strncmp(str, vars()->sc[i], strlen(vars()->sc[i])) == 0)
		{
			*special = vars()->sc[i];
			return (strlen(vars()->sc[i]));
		}
		i++;
	}
	return (0);
}
