/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/08/29 16:52:14 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int exit_status;

void add_variable_to_envp(char *new_var)
{
    // Calculate the new size of the envp array
    int new_envp_size = 0;
    char **envp = vars()->envp;
    while (envp[new_envp_size] != NULL){
        new_envp_size++;
    }

    // Allocate memory for the new envp array
    char **new_envp = (char **)malloc((new_envp_size + 2) * sizeof(char *));
    if (!new_envp)
    {
        perror("malloc");
        exit_status = 1;
        exit(1);
    }
    // Copy existing envp variables to the new array
    for (int i = 0; i < new_envp_size; i++)
    {
        new_envp[i] = strdup(envp[i]);
        if (!new_envp[i])
        {
            perror("strdup");
            exit_status = 1;
            exit(1);
        }
    }

    // Add the new variable to the new envp array
    new_envp[new_envp_size] = strdup(new_var);
    new_envp[new_envp_size + 1] = NULL;

    // Penso que temos que mudar o apontador do envp
    int i = 0;
    while(new_envp[i] != NULL)
    {
        // free(envp[i]);
        i++;
    }
    vars()->envp = new_envp;
}

int validate_format(const char *input) {
    const char *equal_sign = strchr(input, '=');

    if (equal_sign == NULL || equal_sign == input) {
        return 0; // No equal sign or empty VAR
    }

    size_t var_length = equal_sign - input;
    char var[var_length + 1];
    strncpy(var, input, var_length);
    var[var_length] = '\0';

    int i = 0;
    // printf("var = %s\n", var);
    while (var[i]){
        if (!ft_isalnum(var[i]))
            return 0; // VAR is not alphanumeric
        i++;
    }
    return 1; // Format is valid
}


int check_alnum(const char *input_string) {
    if (!input_string || (input_string[0] >= '0' && input_string[0] <= '9'))
        return 0;
    while (*input_string) {
        if (!isalnum(*input_string)) {
            return 0;
        }
        input_string++;
    }
    return 1;
}

int exec_export(struct execcmd *ecmd)
{
    int i = 1;
    while (ecmd->argv[i] != NULL)
    {
        // printf("split = %s\n", ft_split(ecmd->argv[i], '=')[0]);
        if (!check_alnum(ft_split(ecmd->argv[i], '=')[0]))
        {
            write(2, " not a valid identifier\n", ft_strlen(" not a valid identifier\n"));
            exit_status = 1;
            if (vars()->forked){
                exit(1);
            }
            return 1;
        }
        if (validate_format(ecmd->argv[i])){  
            add_variable_to_envp(ecmd->argv[i]);
        }
        i++;
    }
    exit_status = 0;
    if (vars()->forked){
        exit(0);
    }
    return 0;
}

int exec_env(void)
{
    int i = 0;
    while (vars()->envp[i] != NULL)
    {
        printf("%s\n", vars()->envp[i]);
        i++;
    }
    exit_status = 0;
    if (vars()->forked)
        exit(0);
    return 0;
}

int exec_builtin(struct execcmd *ecmd)
{
    if (strcmp("export", ecmd->argv[0]) == 0)
        return exec_export(ecmd);
    if (strcmp("env", ecmd->argv[0]) == 0)
        return exec_env();    
    // if (strcmp("cd", ecmd->argv[0]) == 0)
    //     return exec_cd(ecmd);
    // if (strcmp("unset", ecmd->argv[0]) == 0)
    //     return exec_unset(ecmd);
    return -1;
}

void exec_single_command(struct execcmd *ecmd) {
    if (!ecmd->argv[0])
        exit(1);  // Case when no command is received
    if (exec_builtin(ecmd) == -1) {
        execve(ecmd->argv[0], ecmd->argv, vars()->envp);
        fprintf(stderr, "exec %s failed\n", ecmd->argv[0]);
        exit(EXIT_FAILURE);            
    }
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
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exec_tree(pcmd->left);
    } else {
        wait(0);  // Wait for child process
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        exec_tree(pcmd->right);
        exit(EXIT_SUCCESS);
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
