/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   junta_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:09 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:09:55 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_tokens(t_list *lst)
{
	t_list	*current;

	current = lst;
	printf("Tokens:\n");
	while (current != NULL)
	{
		printf("token(%d) = {%s}\n", current->content->state, \
current->content->s);
		current = current->next;
	}
}

int	is_redir(const char *str)
{
	if (strncmp(str, ">>", 2) == 0)
		return (1);
	if (strncmp(str, "<<", 2) == 0)
		return (1);
	if (strncmp(str, ">", 1) == 0)
		return (1);
	if (strncmp(str, "<", 1) == 0)
		return (1);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*exp_dollar(char *s, char **envp)
{
	char	*path;
	int		i;

	i = 0;
	if (s == NULL)
		return ("");
	s = junta_strings(s, "=");
	while (envp[i])
	{
		path = ft_strnstr(envp[i], s, ft_strlen(s));
		if (path)
			break ;
		i++;
	}
	i = ft_strlen(s);
	free(s);
	if (!path)
		return ("$");
	return (path + i);
}

void	junta_tokens(t_list *lst)
{
	t_list	*cur;
	t_list	*tmp;

	cur = lst;
	while (cur && cur->next)
	{
		if (strcmp(cur->content->s, " ") != 0 && \
strcmp(cur->next->content->s, " ") != 0)
		{
			cur->content->s = \
junta_strings(cur->content->s, cur->next->content->s);
			if (cur->next->content->state > cur->content->state)
				cur->content->state = cur->next->content->state;
			tmp = cur->next;
			cur->next = tmp->next;
			free(tmp->content->s);
			free(tmp->content);
			free(tmp);
		}
		else
		{
			cur = cur->next;
		}
	}
}
