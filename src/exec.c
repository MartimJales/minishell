/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/08/23 02:44:49 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

        // if(fork1() == 0)
        //     ft_exec(*vars()->tokens); // Agora metemos apenas o exec, no futuro vamos ter o runcmd
        // wait(0);
        // print_tokens();

void exec_pipe(struct pipecmd *pcmd) {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t cpid2 = fork();
    if (cpid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid2 == 0) { /* Child 2 (right side of the pipe) */
        close(pipefd[1]);   // close write end
        dup2(pipefd[0], STDIN_FILENO);  // redirect stdin to read end of pipe
        close(pipefd[0]);

        exec_tree(pcmd->right);
        exit(EXIT_SUCCESS);
    }

    pid_t cpid1 = fork();
    if (cpid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid1 == 0) { /* Child 1 (left side of the pipe) */
        close(pipefd[0]);   // close read end
        dup2(pipefd[1], STDOUT_FILENO);  // redirect stdout to write end of pipe
        close(pipefd[1]);

        exec_tree(pcmd->left);
        exit(EXIT_SUCCESS);
    } 

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);  // wait for child 1
    wait(NULL);  // wait for child 2
}

void exec_tree(struct cmd *root) {
    if (!root) return ;

    switch (root->type) {
        case EXEC:
            {
                struct execcmd *ecmd = (struct execcmd *)root;
                // perror("vamos executar um comando\n");
                if (!ecmd->argv[0])
                    exit(1); // Caso em que não recebemos nenhum comando
                execve(ecmd->argv[0], ecmd->argv, vars()->envp);
                printf("exec %s failed\n", ecmd->argv[0]); // Temos que mandar isto para o stderror
            }
            break;
        case REDIR:
            {
                // perror("vamos redirecionar algo\n");
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
                // perror("vamos mexer nas canalizações\n");
                struct pipecmd *pcmd = (struct pipecmd *)root;
                if (!pcmd->right){
                    exec_tree(pcmd->left);
                }
                exec_pipe(pcmd);
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
