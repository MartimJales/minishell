/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 00:14:27 by mjales            #+#    #+#             */
/*   Updated: 2023/08/31 00:32:56 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

struct cmd	*parsepipe(t_list *lst)
{
	t_list			*begin;
	struct pipecmd	*pipe;
	t_list			*current;

	begin = lst;
	if (!lst)
		return (NULL);
	while (lst->next && *(lst->next->content->s) != '|')
		lst = lst->next;
	pipe = malloc(sizeof(*pipe));
	pipe->type = PIPE;
	current = begin; 
	if (lst->next)
		pipe->right = parsepipe(lst->next->next);
	else
	{
		pipe->left = parseredir(current);
		pipe->right = NULL;
		return ((struct cmd *)pipe);
	}
	lst->next = NULL;
	pipe->left = parseredir(current);
	return ((struct cmd *)pipe);
}

struct cmd	*create_redircmd(t_list *lst, char *filename, int redir_signal)
{
	struct redircmd	*cmd;

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
	return ((struct cmd *)cmd);
}

struct cmd	*parseredir(t_list *lst)
{
	t_list	*old;
	t_list	*prev;
	int		redir_signal;

	old = lst;
	prev = malloc(sizeof(t_list));
	prev->next = lst;
	if (!lst)
		return (NULL);
	while (lst)
	{
		if (lst->content->state == DEF)
			redir_signal = ft_redir_signal(lst->content->s);
		if (redir_signal != 0)
		{
			prev->next = prev->next->next->next;
			return (create_redircmd(old, lst->next->content->s, redir_signal));
		}
		else 
		{
			prev = prev->next;
			lst = lst->next;
		}
	}
	return (parseexec(old));
}

struct cmd	*parseexec(t_list *lst)
{
	struct execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	cmd->argv = check_path(lst, vars()->path_arg);
	return ((struct cmd *)cmd);
}