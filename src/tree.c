/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 00:14:27 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:38:05 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

struct s_pipecmd	*setup_pipe(t_list *current, t_list *lst)
{
	struct s_pipecmd	*pipe;

	pipe = malloc(sizeof(*pipe));
	pipe->type = PIPE;
	if (lst->next)
	{
		pipe->right = parsepipe(lst->next->next);
		free(lst->next->content->s);
		free(lst->next->content);
		free(lst->next);
		lst->next = NULL;
	}
	else
		pipe->right = NULL;
	pipe->left = parseredir(current);
	return (pipe);
}

struct s_cmd	*parsepipe(t_list *lst)
{
	t_list			*begin;

	begin = lst;
	if (!lst)
		return (NULL);
	while (lst->next && *(lst->next->content->s) != '|')
		lst = lst->next;
	return ((struct s_cmd *)setup_pipe(begin, lst));
}

struct s_cmd	*create_redircmd(t_list *lst, char *filename, int redir_signal)
{
	struct s_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->file = filename;
	cmd->mode = redir_mode(redir_signal);
	if (redir_signal == IN || redir_signal == HEREDOC)
		cmd->fd = 0;
	else if (redir_signal == OUT || redir_signal == APPEND)
		cmd->fd = 1;
	else
		cmd->fd = 2;
	cmd->cmd = parseredir(lst);
	return ((struct s_cmd *)cmd);
}

int	detect_redirection(t_list *current)
{
	if (current->content->state == DEF)
		return (ft_redir_signal(current->content->s));
	return (0);
}

void	free_nodes(t_list *to_free1, t_list *to_free2)
{
	free(to_free1->content->s);
	free(to_free1->content);
	free(to_free1);
	if (to_free2)
	{
		if (to_free2->content)
			free(to_free2->content);
		free(to_free2);
	}
}
