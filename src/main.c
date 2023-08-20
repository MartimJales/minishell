/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/08/10 00:56:45 by mjales           ###   ########.fr       */
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
    vars()->path_arg = create_path(envp);
    vars()->envp = envp;
    // for (int i = 0; vars()->path_arg[i]; i++)
    // {
    //     printf("%s\n", vars()->path_arg[i]);
    // }
    signal(SIGINT, signal_cmd);
	signal(SIGQUIT, SIG_IGN);
    while (1)
	{
		vars()->num_sc = 14;
        create_sc();
		vars()->s = readline("minishell>");
        signal(SIGINT, signal_cmd);
		signal(SIGQUIT, SIG_IGN);
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
        if(fork1() == 0)
            ft_exec(*vars()->tokens); // Agora metemos apenas o exec, no futuro vamos ter o runcmd
        wait(0);
        free_tokens();
	}
(void)argc;
(void)argv;
(void)envp;
	return (0);
}
