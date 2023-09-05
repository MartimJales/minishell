/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:22 by mjales            #+#    #+#             */
/*   Updated: 2023/09/04 23:04:23 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	free_tokens(vars()->tokens);
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
