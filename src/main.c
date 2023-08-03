/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/08/03 19:55:12 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_vars	*vars(void)
{
	static t_vars	var;
    
	return (&var);
}

t_elems	*elems(void)
{
	static 	t_elems elem;

	return (&elem);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)argc;
    
	(void)envp;
    while (1)
	{
		vars()->num_sc = 14;
        create_sc();
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
        add_history(vars()->s);
		elems()->s = vars()->s;
		lexer(envp);
        // free_tokens(vars()->tokens);
	}
(void)argc;
(void)argv;
(void)envp;
	return (0);
}
