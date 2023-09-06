/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:00:05 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:30:25 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	process_quote_state(int *old, int i, int *state, int *space)
{
	if (*state == 0)
	{
		ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
		*old = i + 1;
		*state = 1;
	}
	else if (*state == 1)
	{
		ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
		*old = i + 1;
		*state = 0;
	}
	*space = 0;
}

void	process_double_quote_state(int *old, int i, int *state, int *space)
{
	if (*state == 0)
	{
		ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
		*old = i + 1;
		*state = 2;
	}
	else if (*state == 2)
	{
		ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
		*old = i + 1;
		*state = 0;
	}
	*space = 0;
}

void	process_space_state(int *old, int i, int *state, int *space)
{
	if (*state == 0)
	{
		ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
		*old = i;
		*state = 0;
		*space = 1;
	}
}

void	process_new_space(int *state, int *space)
{
	ft_lstadd_back(&vars()->tokens, create_space_token(*state));
	*state = 0;
	*space = 0;
}

void	token_conditions(int *old, int *i, int *state, int *space)
{
	if (elems()->s[*i] == '\'')
		process_quote_state(old, *i, state, space);
	else if (elems()->s[*i] == '\"')
		process_double_quote_state(old, *i, state, space);
	else if (elems()->s[*i] == ' ' && *state == 0)
		process_space_state(old, *i, state, space);
	else if (is_redir(&vars()->s[*i]) && *state == 0)
	{
		process_space_state(old, *i, state, space);
		(*i)++;
		if (is_redir(&vars()->s[*i]) && *state == 0)
			(*i)++;
		process_space_state(old, *i, state, space);
		process_new_space(state, space);
	}
	else if (*space)
	{
		ft_lstadd_back(&vars()->tokens, create_token(*old, *i, *state));
		*old = *i;
		*state = 0;
		*space = 0;
	}
}
