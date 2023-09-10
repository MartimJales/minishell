/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:01 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 20:21:00 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_pipe(void)
{
	if (pipe(vars()->pipefd) == -1)
	{
		perror("Error creating pipe");
		exit(EXIT_FAILURE);
	}
}

void	write_to_pipe_and_cleanup(char *mi)
{
	write(vars()->pipefd[1], mi, ft_strlen(mi));
	free(mi);
}

char	*append_to_multiline(char *mi, char *input_buffer)
{
	char	*temp;

	if (mi)
	{
		temp = ft_realloc(mi, ft_strlen(mi) + ft_strlen(input_buffer) + 2);
		mi = temp;
		mi = junta_strings(mi, input_buffer);
		mi = junta_strings(mi, "\n");
	}
	else
	{
		mi = ft_strdup(input_buffer);
		if (mi == NULL)
		{
			perror("Error allocating memory");
			free(input_buffer);
			return (NULL);
		}
		mi = junta_strings(mi, "\n");
	}
	return (mi);
}

void	signal_hd(int sig)
{
	if (sig == 2 || sig == SIGQUIT)
	{
		clear_history();
		exit (1);
	}
}

void	heredoc_signals(void)
{
	signal(SIGINT, signal_hd);
	signal(SIGQUIT, signal_hd);
}
