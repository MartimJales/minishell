/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:06:47 by mjales            #+#    #+#             */
/*   Updated: 2023/09/09 00:12:22 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_redir_signal(char *s)
{
	if (ft_strncmp(">>", s, 2) == 0)
		return (APPEND);
	if (ft_strncmp("<<", s, 2) == 0)
		return (HEREDOC);
	if (*s == '>')
		return (OUT);
	if (*s == '<')
		return (IN);
	return (0);
}

int	redir_mode(int redir_signal)
{
	if (redir_signal == IN)
		return (O_RDONLY);
	if (redir_signal == OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	if (redir_signal == APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	if (redir_signal == HEREDOC)
		return (HEREDOC);
	return (0);
}

void	exec_redir(struct s_redircmd *rcmd)
{
	if (rcmd->mode == HEREDOC)
		heredoc(rcmd->file);
	else
	{
		close(rcmd->fd);
		if (open(rcmd->file, rcmd->mode, 0664) < 0)
			g_exit_status = EXIT_FAILURE;
	}
	exec_tree(rcmd->cmd);
}

char	*ft_strchr(const char *s, int c)
{
	char	ch;

	ch = (char)c;
	if (s[0] == ch)
	{
		return ((char *)s);
	}
	while (*s++)
	{
		if (*s == ch)
		{
			return ((char *)s);
		}
	}
	return (NULL);
}

char	*ft_strstr(const char *s1, const char *s2)
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp;

	tmp = (char *)s1;
	if (!*s2)
		return ((char *)s1);
	while (*tmp)
	{
		tmp1 = (char *)tmp;
		tmp2 = (char *)s2;
		while (*tmp && *tmp2 && *tmp == *tmp2)
		{
			tmp++;
			tmp2++;
		}
		if (!*tmp2)
			return (tmp1);
		tmp = tmp1 + 1;
	}
	return (NULL);
}


