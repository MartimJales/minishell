/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:51 by mjales            #+#    #+#             */
/*   Updated: 2023/09/08 16:02:46 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**list_to_array(t_list *lst)
{
	int		count;
	t_list	*tmp;
	char	**arr;
	int		i;

	count = 0;
	tmp = lst;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	tmp = lst;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->content->s);
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

struct s_cmd	*handle_redirection(t_list **old, \
t_list *current, t_list *prev, int redir_signal)
{
	t_list	*to_free1;
	t_list	*to_free2;
	char	*content_s;

	to_free1 = current;
	to_free2 = current->next;
	content_s = NULL;
	if (current->next && current->next->content)
		content_s = current->next->content->s;
	if (prev)
		prev->next = current->next->next;
	else
		*old = current->next->next;
	free_nodes(to_free1, to_free2);
	return (create_redircmd(*old, content_s, redir_signal));
}

struct s_cmd	*parseredir(t_list *lst)
{
	t_list	*old;
	t_list	*prev;
	t_list	*current;
	int		redir_signal;

	old = lst;
	prev = NULL;
	current = lst;
	redir_signal = 0;
	if (!lst)
		return (NULL);
	while (current)
	{
		redir_signal = detect_redirection(current);
		if (redir_signal != 0)
			return (handle_redirection(&old, current, prev, redir_signal));
		prev = current;
		current = current->next;
	}
	return (parseexec(old));
}

struct s_cmd	*parseexec(t_list *lst)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	cmd->argv = check_path(lst, vars()->path_arg);
	free_tokens(lst);
	return ((struct s_cmd *)cmd);
}
