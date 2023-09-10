/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:58:49 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 20:15:21 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	lexer(void)
{
	int	i;
	int	old;
	int	state;
	int	space;

	i = -1;
	old = 0;
	state = 0;
	space = 0;
	elems()->s = vars()->s;
	while (elems()->s[++i])
	{
		token_conditions(&old, &i, &state, &space);
	}
	if (old != i)
		ft_lstadd_back(&vars()->tokens, create_token(old, i, state));
	find_dollar();
	junta_tokens(vars()->tokens);
	subdivide_tokens();
}

t_list	*p_tokens(t_list *current_tokens, int j)
{
	t_list	*new_tokens;
	t_list	*current;
	t_list	*result;
	t_list	*temp;

	new_tokens = NULL;
	current = current_tokens;
	while (current != NULL)
	{
		result = subdivide_current_token(current, j);
		if (!new_tokens)
			new_tokens = result;
		else
		{
			temp = new_tokens;
			while (temp->next)
				temp = temp->next;
			temp->next = result;
		}
		current = current->next;
	}
	return (new_tokens);
}

void	subdivide_tokens(void)
{
	int		j;
	t_list	*new_tokens;

	j = -1;
	while (++j < 14)
	{
		new_tokens = p_tokens(vars()->tokens, j);
		free_tokens(vars()->tokens);
		vars()->tokens = new_tokens;
	}
}

void	free_tokens(t_list *lst)
{
	t_list	*current;
	t_list	*next;

	current = lst;
	while (current != NULL)
	{
		next = current->next;
		free(current->content->s);
		free(current->content);
		free(current);
		current = next;
	}
	vars()->tokens = NULL;
}

t_list	*subdivide_current_token(t_list *current, int j)
{
	t_list	*new_tokens;
	int		state;

	new_tokens = NULL;
	state = current->content->state;
	if (state == DEF && !is_special(current->content->s, vars()->sc))
		new_tokens = handle_token_subdivision(current->content->s, j, state);
	else
		add_token(&new_tokens, current->content->s, state);
	return (new_tokens);
}
