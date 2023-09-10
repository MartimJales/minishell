/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 20:20:27 by dcordovi          #+#    #+#             */
/*   Updated: 2023/09/10 20:20:28 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

int	validate_format(const char *input)
{
	const char	*equal_sign;
	const char	*ptr;

	equal_sign = strchr(input, '=');
	ptr = input;
	if (equal_sign == NULL || equal_sign == input)
		return (0);
	while (ptr != equal_sign)
	{
		if (!ft_isalnum(*ptr))
			return (0);
		ptr++;
	}
	return (1);
}
