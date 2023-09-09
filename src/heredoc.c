/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:01 by mjales            #+#    #+#             */
/*   Updated: 2023/09/09 03:49:44 by mjales           ###   ########.fr       */
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

void	heredoc(const char *delimiter)
{
	char	*input_buffer;
	char	*mi;

	input_buffer = NULL;
	mi = NULL;
	init_pipe();
	while (1)
	{
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
	size_t		var_length;
	int			i;
	char		*var;

	equal_sign = ft_strchr(input, '=');
	if (equal_sign == NULL || equal_sign == input)
		return (0);
	var_length = equal_sign - input;
	var = malloc(var_length + 1);
	ft_strncpy(var, input, var_length);
	var[var_length] = '\0';
	i = 0;
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
		{
			free(var);
			return (0);
		}
		i++;
	}
	free(var);
	return (1);
}
