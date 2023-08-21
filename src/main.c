/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/08/21 23:58:26 by mjales           ###   ########.fr       */
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

void debug_tree(struct cmd *root) {
    if (!root) return ;

    switch (root->type) {
        case EXEC:
            {
                struct execcmd *e = (struct execcmd *)root;
                printf("EXEC cmd:\n");
                for (int i = 0; e->argv && e->argv[i]; i++) {
                    printf("  argv[%d] = %s\n", i, e->argv[i]);
                }
            }
            break;
        case REDIR:
            {
                struct redircmd *r = (struct redircmd *)root;
                printf("REDIR cmd:\n");
                printf("  file: %s\n", r->file);
                printf("  mode: %d\n", r->mode);
                // Uncomment the line below if you add the fd field later
                // printf("  fd: %d\n", r->fd);
                printf("  -> Nested cmd:\n");
                debug_tree(r->cmd);
            }
            break;
        case PIPE:
            {
                struct pipecmd *p = (struct pipecmd *)root;
                printf("PIPE cmd:\n");
                printf("  -> Left cmd:\n");
                debug_tree(p->left);
                printf("  -> Right cmd:\n");
                debug_tree(p->right);
            }
            break;
        default:
            printf("Unknown cmd type: %d\n", root->type);
            break;
    }
}




int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)argc;
    
	(void)envp;
    vars()->path_arg = create_path(envp);
    vars()->envp = envp;
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
        // if(fork1() == 0)
        //     ft_exec(*vars()->tokens); // Agora metemos apenas o exec, no futuro vamos ter o runcmd
        // wait(0);
        // print_tokens();
        struct pipecmd *pipe = (struct pipecmd *)parsepipe(vars()->tokens);
        debug_tree((struct cmd *)pipe);
        // struct redircmd *redir = NULL;
        // while (pipe->right != NULL) {
        //     redir = (struct redircmd *)(pipe->left);  
        //     printf("redir = {%s}\n", redir->debug);
        //     pipe = (struct pipecmd *)pipe->right;
        // }
        (void)pipe;
        free_tokens();
	}
(void)argc;
(void)argv;
(void)envp;
	return (0);
}
