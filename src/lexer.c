/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 13:58:49 by mjales            #+#    #+#             */
/*   Updated: 2023/09/05 13:59:12 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		process_token_conditions(&old, &i, &state, &space);
	}
	if (old != i)
		ft_lstadd_back(&vars()->tokens, create_token(old, i, state));
	find_dollar(envp);
	junta_tokens(vars()->tokens);
	subdivide_tokens();
}
