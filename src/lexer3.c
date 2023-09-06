/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:14 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:20:39 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_list	*create_token(int start, int end, int state)
{
	int		size;
	t_list	*aux;

	size = end - start + 1;
	aux = ft_lstnew(NULL);
	aux->content = malloc(sizeof(t_elems));
	aux->content->s = malloc(size);
	ft_strlcpy(aux->content->s, elems()->s + start, size);
	aux->content->state = state;
	return (aux);
}

t_list	*create_space_token( int state)
{
	t_list	*aux;

	aux = ft_lstnew(NULL);
	aux->content = malloc(sizeof(t_elems));
	aux->content->s = strdup(" ");
	aux->content->state = state;
	return (aux);
}

void	find_dollar(void)
{
	t_list	*aux;

	aux = vars()->tokens;
	while (aux != NULL)
	{
		if (aux->content->state != SQ)
			aux->content->s = replace_dollar(aux->content->s);
		aux = aux->next;
	}
}

void	add_token(t_list **list, const char *token_str, int state)
{
	t_list	*new_node;
	t_elems	*content;
	t_list	*current;

	new_node = ft_lstnew(NULL);
	content = (t_elems *)malloc(sizeof(t_elems));
	content->s = strdup(token_str);
	content->state = state;
	new_node->content = content;
	if (*list == NULL)
		*list = new_node;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}
