/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:00:05 by mjales            #+#    #+#             */
/*   Updated: 2023/09/04 23:02:12 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int	exit_status;

void	process_quote_state(int *old, int i, int *state)
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
}

void	process_double_quote_state(int *old, int i, int *state)
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

void	lexer(char **envp)
{
	int	i;
	int	old;
	int	state;
	int	space;

	i = -1;
	old = 0;
	state = 0;
	space = 0;

	while (elems()->s[++i])
	{
		if (elems()->s[i] == '\'')
		{
			process_quote_state(&old, i, &state);
			space = 0;
		}
		else if (elems()->s[i] == '\"')
		{
			process_double_quote_state(&old, i, &state);
			space = 0;
		}
		else if (elems()->s[i] == ' ' && state == 0){
			process_space_state(&old, i, &state, &space);
		}
		else if (is_redir(&vars()->s[i]) && state == 0) 
		{
			process_space_state(&old, i, &state, &space);
			i++;
			if (is_redir(&vars()->s[i]) && state == 0)
				i++;
			process_space_state(&old, i, &state, &space);
			process_new_space(&state, &space);
		}
		else if (space)
		{
			ft_lstadd_back(&vars()->tokens, create_token(old, i, state));
			old = i;
			state = 0;
			space = 0;
		}
	}
	if (old != i)
		ft_lstadd_back(&vars()->tokens, create_token(old, i, state));
	find_dollar(envp);

	junta_tokens(vars()->tokens);
	subdivide_tokens();
}


