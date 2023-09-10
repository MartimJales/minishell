/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:01 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 18:18:31 by mjales           ###   ########.fr       */
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
		//rl_on_new_line();
		//rl_replace_line("", 0);
		clear_history();
		exit (1);
	}
}

void	heredoc_signals(void)
{
	signal(SIGINT, signal_hd);
	signal(SIGQUIT, signal_hd);
}

void	heredoc(const char *delimiter)
{
	char	*input_buffer;
	char	*mi;

	input_buffer = NULL;
	mi = NULL;
	init_pipe();
	while (1)
	{
		heredoc_signals();
		input_buffer = readline(">");
		if (!input_buffer)
			break ;
		if (ft_strcmp(input_buffer, delimiter) == 0)
		{
			if (mi)
				write_to_pipe_and_cleanup(mi);
			free(input_buffer);
			break ;
		}
		mi = append_to_multiline(mi, input_buffer);
		add_history(input_buffer);
		free(input_buffer);
	}
	close(vars()->pipefd[1]);
	dup2(vars()->pipefd[0], STDIN_FILENO);
	close(vars()->pipefd[0]);
}

int validate_format(const char *input) {
    const char *equal_sign = strchr(input, '=');

    if (equal_sign == NULL || equal_sign == input) {
        return 0;
    }

    size_t var_length = equal_sign - input;

    for (size_t i = 0; i < var_length; i++) {
        if (!ft_isalnum(input[i])) {
            return 0;
        }
    }
    return 1;
}
