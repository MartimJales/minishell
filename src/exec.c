/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/08/22 14:06:17 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

        // if(fork1() == 0)
        //     ft_exec(*vars()->tokens); // Agora metemos apenas o exec, no futuro vamos ter o runcmd
        // wait(0);
        // print_tokens();

void exec_tree(struct cmd *root) {
    if (!root) return ;

    switch (root->type) {
        case EXEC:
            {
                struct execcmd *e = (struct execcmd *)root;
                printf("vamos executar isto: %s\n", e->argv[0]);
                if (!e->argv[0])
                    exit(1); // Caso em que não recebemos nenhum comando
                execve(e->argv[0], e->argv, vars()->envp);
                printf("exec %s failed\n", e->argv[0]); // Temos que mandar isto para o stderror
            }
            break;
        case REDIR:
            {
                // tratamento do redir
                struct redircmd *rcmd = (struct redircmd*)root;
                close(rcmd->fd);
                if(open(rcmd->file, rcmd->mode, 0664) < 0){
                    printf("open %s failed\n", rcmd->file);
                    exit(1);
                }
                exec_tree(rcmd->cmd);
                break;
            }
        break;
        case PIPE:
            {
                struct pipecmd *p = (struct pipecmd *)root;
                if (p->right){
                    // tratamento do pipe
                }
                exec_tree(p->left);
            }
            break;
        default:
            printf("Unknown cmd type: %d\n", root->type);
            break;
    }
}


int    ft_exec(t_list args)
{
    char	**teste;

    teste = check_path(&args, vars()->path_arg);

    return (execve(teste[0], teste, vars()->envp));
}
