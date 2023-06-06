/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/06/06 11:24:29 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_vars	*vars(void)
{
	static t_vars	var;

	return (&var);
}

int	main(int argc, char **argv, char **envp)
{
	while (1)
	{
		vars()->s = readline("minishell>");
		if (vars()->s == NULL)
			break ;
		if (strcmp(vars()->s, "exit") == 0)
		{
			free(vars()->s);
			break ;
		}
		if (all_space(vars()->s))
		{
			free(vars()->s);
			continue ;
		}
		printf("%s\n", vars()->s);
		add_history(vars()->s);
		vars()->tokens = lexer(vars()->s);
		create_sc();
		int i = 0;
		while (vars()->tokens[i])
		{
			printf("%d = %s\n", i, vars()->tokens[i]);
			i++;
		}
		free(vars()->s);
	}
	(void)argc;
	(void)argv;
	(void)envp;
	return (0);
}
