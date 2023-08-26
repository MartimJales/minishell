/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/08/24 02:16:05 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void exec_single_command(struct execcmd *ecmd) {
    if (!ecmd->argv[0])
        exit(1);  // Case when no command is received
    execve(ecmd->argv[0], ecmd->argv, vars()->envp);
    fprintf(stderr, "exec %s failed\n", ecmd->argv[0]);
    exit(EXIT_FAILURE);
}

void exec_redir(struct redircmd *rcmd) {
    close(rcmd->fd);
    if (open(rcmd->file, rcmd->mode, 0664) < 0) {
        fprintf(stderr, "open %s failed\n", rcmd->file);
        exit(EXIT_FAILURE);
    }
    exec_tree(rcmd->cmd);
}

// Auxiliary function for executing a pipe command
void exec_pipe_command(struct pipecmd *pcmd) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        exec_tree(pcmd->right);
        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exec_tree(pcmd->left);
        wait(NULL);  // Wait for child process
    }
}

void exec_tree(struct cmd *root) {
    if (!root) return;

    switch (root->type) {
        case EXEC:
            exec_single_command((struct execcmd *)root);
            break;
        case REDIR:
            exec_redir((struct redircmd *)root);
            break;
        case PIPE:
            {
                struct pipecmd *pcmd = (struct pipecmd *)root;
                if (!pcmd->right) {
                    exec_tree(pcmd->left);
                } else {
                    exec_pipe_command(pcmd);
                }
            }
            break;
        default:
            fprintf(stderr, "Unknown cmd type: %d\n", root->type);
            break;
    }
}


int    ft_exec(t_list args)
{
    char	**teste;

    teste = check_path(&args, vars()->path_arg);

    return (execve(teste[0], teste, vars()->envp));
}
